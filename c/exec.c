#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
int main(){
	char *argv[]={"/usr/bin/pwd"};
	int ret = execv("/usr/bin/pwd",argv);
	printf(" ret = %d\n",ret);	


	return 0;
}
