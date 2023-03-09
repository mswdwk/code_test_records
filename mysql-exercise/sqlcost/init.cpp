#include <stdio.h>
// #include "mysql/service_parser.h"
// #include <mysql/services.h>
#include <zconf.h>
#include <zlib.h>
#include <stddef.h>
// #include <sql/current_thd.h>
// #include <mysql/plugin.h>
#include <sql/set_var.h>
#include <sql/mysqld.h>
#include <sql/sql_class.h>
#include <sql/sql_lex.h>
#include <sql/sql_parse.h>
#include <my_inttypes.h>
#include <mysql.h>

extern void partitioning_init();
extern bool table_def_init(void);
extern int my_timer_initialize(void);
extern void init_slave_list() ;
extern bool transaction_cache_init() ;
extern int delegates_init();
extern  bool hostname_cache_init(uint size) ;
// extern bool MDL_context_backup_manager::init();

int init_server_components() {
  // DBUG_TRACE;
  /*
    We need to call each of these following functions to ensure that
    all things are initialized so that unireg_abort() doesn't fail
  */
  mdl_init();
  partitioning_init();
  if (table_def_init() | hostname_cache_init(host_cache_size))
    printf("123\n");

  randominit(&sql_rand, (ulong)server_start_time, (ulong)server_start_time / 2);

  /// setup_error_log();  // opens the log if needed

  enter_cond_hook = thd_enter_cond;
  exit_cond_hook = thd_exit_cond;
  enter_stage_hook = thd_enter_stage;
  set_waiting_for_disk_space_hook = thd_set_waiting_for_disk_space;
  // is_killed_hook = thd_killed;

  if (transaction_cache_init()) {
     printf("12\n");
  }

  // if (MDL_context_backup_manager::init()) {
  //   printf("1\n");

  // }


  if (delegates_init()) ;

  udf_init_globals();
  /*
    Needs to be done before dd::init() which runs DDL commands (for real)
    during instance initialization.
  */
  init_sql_command_flags();

    locked_in_memory = false;
    return 0;
}
