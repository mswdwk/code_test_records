#include<stdio.h>
typedef int (*arr)[10];
//typedef int (*arr2[10]);
//typedef int *arr2[10];

int main()
{
	int array[100];
	arr a;	
	arr2 a2;	
	a = (arr)array;
	a2 = (arr2)array;

	printf("sizeof(a)=%d\n",sizeof(a));
	printf("%p a+1= %p\n",a,(a+1));
	//printf("%p a2+1= %p\n",a2,(a2+1));
	return 0;
}
