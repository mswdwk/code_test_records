#include<stdio.h>
#include<iostream>
using namespace std;

bool is_gbk(char input[]){
	unsigned char*p = (unsigned char*)input;	
	int i = 0;
	int is_hanzi_flag = 0;
	int last_is_big_than_127 =0 ;
	for(; p != NULL;i++,p++){
		if(p[i]<127){
			continue;
		}
		else if(last_is_big_than_127) {
			last_is_big_than_127 = 0;
			return true;
		}else{
			last_is_big_than_127 = 1;
			is_hanzi_flag=1;
		}
	}
	return false;
}

bool is_utf8(char input[]){
	unsigned char*p = (unsigned char*)input;
	return true;
}
bool is_utf16(char input[]){
	unsigned char*p = (unsigned char*)input;
	return true;
}
int main()
{
	cout<<"enter input byte string"<<endl;	
   	char input_str[1024];
	cin>>input_str;	 		
	cout<<"\ninpurt str is: "<<input_str<<endl;
	if(is_gbk(input_str)){
		cout<<"this is GBK ENCODE!"<<endl;
	}
	else if(is_utf8(input_str)){
		cout<<"this is UTF-8 ENCODE!"<<endl;
	}
	else if(is_utf16(input_str)){
		cout<<"this is UTF-16 ENCODE!"<<endl;
	}else {
		cout<<"this is OTHER ENCODE!"<<endl;
	}
	
	return 0;
}

