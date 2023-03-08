#include<stdio.h>
// #include "mysql/service_parser.h"
//#include <mysql/services.h>
#include <zconf.h>
#include <zlib.h> 
#include <stddef.h>
#include <my_inttypes.h>
#include <sql/current_thd.h>
#include <mysql/plugin.h>
#include <sql/set_var.h>
#include <sql/mysqld.h>
#include <sql/sql_class.h>
#include <sql/sql_lex.h>
#include <sql/sql_parse.h>
// #include <my_md5_size.h>

int main()
{
	THD *thd = new THD(false);
    const char *query_text = "select 1";
    uint query_length = strlen(query_text);
    Object_creation_ctx *ctx = nullptr;
    bool rc;

    Parser_state parser_state;
    if (parser_state.init(thd, query_text, query_length))
    {
		printf("error\n");
    }

    parser_state.m_input.m_compute_digest= true;

    rc= parse_sql(thd, &parser_state, ctx);
    if (! rc)
    {
      // unsigned char md5[MD5_HASH_SIZE];
      char digest_text[1024];
      bool truncated;
      const sql_digest_storage *digest= & thd->m_digest->m_digest_storage;

      // compute_digest_md5(digest, & md5[0]);
      // compute_digest_text(digest, & digest_text[0], sizeof(digest_text), &truncated);
	  printf("parse sql OK\n");
    }
	
	return 0;
}