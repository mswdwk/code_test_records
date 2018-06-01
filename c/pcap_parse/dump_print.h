#ifndef _DUMP_PRINT_H
#define _DUMP_PRINT_H
#include <stdio.h>

void dump_print(char* comment,int buf_len, void *buf);
void dump_fprintf(FILE*fp,char* comment,int buf_len, void *buf);

#define pdg(fmt,arg...) printf("[%s %d]"fmt,__func__,__LINE__,##arg)
#endif
