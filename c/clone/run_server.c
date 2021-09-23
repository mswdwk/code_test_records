#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define STACK_SIZE      65536*1024*4
//static char*args[]={"mysqld","--defaults-file=/home/user/MySQL5.7_1/etc/my.cnf","--basedir=/home/user/MySQL5.7_1","--data-dir=/home/user/MySQL5.7_1/data","--character-set-server=utf8","--pid-file=/home/user/MySQL5.7_1/mysqld1.pid","--lc-messages-dir=/home/user/MySQL5.7_1/share/charsets",NULL};
static char*args[]={"mysqld","--defaults-file=/home/user/MySQL5.7_1/etc/my.cnf","--character-set-server=utf8","--pid-file=/home/user/MySQL5.7_1/mysqld1.pid","--lc-messages-dir=/home/user/MySQL5.7_1/share/charsets",NULL};

static char*args2[]={"ls","-l",NULL};
static char* mysqld_path="/home/user/MySQL5.7_1/bin/mysqld";
static int clone_flags=(CLONE_FS | SIGCHLD);
static int child_func(void *arg)
{
     printf("Child:Current Working Directory:%s\n",
                     get_current_dir_name());
     chdir("/opt");
	 //execl(mysqld_path,args);
	 execv(mysqld_path,args);
     printf("Child:Current Working Directory:%s\n",
                     get_current_dir_name());
     return 0;
}

int main(int argc, char *argv[])
{
     //Allocate stack for child task
     char *stack = malloc(STACK_SIZE);
     int status;
     printf("Parent:Current Working Directory:%s\n",
                     get_current_dir_name());
     if (!stack) {
             perror("Failed to allocate memory\n");
             exit(1);
     }

     if (clone(child_func, stack + STACK_SIZE, clone_flags, NULL) == -1) {
             perror("clone");
             printf("error:%s\n",strerror(errno));
             exit(1);
     }
     if (wait(&status) == -1) {
             perror("Wait");
             exit(1);
     }
     printf("Child exited with status:%d\t cwd:%s\n",
                     status, get_current_dir_name());
     return 0;
}
