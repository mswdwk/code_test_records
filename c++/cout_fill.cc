#include<iostream>
#include<stdio.h>
#include<netinet/in.h>

using namespace std;
class A{
	public: 
	const static int a=100;
	A(){
		cout<<"construct A"<<endl;
	}
	~A(){
		cout<<"deconstruct A"<<endl;
	}
};
int main(){
	cout.fill('-');
	cout.width(8);
	//cout.fill('#');
	//cout<<"12"<<endl;
	cerr<<"this4";
	cout<<A::a<<endl;
	A a;
	
	cout<<"this obj:"<<a.a<<endl;
	#if 0
	int aa = 0x80000000;
	int sum=0;
	long long l ;
	int b = (sum-aa)&0xffffffff;
	l = b;
	cout<<"this is a:"<<aa<<endl;
	cout<<"this is b:"<<b<<endl;
	printf("\n%0#x\n",b);
	printf("\nl=%0#x\n",l);
	{
	int a= 0x1234; unsigned int b=htons(a); unsigned char*ch =(unsigned char*) &b; printf("this is %02x %02x\n",ch[0],ch[1]);
	}
#endif 
getchar();
	return 0;
}

