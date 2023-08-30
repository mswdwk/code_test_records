#include<iostream>

using namespace std;

class A {
	public:
	void call_del(){
		del();
	}
	void del(){
		cout<<"class A,delete"<<endl;
	};
//	A();

};

class B :public A {
	public :
	void del(){
		cout<<"class B,delete"<<endl;
	}

};

int main()
{
	cout<<"hello"<<endl;
	A *a;
	B b;
	b.call_del();		
	b.del();
	a = (A*)&b;
	a->call_del();
	a->del();
	B  *bp;
	bp = (B*)a;
	bp->call_del();
	bp->del();
}
