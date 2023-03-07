#include<stdio.h>
// #include "mysql/service_parser.h"
#include <mysql.h>
//#include <mysql/services.h>
#include <mysql/plugin.h>
#include <sql/set_var.h>
#include <sql/mysqld.h>

int main()
{
	// init_common_variables();
	printf("mysql_parser_service is %p\n",mysql_parser_service);
	// std::string my_name(my_progname);
	// char *argv[] = {const_cast<char *>(my_name.c_str()),
	// 				const_cast<char *>("--secure-file-priv=NULL"),
	// 				const_cast<char *>("--log_syslog=0"),
	// 				const_cast<char *>("--explicit_defaults_for_timestamp"),
	// 				const_cast<char *>("--datadir=" DATA_DIR),
	// 				const_cast<char *>("--lc-messages-dir=" ERRMSG_DIR),
	// 				nullptr};
	// set_remaining_args(6, argv);
	system_charset_info = &my_charset_utf8mb4_general_ci;

	// mysql_mutex_init(PSI_NOT_INSTRUMENTED, &LOCK_plugin, MY_MUTEX_INIT_FAST);
	sys_var_init();
	init_common_variables();

	return 0;
}