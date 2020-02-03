#include<string>
#include<iostream>
using namespace std;

class Test{
	public:
		string str1;
		int a;
		Test(char*ch){str1=ch;}
		void test();
		void deal(string str);
};

void Test::test()
{
	cout<<"str1="<<str1<<endl;
}

void Test::deal(string str){
	cout<<"str="<<str<<endl;
}
int main()
{
	Test t=Test((char*)"123");
	t.test();
	string abc=(char*)"abc";
	cout<<"abc="<<(char*)abc.c_str()<<endl;
	return 0;
}
