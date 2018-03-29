#include <iostream>
#include <stack>
#include <queue>
#include <map>
#include <string>
#include <string.h>

using namespace std;
//typedef DT int;
typedef int DT;


char *reverse_sentence(char*str)
{
	stack <char*>s;
	int str_len = strlen(str);
	char *tmp=str;
	while(*tmp==' ')tmp++;
	char *word_start=tmp;
	while(tmp){
		if(*tmp==' ') {
			*tmp = 0;
			//string a(word_start);
			//cout<<a.c_str()<<endl;
			cout<<word_start<<endl;
			s.push(word_start);
			word_start = tmp+1;
		}else if(*tmp==0){
			cout<<word_start<<endl;
			s.push(word_start);
			break;
		}
		
		tmp++;
	}
	char *tmp_str = new char(str_len+1);
	int len = 0;
	for(;!s.empty();){
		char*a=s.top();
		strcpy(tmp_str+len,a);
		len += strlen(a);
		tmp_str[len]=' ';
		len++;
		cout<<a<<endl;
		s.pop();
	}
	//cout<<tmp_str<<endl;
	tmp_str[len] = 0;
	strcpy(str,tmp_str);
	delete tmp_str;
	return str;
}

int main()
{
	#if 0
	stack <DT> s;
	int i;
	for(i=0;i<10;i++){
		s.push(i);
	}
	for(;!s.empty();){
		cout<<s.top()<<endl;
		s.pop();
	}
	#else
	//char *sentence=" i am human";
	char sentence[] =" i am human";
	cout<<"before reverse:"<<sentence<<endl;
	reverse_sentence(sentence);
	cout<<"after reverse:"<<sentence<<endl;
	
	#endif
	
	return 0;
}
