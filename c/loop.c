#include<stdio.h>
#include<time.h>
void main()
{
	int count = 0;
	int state = 1;
	for(;;){
		switch(state){
			case 1:{
				sleep(1);
				printf(" 1 sleep count %d\n",count++);
				break;
				break;
			}	
			defalut:{
				sleep(1);
				printf(" 2 sleep count %d\n",count++);
				//break;
			}
		}
	}

}
