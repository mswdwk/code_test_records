#include<stdio.h>
// #include "mysql/service_parser.h"
#include <mysql.h>
//#include <mysql/services.h>
#include <mysql/plugin.h>


// extern  struct mysql_parser_service_st * mysql_parser_service;
extern int init_common_variables();
extern 
int main()
{
	init_common_variables();
	// printf("p is %p\n",mysql_parser_service);
	return 0;
}