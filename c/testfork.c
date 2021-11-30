#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char*argv[]) {
	int i = 0;
	int max_fork_num = 1000;

	if(argc > 1 ){
		max_fork_num = atoi(argv[1]);
		printf("max_fork_num=%d\n",max_fork_num);
	}
	for(i=0;i<max_fork_num;++i){
			pid_t pid1;
			pid1 = fork();
			if(pid1 == -1)
				printf("Fail to fork,count=%d, errno %d err: %s\n",i,errno,strerror(errno));
			else if(pid1 == 0){
				printf("CHILD Process Pid: %d\tParent Process Pid: %d\n", getpid(), getppid());
				sleep(30);
				return 0;
			}
			else 
			   printf("Parent Process Pid: %d, count=%d\n", getpid(),i);
		
			printf("END: Process Pid: %d i=%d\n", getpid() ,i);
	}

    return 0;
}
