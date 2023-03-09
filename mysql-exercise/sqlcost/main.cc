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
#include <mysql.h>
#include <sql/sql_class.h>
#include <sql/sql_lex.h>
#include <sql/sql_parse.h>
#include <my_inttypes.h>
// #include <my_md5_size.h>

#ifndef _WIN32
#include <sys/resource.h>
#include <sys/time.h>
#endif

int init_server_components();
extern const char *my_progname;
extern PSI_mutex_service_t *psi_mutex_service;

int main(int argc, char *argv[])
{
	my_progname = argv[0]; 
	std::string my_name(my_progname);
    char *argv1[] = {const_cast<char *>(my_name.c_str()),
                                    const_cast<char *>("--secure-file-priv=NULL"),
                                    const_cast<char *>("--log_syslog=0"),
                                    const_cast<char *>("--explicit_defaults_for_timestamp"),
                                    const_cast<char *>("--datadir=./" ),
                                    const_cast<char *>("--lc-messages-dir=/home/user/MySQL-8025/share"),
                                    nullptr};
    set_remaining_args(6, argv1);

	system_charset_info = &my_charset_utf8mb4_general_ci;
	mysql_mutex_init(PSI_NOT_INSTRUMENTED, &LOCK_open, MY_MUTEX_INIT_FAST);
	if ( my_init()){
		printf("my_init failed\n");
		return 1;
	}
	printf("my_init ok\n");
	if (sys_var_init()){
		printf("sys_var_init failed\n");
		return 1;
	}
	printf("sys_var_init ok\n");
	if (init_common_variables()){
		printf("init_common_variables failed\n");
		return 1;
	}
	printf("init_common_variables ok\n");

	if(item_create_init()){
		printf("item_create_init failed\n");
		return 1;
	}
	printf("item_create_init ok\n");
	if (mysql_server_init(0,nullptr,nullptr)){
		printf("mysql_server_init failed\n");
		return 1;
	}
	printf("mysql_server_init ok\n");

	if(init_server_components()){
		printf("init_server_components failed\n");
		return 1;
	}
	printf("init_server_components ok, max_digest_length %lu\n",max_digest_length);
	max_digest_length= 1024*1024-1;

#ifndef _WIN32
	rlimit core_limit;
	core_limit.rlim_cur = 0;
	core_limit.rlim_max = 0;
	setrlimit(RLIMIT_CORE, &core_limit);
#endif
	// init_signal_handling();

	THD *thd;
	thd = new THD(false);
	thd->set_new_thread_id();
	thd->thread_stack = (char *) thd;
	thd->store_globals();
	lex_start(thd);

	const char *query_text = "select a from testdb.t1 where c1 = 123 or c2>7 and c3 like '%123' ";
	uint query_length = strlen(query_text);
	Object_creation_ctx *ctx = nullptr;
	bool rc;

	Parser_state parser_state;
	if (parser_state.init(thd, query_text, query_length))
	{
		printf("error\n");
	}

	parser_state.m_input.m_compute_digest = true;
	thd->m_digest = &thd->m_digest_state;
	thd->m_digest->reset(thd->m_token_array, max_digest_length);

	rc = parse_sql(thd, &parser_state, ctx);
	if (!rc)
	{
		unsigned char hash_buf[DIGEST_HASH_SIZE+1];
		String digest_text;
		const sql_digest_storage *digest = &thd->m_digest->m_digest_storage;
		printf("Parse Sql OK, %p\n",thd->m_digest);
		compute_digest_hash(digest, hash_buf);
		compute_digest_text(digest, &digest_text);
		printf("Sql Hash: %s\n",hash_buf);
		printf("Sql Normalized: [ %s ]\n",digest_text.c_ptr());
	}

	mysql_mutex_destroy(&LOCK_open);
  	my_end(0);
	return 0;
}