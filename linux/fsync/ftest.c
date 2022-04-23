#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


#define PAGESIZE 8192*2*2

void fil_buf(char*buf,char init,int count){
	int i =0;
	if(buf){
		for(i =0;i < count;++i)
			buf[i]=init;
		buf[i]=0;
	}
	
}

int main(int argc,char*argv[]){
	const char* filename= "a.txt";
	int loop_count = 1000;
	//int fd = open(filename,O_APPEND|O_CREAT|O_RDWR);
	int fd = open(filename,O_CREAT|O_RDWR);
	if (fd == -1)
		return 0; 
	int  close_sync = 0;
	if(argc>1)
		close_sync=1;
	printf("open file %s success, fd %lu close_sync=%d \n",filename,fd,close_sync);
	
	char init[PAGESIZE +1];
	fil_buf(init,'a',PAGESIZE );
	char buf[PAGESIZE +100] = {0};

	int data_len = 0;	
	int ret1, ret2;
	int i = 0;
	for(i =0 ;i < loop_count; ++i){
		data_len = sprintf(buf,"!---------- this is line %d ! ----------!%s\n",i,init);
		ret1 = write(fd,buf,data_len);
		if(!close_sync)
			ret2 = fsync(fd);
	}

	printf(" ret1 = %d ret2 = %d\n",ret1,ret2);	


	return 0;
}
