#include <iostream>
#include <stack>
#include <queue>
#include <map>
#include <string>
#include <string.h>
#include <stdlib.h>

using namespace std;

const char* smart_repeat_str(const char* input) {
    stack<int> s_number;
    stack<char*> s_string;
    while ( NULL != input && *input !='\0') {
        // cout<<"input:"<<input<<endl;
        char tmp_digit[128]={0};
        int i = 0;
        while (isdigit(*input)) {
           tmp_digit[i++]=*input++;
        }
        tmp_digit[i]='\0';
        if(strlen(tmp_digit) > 0) {
            int num = atoi(tmp_digit);
            s_number.push(num);
            // cout<<"push number:"<<num<<",i="<<i<<endl;
        }
        if (*input == '[') {
            input++;
            char *ch = new char(1);
            *ch='\0';
            s_string.push(ch);
            // cout<<"push string: noop"<<endl;
        }

        // deal with char
        char ch_tmp[128];
        i = 0;
        while (input != NULL && input != '\0' && isalpha(*input)) {
            ch_tmp[i] = *input;
            input++;
            i++;
        }
        ch_tmp[i] = '\0';
        if (strlen(ch_tmp) > 0) {
            char* new_ch = NULL;
            int length = 0;
            length = strlen(ch_tmp);
            new_ch = new char[length+1];
            strcpy(new_ch,ch_tmp);
            new_ch[length]='\0';
            s_string.push(new_ch);
            // cout<<"push single string:"<<new_ch<<", stack number.size="<<s_number.size();
            // cout<<", s_string.size="<<s_string.size()<<endl;
        } 
        if (*input == ']') {
            // cout<<"caught: input="<<input<<", number.size="<<s_number.size();
            //     cout<<", s_string.size="<<s_string.size()<<endl;
            if( !s_number.empty()  && !s_string.empty() ){
                int top_int = s_number.top();
                s_number.pop();
                char* ch = s_string.top();
                s_string.pop();
                char* ch2 = NULL;
                if(!s_string.empty()){
                    ch2 = s_string.top();
                    s_string.pop();
                }
                
                int length = 0;
                if (ch2 != NULL)
                    length = top_int*(strlen(ch)+strlen(ch2));
                else
                    length = top_int*strlen(ch);
                char *ch_new = new char[length+1];
                memset(ch_new,0,length+1);
                for(int i=0;i<top_int;++i){
                    if(ch2 != NULL)
                        strcat(ch_new,ch2);
                    strcat(ch_new,ch);
                }
                ch_new[length] = '\0';
                // cout<<"s_string.size()= "<<s_string.size()<<endl;
                char *top = NULL;;
                char *merge_ch = ch_new;
                if(!s_string.empty()){
                    top = s_string.top();
                    // cout<<"before push merge string: get top str["<<top<<"]"<<endl;
                    length =strlen(ch_new)+strlen(top);
                    merge_ch = new char[length+1];
                    memset(merge_ch,0,length+1);
                    strcat(merge_ch,top);
                    strcat(merge_ch,ch_new);
                    delete ch_new;
                    s_string.pop();
                    delete top;
                }
                merge_ch[length]='\0';
                s_string.push(merge_ch);
                if(ch2 != NULL){
                    // cout<<"push merge string: "<<merge_ch<<",orgin str:["<<ch<<","<<ch2<<"],orgin str len:[";
                    // cout<<strlen(ch)<<","<<strlen(ch2)<<"],top_number:"<<top_int<<endl;
                    delete ch2;
                }else{
                    // ch2="";
                    // cout<<"push merge string,2: "<<merge_ch<<",orgin str:["<<ch<<","<<ch2<<"],orgin str len:[";
                    // cout<<strlen(ch)<<","<<strlen(ch2)<<"],top_number:"<<top_int<<endl;
                }
                delete ch;
            } else {
                cout<<"caught an error: input="<<input<<", number.size="<<s_number.size();
                cout<<", s_string.size="<<s_string.size()<<endl;
            }
            input++;
        }  
        i = 0;
    }
    if(!s_string.empty()){
        char *top = s_string.top();
        s_string.pop();
        return (const char*)top;
    }else{
        return "";
    }
}

struct test_case {
    const char*input_str;
    const char*expect_str;
} g_cases[] = {
        {"3[1[x2[y]]hello2[i]]","xyyhelloiixyyhelloiixyyhelloii"},
        {"2[3[yb]5[x]1[nihao2[i]]]","ybybybxxxxxnihaoiiybybybxxxxxnihaoii"},
        {"2[3[yb]5[x2[a1[j]]]1[nihao2[i]]]","ybybybxajajxajajxajajxajajxajajnihaoiiybybybxajajxajajxajajxajajxajajnihaoii"},
        {"12[a]","aaaaaaaaaaaa"},
        {"1[a2[b]3[c]4[d]5[e]6[f]7[g]8[h2[i]]]","abbcccddddeeeeeffffffggggggghiihiihiihiihiihiihiihii"},
        {"",""},
        {"1[2[3[4[1[1[1[a]]]]]]]","aaaaaaaaaaaaaaaaaaaaaaaa"}
};

void test_cases(){
    int success_count = 0;
    unsigned int i = 0;
    for( ;i< sizeof(g_cases)/sizeof(struct test_case);++i){
        const char*input_str = g_cases[i].input_str;
        const char*expect_str = g_cases[i].expect_str;
        cout<<"case "<<i<<" origin input str is : [len "<<strlen(input_str)<<"] "<<input_str<<endl;
        const char *repeat_str = smart_repeat_str(input_str);
        cout<<"after smart repeat, str is : [len "<<strlen(repeat_str)<<"] "<<repeat_str<<endl;
        if(0 != strcmp(expect_str,repeat_str)){
            cout<<"Test case failed!"<<"success test case count: "<<success_count<<",run "<<i<<" cases"<<endl;
            cout<<"Input string: "<<input_str<<endl;
            cout<<"Expect_str:"<<expect_str<<",actural str is: "<<repeat_str<<endl;
            break;
        }
        success_count++;
    }
    cout<<"\nEnd: all test case ok, success test case count: "<<success_count<<endl;
}

int main()
{
    test_cases();
    return 0;
}
