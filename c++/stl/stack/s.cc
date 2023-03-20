#include<stack>
#include <iostream>
using namespace std;

int main()
{
	stack<char*> s;
	cout<<"hello: size="<<s.size()<<",empty()="<<s.empty()<<endl;	
	cout<<"hello:"<<s.top()<<endl;	
	return 0;
}
