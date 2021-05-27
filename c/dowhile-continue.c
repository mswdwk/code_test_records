#include<stdio.h>
#include<time.h>
void main()
{
	int i = 0;
	do {
		if(i%3 == 0)	{
			printf("i=%d\n",i);
			continue;
		}
		i++;
	}while(i<10);

}
