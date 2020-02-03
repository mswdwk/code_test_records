#include<iostream>
using namespace std;

class father{
	public:
		int fa;
	father(int a);
};
father::father(int a){
	fa = a;
}
class son : public father{
	public:
		int sb;
		son(int mm):father(mm){};
};

int main()
{
	son s=son(123);
	cout<<"\n"<<s.fa<<endl;

	return 0;
}
