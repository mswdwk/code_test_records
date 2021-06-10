#include<stdio.h>

typedef struct _A{
char a;
int b[2];
double c;
char d;
int e[2];
char f;
}A;
#define OFFSET(type,field) sizeof( ( (type*)0 )->field)
#define print(type,field) printf("type %s field %s offset %u\n",#type,#field,OFFSET(type,field))
#define printaddr(instance,field) printf("instance %s field %s addr %p\n",#instance,#field,&instance.field)
void main(){
	//printf("a=%d\n",offset(A,a));
	//printf("a=%d\n",offset(A,a));
	print(A,a);
	print(A,b);
	print(A,c);
	print(A,d);
	print(A,e);
	print(A,f);

	A a;
	
	printaddr(a,a);
	printaddr(a,b);
	printaddr(a,c);
	printaddr(a,d);
	printaddr(a,e);
	printaddr(a,f);
} 

