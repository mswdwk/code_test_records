
opt_explain_json.cc (sql) line 93 : static const char K_USING_FILESORT[]=               "using_filesort";
opt_explain_json_namespace::simple_sort_ctx::format_body in opt_explain_json.cc (sql) :     obj->add(K_USING_FILESORT, using_filesort);
opt_explain_json_namespace::sort_ctx::format_body in opt_explain_json.cc (sql) :       obj->add(K_USING_FILESORT, using_filesort);
opt_explain_json_namespace::union_result_ctx::format in opt_explain_json.cc (sql) :   order_by.add(K_USING_FILESORT, !order_by_subqueries.is_empty());

Explain_table::shallow_explain in opt_explain.cc (sql) :       flags.set(ESC_ORDER_BY, ESP_USING_FILESORT);
JOIN::add_sorting_to_table in sql_select.cc (sql) :   explain_flags.set(sort_order->src, ESP_USING_FILESORT);
