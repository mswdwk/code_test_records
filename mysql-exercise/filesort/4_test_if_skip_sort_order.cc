static bool
test_if_skip_sort_order(JOIN_TAB *tab, ORDER *order, ha_rows select_limit,
                        const bool no_changes, const key_map *map,
                        const char *clause_type)
{
  int ref_key;
  uint ref_key_parts= 0;
  int order_direction= 0;
  uint used_key_parts;
  TABLE *const table= tab->table();
  JOIN *const join= tab->join();
  THD *const thd= join->thd;
  QUICK_SELECT_I *const save_quick= tab->quick();
  int best_key= -1;
  bool set_up_ref_access_to_key= false;
  bool can_skip_sorting= false;                  // used as return value
  int changed_key= -1;
  DBUG_ENTER("test_if_skip_sort_order");

  /* Check that we are always called with first non-const table */
  DBUG_ASSERT((uint)tab->idx() == join->const_tables);

  Plan_change_watchdog watchdog(tab, no_changes);

  /* Sorting a single row can always be skipped */
  if (tab->type() == JT_EQ_REF ||
      tab->type() == JT_CONST  ||
      tab->type() == JT_SYSTEM)
  {
    DBUG_RETURN(1);
  }

  /*
    Check if FT index can be used to retrieve result in the required order.
    It is possible if ordering is on the first non-constant table.
  */
  if (join->order && join->simple_order)
  {
    /*
      Check if ORDER is DESC, ORDER BY is a single MATCH function.
    */
    Item_func_match *ft_func= test_if_ft_index_order(order);
    /*
      Two possible cases when we can skip sort order:
      1. FT_SORTED must be set(Natural mode, no ORDER BY).
      2. If FT_SORTED flag is not set then
      the engine should support deferred sorting. Deferred sorting means
      that sorting is postponed utill the start of index reading(InnoDB).
      In this case we set FT_SORTED flag here to let the engine know that
      internal sorting is needed.
    */
    if (ft_func && ft_func->ft_handler && ft_func->ordered_result())
    {
      /*
        FT index scan is used, so the only additional requirement is
        that ORDER BY MATCH function is the same as the function that
        is used for FT index.
      */
      if (tab->type() == JT_FT &&
          ft_func->eq(tab->position()->key->val, true))
      {
        ft_func->set_hints(join, FT_SORTED, select_limit, false);
        DBUG_RETURN(true);
      }
      /*
        No index is used, it's possible to use FT index for ORDER BY if
        LIMIT is present and does not exceed count of the records in FT index
        and there is no WHERE condition since a condition may potentially
        require more rows to be fetch from FT index.
      */
      else if (!tab->condition() &&
               select_limit != HA_POS_ERROR &&
               select_limit <= ft_func->get_count())
      {
        /* test_if_ft_index_order() always returns master MATCH function. */
        DBUG_ASSERT(!ft_func->master);
        /* ref is not set since there is no WHERE condition */
        DBUG_ASSERT(tab->ref().key == -1);

        /*Make EXPLAIN happy */
        tab->set_type(JT_FT);
        tab->ref().key= ft_func->key;
        tab->ref().key_parts= 0;
        tab->set_index(ft_func->key);
        tab->set_ft_func(ft_func);

        /* Setup FT handler */
        ft_func->set_hints(join, FT_SORTED, select_limit, true);
        ft_func->join_key= true;
        table->file->ft_handler= ft_func->ft_handler;
        DBUG_RETURN(true);
      }
    }
  }
  
  /*
    Keys disabled by ALTER TABLE ... DISABLE KEYS should have already
    been taken into account.
  */
  key_map usable_keys= *map;

  for (ORDER *tmp_order=order; tmp_order ; tmp_order=tmp_order->next)
  {
    Item *item= (*tmp_order->item)->real_item();
    if (item->type() != Item::FIELD_ITEM)
    {
      usable_keys.clear_all();
      DBUG_RETURN(0);
    }
    usable_keys.intersect(((Item_field*) item)->field->part_of_sortkey);
    if (usable_keys.is_clear_all())
      DBUG_RETURN(0);					// No usable keys
  }
  if (tab->type() == JT_REF_OR_NULL || tab->type() == JT_FT)
    DBUG_RETURN(0);

  ref_key= -1;
  /* Test if constant range in WHERE */
  if (tab->type() == JT_REF)
  {
    DBUG_ASSERT(tab->ref().key >= 0 && tab->ref().key_parts);
    ref_key=	   tab->ref().key;
    ref_key_parts= tab->ref().key_parts;
  }
  else if (tab->type() == JT_RANGE || tab->type() == JT_INDEX_MERGE)
  {
    // Range found by opt_range
    int quick_type= tab->quick()->get_type();
    /* 
      assume results are not ordered when index merge is used 
      TODO: sergeyp: Results of all index merge selects actually are ordered 
      by clustered PK values.
    */
  
    if (quick_type == QUICK_SELECT_I::QS_TYPE_INDEX_MERGE || 
        quick_type == QUICK_SELECT_I::QS_TYPE_ROR_UNION || 
        quick_type == QUICK_SELECT_I::QS_TYPE_ROR_INTERSECT)
      DBUG_RETURN(0);
    ref_key=	   tab->quick()->index;
    ref_key_parts= tab->quick()->used_key_parts;
  }
  else if (tab->type() == JT_INDEX_SCAN)
  {
    // The optimizer has decided to use an index scan.
    ref_key=       tab->index();
    ref_key_parts= actual_key_parts(&table->key_info[tab->index()]);
  }

  Opt_trace_context * const trace= &thd->opt_trace;
  Opt_trace_object trace_wrapper(trace);
  Opt_trace_object
    trace_skip_sort_order(trace, "reconsidering_access_paths_for_index_ordering");
  trace_skip_sort_order.add_alnum("clause", clause_type);

  if (ref_key >= 0)
  {
    /*
      We come here when ref/index scan/range scan access has been set
      up for this table. Do not change access method if ordering is
      provided already.
    */
    if (!usable_keys.is_set(ref_key))
    {
      /*
        We come here when ref_key is not among usable_keys, try to find a
        usable prefix key of that key.
      */
      uint new_ref_key;
      /*
	If using index only read, only consider other possible index only
	keys
      */
      if (table->covering_keys.is_set(ref_key))
	usable_keys.intersect(table->covering_keys);

      if ((new_ref_key= test_if_subkey(order, tab, ref_key, ref_key_parts,
				       &usable_keys)) < MAX_KEY)
      {
	/* Found key that can be used to retrieve data in sorted order */
	if (tab->ref().key >= 0)
        {
          /*
            We'll use ref access method on key new_ref_key. The actual change
            is done further down in this function where we update the plan.
          */
          set_up_ref_access_to_key= true;
        }
	else if (!no_changes)
	{
          /*
            The range optimizer constructed QUICK_RANGE for ref_key, and
            we want to use instead new_ref_key as the index. We can't
            just change the index of the quick select, because this may
            result in an incosistent QUICK_SELECT object. Below we
            create a new QUICK_SELECT from scratch so that all its
            parameres are set correctly by the range optimizer.

            Note that the range optimizer is NOT called if
            no_changes==true. This reason is that the range optimizer
            cannot find a QUICK that can return ordered result unless
            index access (ref or index scan) is also able to do so
            (which test_if_order_by_key () will tell).
            Admittedly, range access may be much more efficient than
            e.g. index scan, but the only thing that matters when
            no_change==true is the answer to the question: "Is it
            possible to avoid sorting if an index is used to access
            this table?". The answer does not depend on the outcome of
            the range optimizer.
          */
          key_map new_ref_key_map;  // Force the creation of quick select
          new_ref_key_map.set_bit(new_ref_key); // only for new_ref_key.

          Opt_trace_object
            trace_recest(trace, "rows_estimation");
          trace_recest.add_utf8_table(tab->table_ref).
          add_utf8("index", table->key_info[new_ref_key].name);
          QUICK_SELECT_I *qck;
          const bool no_quick=
            test_quick_select(thd, new_ref_key_map,
                              0,       // empty table_map
                              join->calc_found_rows ?
                                HA_POS_ERROR :
                                join->unit->select_limit_cnt,
                              false,   // don't force quick range
                              order->direction, tab,
                              // we are after make_join_select():
                              tab->condition(), &tab->needed_reg,
                              &qck) <= 0;
          DBUG_ASSERT(tab->quick() == save_quick);
          tab->set_quick(qck);
          if (no_quick)
          {
            can_skip_sorting= false;
            goto fix_ICP;
          }
	}
        ref_key= new_ref_key;
        changed_key= new_ref_key;
      }
    }
    /* Check if we get the rows in requested sorted order by using the key */
    if (usable_keys.is_set(ref_key) &&
        (order_direction= test_if_order_by_key(order,table,ref_key,
					       &used_key_parts)))
      goto check_reverse_order;
  }
  {
    /*
      There is no ref/index scan/range scan access set up for this
      table, or it does not provide the requested ordering. Do a
      cost-based search on all keys.
    */
    uint best_key_parts= 0;
    uint saved_best_key_parts= 0;
    int best_key_direction= 0;
    ha_rows table_records= table->file->stats.records;

    /*
      If an index scan that cannot provide ordering has been selected
      then do not use the index scan key as starting hint to
      test_if_cheaper_ordering()
    */
    const int ref_key_hint= (order_direction == 0 &&
                             tab->type() == JT_INDEX_SCAN) ? -1 : ref_key;

    test_if_cheaper_ordering(tab, order, table, usable_keys,
                             ref_key_hint,
                             select_limit,
                             &best_key, &best_key_direction,
                             &select_limit, &best_key_parts,
                             &saved_best_key_parts);

    if (best_key < 0)
    {
      // No usable key has been found
      can_skip_sorting= false;
      goto fix_ICP;
    }

    /*
      Does the query have a "FORCE INDEX [FOR GROUP BY] (idx)" (if
      clause is group by) or a "FORCE INDEX [FOR ORDER BY] (idx)" (if
      clause is order by)?
    */
    const bool is_group_by= join && join->grouped && order == join->group_list;
    const bool is_force_index= table->force_index ||
      (is_group_by ? table->force_index_group : table->force_index_order);

    /*
      filesort() and join cache are usually faster than reading in
      index order and not using join cache. Don't use index scan
      unless:
       - the user specified FORCE INDEX [FOR {GROUP|ORDER} BY] (have to assume
         the user knows what's best)
       - the chosen index is clustered primary key (table scan is not cheaper)
    */
    if (!is_force_index &&
        (select_limit >= table_records) &&
        (tab->type() == JT_ALL &&
         join->primary_tables > join->const_tables + 1) &&
         ((unsigned) best_key != table->s->primary_key ||
          !table->file->primary_key_is_clustered()))
    {
      can_skip_sorting= false;
      goto fix_ICP;
    }

    if (table->quick_keys.is_set(best_key) &&
        !tab->quick_order_tested.is_set(best_key) &&
        best_key != ref_key)
    {
      tab->quick_order_tested.set_bit(best_key);
      Opt_trace_object
        trace_recest(trace, "rows_estimation");
      trace_recest.add_utf8_table(tab->table_ref).
        add_utf8("index", table->key_info[best_key].name);

      key_map keys_to_use;           // Force the creation of quick select
      keys_to_use.set_bit(best_key); // only best_key.
      QUICK_SELECT_I *qck;
      test_quick_select(thd,
                        keys_to_use,
                        0,        // empty table_map
                        join->calc_found_rows ?
                        HA_POS_ERROR :
                        join->unit->select_limit_cnt,
                        true,     // force quick range
                        order->direction, tab, tab->condition(),
                        &tab->needed_reg, &qck);
      /*
        If tab->quick() pointed to another quick than save_quick, we would
        lose access to it and leak memory.
      */
      DBUG_ASSERT(tab->quick() == save_quick || tab->quick() == NULL);
      tab->set_quick(qck);
    }
    order_direction= best_key_direction;
    /*
      saved_best_key_parts is actual number of used keyparts found by the
      test_if_order_by_key function. It could differ from keyinfo->key_parts,
      thus we have to restore it in case of desc order as it affects
      QUICK_SELECT_DESC behaviour.
    */
    used_key_parts= (order_direction == -1) ?
      saved_best_key_parts :  best_key_parts;
    changed_key= best_key;
    // We will use index scan or range scan:
    set_up_ref_access_to_key= false;
  }

check_reverse_order:                  
  DBUG_ASSERT(order_direction != 0);

  if (order_direction == -1)		// If ORDER BY ... DESC
  {
    if (tab->quick())
    {
      /*
	Don't reverse the sort order, if it's already done.
        (In some cases test_if_order_by_key() can be called multiple times
      */
      if (tab->quick()->reverse_sorted())
      {
        can_skip_sorting= true;
        goto fix_ICP;
      }

      if (tab->quick()->reverse_sort_possible())
        can_skip_sorting= true;
      else
      {
        can_skip_sorting= false;
        goto fix_ICP;
      }
    }
    else
    {
      // Other index access (ref or scan) poses no problem
      can_skip_sorting= true;
    }
  }
  else
  {
    // ORDER BY ASC poses no problem
    can_skip_sorting= true;
  }

  DBUG_ASSERT(can_skip_sorting);

  /*
    Update query plan with access pattern for doing 
    ordered access according to what we have decided
    above.
  */
  if (!no_changes) // We are allowed to update QEP
  {
    if (set_up_ref_access_to_key)
    {
      /*
        We'll use ref access method on key changed_key. In general case 
        the index search tuple for changed_ref_key will be different (e.g.
        when one index is defined as (part1, part2, ...) and another as
        (part1, part2(N), ...) and the WHERE clause contains 
        "part1 = const1 AND part2=const2". 
        So we build tab->ref() from scratch here.
      */
      Key_use *keyuse= tab->keyuse();
      while (keyuse->key != (uint)changed_key &&
             keyuse->table_ref == tab->table_ref)
        keyuse++;

      if (create_ref_for_key(join, tab, keyuse, tab->prefix_tables()))
      {
        can_skip_sorting= false;
        goto fix_ICP;
      }

      DBUG_ASSERT(tab->type() != JT_REF_OR_NULL && tab->type() != JT_FT);

      // Changing the key makes filter_effect obsolete
      tab->position()->filter_effect= COND_FILTER_STALE;
    }
    else if (best_key >= 0)
    {
      /*
        If ref_key used index tree reading only ('Using index' in EXPLAIN),
        and best_key doesn't, then revert the decision.
      */
      if(!table->covering_keys.is_set(best_key))
        table->set_keyread(false);
      if (!tab->quick() || tab->quick() == save_quick) // created no QUICK
      {
        // Avoid memory leak:
        DBUG_ASSERT(tab->quick() == save_quick || tab->quick() == NULL);
        tab->set_quick(NULL);
        tab->set_index(best_key);
        tab->set_type(JT_INDEX_SCAN);       // Read with index_first(), index_next()
        /*
          There is a bug. When we change here, e.g. from group_min_max to
          index scan: loose index scan expected to read a small number of rows
          (jumping through the index), this small number was in
          position()->rows_fetched; index scan will read much more, so
          rows_fetched should be updated. So should the filtering effect.
          It is visible in main.distinct in trunk:
          explain SELECT distinct a from t3 order by a desc limit 2;
          id	select_type	table	partitions	type	possible_keys	key	key_len	ref	rows	filtered	Extra
          1	SIMPLE	t3	NULL	index	a	a	5	NULL	40	25.00	Using index
          "rows=40" should be ~200 i.e. # of records in table. Filter should be
          100.00 (no WHERE).
        */
        table->file->ha_index_or_rnd_end();
        if (thd->lex->is_explain())
        {
          /*
            @todo this neutralizes add_ref_to_table_cond(); as a result
            EXPLAIN shows no "using where" though real SELECT has one.
          */
          tab->ref().key= -1;
          tab->ref().key_parts= 0;
        }
        tab->position()->filter_effect= COND_FILTER_STALE;
      }
      else if (tab->type() != JT_ALL)
      {
        /*
          We're about to use a quick access to the table.
          We need to change the access method so as the quick access
          method is actually used.
        */
        DBUG_ASSERT(tab->quick());
        DBUG_ASSERT(tab->quick()->index==(uint)best_key);
        tab->set_type(calc_join_type(tab->quick()->get_type()));
        tab->use_quick=QS_RANGE;
        tab->ref().key= -1;
        tab->ref().key_parts=0;		// Don't use ref key.
        if (tab->quick()->is_loose_index_scan())
          join->tmp_table_param.precomputed_group_by= TRUE;
        tab->position()->filter_effect= COND_FILTER_STALE;
      }
    } // best_key >= 0

    if (order_direction == -1)		// If ORDER BY ... DESC
    {
      if (tab->quick())
      {
        /* ORDER BY range_key DESC */
        QUICK_SELECT_I *tmp= tab->quick()->make_reverse(used_key_parts);
        if (!tmp)
        {
          /* purecov: begin inspected */
          can_skip_sorting= false;      // Reverse sort failed -> filesort
          goto fix_ICP;
          /* purecov: end */
        }
        if (tab->quick() != tmp && tab->quick() != save_quick)
          delete tab->quick();
        tab->set_quick(tmp);
        tab->set_type(calc_join_type(tmp->get_type()));
        tab->position()->filter_effect= COND_FILTER_STALE;
      }
      else if (tab->type() == JT_REF &&
               tab->ref().key_parts <= used_key_parts)
      {
        /*
          SELECT * FROM t1 WHERE a=1 ORDER BY a DESC,b DESC

          Use a traversal function that starts by reading the last row
          with key part (A) and then traverse the index backwards.
        */
        tab->reversed_access= true;

        /*
          The current implementation of join_read_prev_same() does not
          work well in combination with ICP and can lead to increased
          execution time. Setting changed_key to the current key
          (based on that we change the access order for the key) will
          ensure that a pushed index condition will be cancelled.
        */
        changed_key= tab->ref().key;
      }
      else if (tab->type() == JT_INDEX_SCAN)
        tab->reversed_access= true;
    }
    else if (tab->quick())
      tab->quick()->need_sorted_output();

  } // QEP has been modified

fix_ICP:
  /*
    Cleanup:
    We may have both a 'tab->quick()' and 'save_quick' (original)
    at this point. Delete the one that we won't use.
  */
  if (can_skip_sorting && !no_changes)
  {
    if (tab->type() == JT_INDEX_SCAN &&
        select_limit < table->file->stats.records)
    {
      tab->position()->rows_fetched= select_limit;
      tab->position()->filter_effect= COND_FILTER_STALE_NO_CONST;
    }

    // Keep current (ordered) tab->quick()
    if (save_quick != tab->quick())
      delete save_quick;
  }
  else
  {
    // Restore original save_quick
    if (tab->quick() != save_quick)
    {
      delete tab->quick();
      tab->set_quick(save_quick);
    }
  }

  Opt_trace_object
    trace_change_index(trace, "index_order_summary");
  trace_change_index.add_utf8_table(tab->table_ref)
    .add("index_provides_order", can_skip_sorting)
    .add_alnum("order_direction", order_direction == 1 ? "asc" :
               ((order_direction == -1) ? "desc" :
                "undefined"));

  if (changed_key >= 0)
  {
    // switching to another index
    // Should be no pushed conditions at this point
    DBUG_ASSERT(!table->file->pushed_idx_cond);
    if (unlikely(trace->is_started()))
    {
      trace_change_index.add_utf8("index", table->key_info[changed_key].name);
      trace_change_index.add("plan_changed", !no_changes);
      if (!no_changes)
        trace_change_index.add_alnum("access_type", join_type_str[tab->type()]);
    }
  }
  else if (unlikely(trace->is_started()))
  {
    trace_change_index.add_utf8("index",
                                ref_key >= 0 ?
                                table->key_info[ref_key].name : "unknown");
    trace_change_index.add("plan_changed", false);
  }
  DBUG_RETURN(can_skip_sorting);
}

