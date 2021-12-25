#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static int signal_count = 0;
void handle_sigal(int sig){
	signal_count++;
	if(signal_count<10)
		printf("pid %d caught signal %d count %d\n",getpid(),sig,signal_count );	
	return;
}

int main(int argc,char*argv[]) {
	int i = 0;
	int max_fork_num = 1000;
	signal(SIGPIPE,handle_sigal);
	signal(SIGSEGV,handle_sigal);
	signal(SIGABRT,handle_sigal);
	if(argc > 1 ){
		printf("max_fork_num=%d\n",max_fork_num);
		max_fork_num = atoi(argv[1]);
	}
	for(i=0;i<max_fork_num;++i){
			pid_t pid1;
			pid1 = fork();
			if(pid1 == -1){
				printf("\nFail to fork,count=%d, errno %d \n\n",i,errno);
				exit(0);
			}
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
