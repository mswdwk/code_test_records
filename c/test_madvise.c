#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
// 成功执行MADV_DONTNEED操作之后，访问指定区域的语义将发生变化
// https://www.jianshu.com/p/965b1ed71ae4

#define NUM 2
int PAGE_SIZE = 0;

void demo() {
    PAGE_SIZE = getpagesize(); 
	printf("页表大小为:%d bytes\n",PAGE_SIZE);
	int count = 0;	
	for(int i=0; i < NUM*1024; i++) {
		for(int j=0; j<1024/16; j++) {
			unsigned int alloc_size= PAGE_SIZE*32;
			char *bp =  (char *)malloc(alloc_size);
			count++;	
			if( NULL != bp){ 
				memset(bp,0,alloc_size);
			} else {
				printf("can not alloc memory,count=%d \n",count);
				system("free -m|grep Mem");	
				return;
			}

			//madvise(bp,PAGE_SIZE, MADV_DONTNEED);
		}
		system("free -m|grep Mem");	
	}
}

int main() {
	demo();
	system("pause");	
    return 0;
}

