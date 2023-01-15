#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
// 成功执行MADV_DONTNEED操作之后，访问指定区域的语义将发生变化
// https://www.jianshu.com/p/965b1ed71ae4

#define NUM 500
int PAGE_SIZE = 0;

void demo() {
    PAGE_SIZE = getpagesize(); 
	printf("页表大小为:%d bytes\n",PAGE_SIZE);
	
	for(int i=0; i < NUM*1024; i++) {
		for(int j=0; j<NUM; j++) {
			char *bp =  (char *)malloc(PAGE_SIZE);
			madvise(bp,PAGE_SIZE, MADV_DONTNEED);
		}
		system("free -m|grep Mem");	
	}
}

int main() {
	demo();
    return 0;
}

