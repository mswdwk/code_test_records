#include<iostream>
#include<setjmp.h>
#include<signal.h>
using namespace std;
jmp_buf env;

void do_sig(int sig)
{
	cout << "caught signal " << sig << endl;
	return siglongjmp(env, 2);
}

int main()
{
	char *p = NULL;
	signal(SIGSEGV, do_sig);
	if (sigsetjmp(env, 0) == 0)	//siglongjmp会在这里返回
	{
		cout << "enter logic 1" << endl;
		try {
			cout << "this is " << *p << endl;
		}
		catch( ...) {
			cout << "catch  an error" << endl;
		}
	} else {
		cout << "catch exception xxx" << endl;
	}
	return 0;
}
