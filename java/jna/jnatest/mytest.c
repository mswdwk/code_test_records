#include<stdio.h>
#include<string.h>

//EXPORT
char* stripconst(char*origin)
{
	if(NULL == origin ){
		return NULL ;
	}	
	
	return origin + 2;
}
