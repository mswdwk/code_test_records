/*
 test Macro definition
 */
#include<stdio.h>
#define f(a,b) a##b
#define g(a) #a
#define h(a) g(a)

int main()
{
#define M N+1
#define N 1
	int i = M;
	printf("i = %d\n",i);
	printf("%s\n",h(f(1,2)));//输出12
	printf("%s\n",g(f(1,2)));//输出f(1,2)
	return 0;
}
