#include<stdio.h>
typedef int (*arr)[10];
//typedef int (*arr2[10]);
//typedef int *arr2[10];

int main()
{
	int array[100];
	arr a;	
//	arr2 a2;	
	a = (arr)array;
//	a2 = (arr2)array;

	printf("sizeof(a)=%d\n",sizeof(a));
	printf("%p a+1= %p\n",a,(a+1));
	//printf("%p a2+1= %p\n",a2,(a2+1));
int x=1,y=1,z=1;
y=y+z;
x=x+y;  //  y=2, x=3
printf("%d,",x<y?y:x); // 3 
printf("%d,",x<y?x++:y++); // 3 
printf("%d,%d\n",x,y); // 3,3
	return 0;
}
