#include<stdio.h>
#include<iostream>
using namespace std;
int main()
{
	int a = 123;int b=345;
	if(!a && b++){
		printf("3 b=%d\n",b);
	} else {
		printf("4 b=%d\n",b);
	}
	if(!!a && ++b){
		printf("1 b=%d\n",b);
	} else{
		printf("2 b=%d\n",b);
	}
	return 0;
}
