#include<stdio.h>
#include<time.h>

void main(){
	unsigned int a = 0xFFFFFFFF;
	int b = -1;
	int r = ( a == b);
	if( a == b )
		printf("=\n");
	else
		printf("!=\n");
	int i = 0;
	for(i=0;i<1000;++i){
	 	//sleep(1);
		exit(0);
	}
	printf("r=%d,i=\n",r,i);
}
