#include <iostream>
#include <string>
using namespace std;

//string转char*
void s_c()
{
    string str="hello word";
    const char* cstr = str.c_str();
    cout<<cstr<<endl;
}

//char* 转string
void c_s()
{
    char* c = "hello word";
    string s(c);
    cout<<s<<endl;
}


//string 赋值操作
void string_some()
{
    //创建一个空的字符串
    string str="hello word";
	cout<<"str1 = "<<str<<endl;
    //使用str 初始化str2
    string str2(str);
	cout<<"str2 = "<<str2<<endl;
    //使用char*初始化string；
    const char* c= "hello word";
    string str3(c);
    cout<<"str3 = "<<str3<<endl;

    //字符赋值给当前字符串
    char s_si[]="2";
    string str4(s_si);
    cout<<"str4 = "<<str4<<endl;

    //使用n个字符初始化c
    int n_i = 5;
    char n_c[] = "s";
    string str5(n_i,*n_c);
    cout<<"str5 = "<<str5<<endl;

    //将s从start开始n个字符赋值给字符串
    string s ="ABCDEFG";
    int start = 2;
    int n =6;
    string str6(s,start,n);
    cout<<"str6 = "<<str6<<endl;

    //string 通过[],at方式取字符 at方式取字符如果越界可以被异常捕获
    string str7="1234567";
    cout<<"str[1] = "<<str[1]<<endl;
    cout<<"str.at(1) = "<<str.at(1)<<endl;
}

//string 拼接操作
void string_pin()
{
    //+=拼接字符串
    string str = "12345";
    string str2 = "67890";
    str += str2;
    cout<<"str+str2 ="<<str<<endl;

    //+=拼接char*
    char * c="1234";
    string str3="5678";
    str3+=c;
    cout<<"str3+c="<<str3<<endl;

    //+=拼接char字符
    char c_s[]="cd";
    string str4 = "ab";
    str4+=c_s;
    cout<<"str4="<<str4<<endl;

    //连接string到当前字符串尾部
    string str5="123";
    str4.append(str5);
    cout<<"str4="<<str4<<endl;

    //连接char*到当前字符串尾部
    const char* c_s1="cdefg";
    str5.append(c_s1);
    cout<<"str5="<<str5<<endl;

    //连接char 到当前字符串尾部
    const char c_s2[]="0987";
    string str6="5432";
    str6.append(c_s2);
    cout<<"str6"<<str6<<endl;

    //吧str7中p开始的n个字符串链接到当前字符串尾部
    string str7 = "abcdef";
    int p=0;int n =2;
    str4.append(str7,p,n);
    cout<<"str4="<<str4<<endl;

    //在当前字符串结尾添加n个字符串a
    char c_8[] = "1234";
    int n_8 = 2;
    string str8 = "asd";
    str8.append(n_8,*c_8);
    cout<<"str8="<<str8<<endl;
}


//string 查找和替换
void string_find()
{
     //find 从n开始查找s所在的位置
     string str = "1234567890acsdefgh";
     string s = "6";
     int n = 4;
     int f_s = str.find(s,n);
     cout<<"f_s="<<f_s<<endl;

     //char*类型同上
     const char* s_2 = "7";
     int n_2 =2;
     int f_s1=str.find(s_2,n_2);
     cout<<"f_s1="<<f_s1<<endl;

     //
     string s_3="e";
     int n_3=13;
     int n_3_2 = 8;
     int f_s2 = str.find(s_2,8,n_3_2);
     cout<<"f_s2="<<f_s2<<endl;

     //rfind 从后向前查
     string s_r = "3";
     int f_r = str.rfind(s_r,n);
     cout<<"f_r="<<f_r<<endl;
}


//string比较操作
//compare 大于时返回1 小于时返回-1，等于时返回0，比较时参考字典顺序
void str_compare()
{
    //string 比较
    string s="1";
    string c="1";
    int comp = s.compare(c);
    cout<<"comp="<<comp<<endl;

    //char* 比较
    const char* c_c="2";
    int c_s_comp = s.compare(c_c);
    cout<<"c_s_comp="<<c_s_comp<<endl;

    //copy子串
    string str ="1234567890";
    int sr_c = 5;
    int po_c = 3;
    string so_str = str.substr(sr_c,po_c);
    cout<<"so_str="<<so_str<<endl;
}


//string 插入和删除
void de_str()
{
    //将字符串str_num插入到str第五个字符以后
    string str = "abcdefgh";
    string str_num = "123456789";
    int num = 5;
    string s = str.insert(num,str_num);
    cout<<"str="<<s<<endl;

    //删除从第pos位置开始的pos_n个元素
    int pos = 3;
    int pos_n = 2;
    s.erase(pos,pos_n);
    cout<<"s.erase(3,2) = "<<s<<endl;
}

int main()
{
    s_c();
    c_s();
    string_some();
    string_pin();
    string_find();
    str_compare();
    de_str();
    cout << ".............." << endl;
    return 0;
}
