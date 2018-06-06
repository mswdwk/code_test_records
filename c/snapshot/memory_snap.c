/*  
*   Record progress memory snapshot into * * file,the coredump file.
*   
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int main()
{
	char abc[12]={0};
	pid_t pid = fork();
	if(pid<0)
		exit(1);
	else if (pid == 0){
		printf("son process %zu!\n",getpid());
		raise(SIGABRT);
	}
	else{
		printf("father process %zu!\n",getpid());
		raise(SIGABRT);
	}
	return 0;
}
