#include "dump_print.h"

void dump_print(char* comment,int buf_len, void *buf)
{
    int i;
	if(!buf)return;
    if(comment)printf("%s length %d\n",comment, buf_len);
    for (i = 0; i < buf_len; i++) {
        if ((i & 0xf) == 0)
            printf( "\n0x%08X  ",i);
        printf( "%02X%s", (((unsigned char*)buf)[i]&0xff),(i+1)&0x3 ? "":" ");
    }
    printf( "\n");
}

