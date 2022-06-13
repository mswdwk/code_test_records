#include <sys/time.h>
#include <sys/resource.h>
#include<iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

using namespace std;
#define DEFAT        "\033[m\n"
#define GREEN        "\033[0;32;32m"

void usage(){
	char*usage="-t <millisecond> <your_exefile>";
	cout<<usage<<endl;
}

void print_mem_usage(struct rusage ru){
	cout<<" maxrss "<<ru.ru_maxrss<<" ,shared mem:"<<ru.ru_ixrss<<
	" ,unshared mem: "<<ru.ru_idrss	<<" ,unshared stack mem: "<<ru.ru_isrss<<endl;
}

int main(int argc ,char*argv[])
{
	usage();
	int who;
	who = RUSAGE_CHILDREN;
	//who=RUSAGE_SELF;
	struct rusage ru;
	cout<<"argc="<<argc<<endl;	
	cout<<"argv[0]="<<argv[0]<<endl;	
	int sleep_us= 300*1000;
	int status=0,pid=0;
	char*cmd="ls";
	if(argc==4)
		cmd=argv[3];
	printf("fork前：PID:%d,cmd=%s\n",getpid(),cmd);

    pid=fork();//fork调用
    if(-1==pid){
        perror("fork error:");
        return -1;
    }
    else if(pid>0){//运行到这里面的是父进程
        while(  status== 0){
            printf("PID:%d,fork返回值:%d\n",getpid(),pid);
			getrusage(who, &ru);
			
			print_mem_usage(ru);
            usleep(sleep_us);
        }
        //等待子进程退出，回收僵尸进程（已退出的子进程）
        while(waitpid(pid,&status,0)<0){
            if(EINTR!=errno){
                return -1;
            }
			cout<<"child process exit "<<pid<<endl;
        }
        exit(0);//父进程退出
    }
    else if(0==pid){//运行到这里面的是子进程
        printf(GREEN"PID:%d,PPID:%d,fork返回值:%d"DEFAT,getpid(),getppid(),pid);
		system(cmd);
        exit(0);
        //子进程退出
    }	

	getrusage(who, &ru);
	print_mem_usage(ru);
	return 0;
}
