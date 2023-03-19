#include <iostream>
#include <stack>
#include <queue>
#include <map>
#include <string>
#include <string.h>
#include <stdlib.h>

using namespace std;

stack<int> s_number;
stack<char*> s_string;

// 2[3[yb]5[x]2[1[nihao]5[i]]]
char* repeat_str(const char* input) {
    while ( NULL != input && *input !='\0') {
        cout<<"input:"<<input<<endl;
        char tmp_digit[128]={0};
        int i = 0;
        while (isdigit(*input)) {
           tmp_digit[i++]=*input++;
        }
        tmp_digit[i]='\0';
        if(strlen(tmp_digit) > 0) {
            int num = atoi(tmp_digit);
            s_number.push(num);
            cout<<"push number:"<<num<<",i="<<i<<endl;
        }
        // cout<<"input step 2:"<<input<<endl;
        if (*input == '[') {
            input++;
            char *ch = new char(1);
            *ch='\0';
            s_string.push(ch);
            // s_number.push(1);
            cout<<"push string: noop"<<endl;
        }

        // deal with char
        char ch_tmp[128];
        i = 0;
        while (input != NULL && input != '\0' && isalpha(*input)) {
            ch_tmp[i] = *input;
            input++;
            i++;
        }
        // cout<<"input step 3:"<<input<<", i="<<i<<endl;
        ch_tmp[i] = '\0';
        if (strlen(ch_tmp) > 0) {
            char* new_ch = NULL;
            char* top_ch = NULL ;//s_string.top();
            // s_string.pop();
            int length = 0;
            if ( top_ch != NULL) {
                length = strlen(ch_tmp)+strlen(top_ch);
                new_ch = new char(length+1);
                strcpy(new_ch, top_ch);
                strcat(new_ch, ch_tmp);
                delete top_ch;
            } else {
                length = strlen(ch_tmp);
                new_ch = new char(length+1);
                strcpy(new_ch,ch_tmp);
            }
            new_ch[length]='\0';

            s_string.push(new_ch);

            cout<<"push string:"<<new_ch<<", stack number.size="<<s_number.size();
                cout<<", s_string.size="<<s_string.size()<<endl;
        } 
        // cout<<"input step 4:"<<input<<endl;
        // 2[3[ab]5[cd[7[x]2[p]]]
        if (*input == ']') {
            cout<<"caught: input="<<input<<", number.size="<<s_number.size();
                cout<<", s_string.size="<<s_string.size()<<endl;
            if( !s_number.empty()  && !s_string.empty() ){
                int top_int = s_number.top();
                s_number.pop();
                char* ch = s_string.top();
                s_string.pop();
                char* ch2 = s_string.top();
                s_string.pop();
                int length = top_int*(strlen(ch)+strlen(ch2));
                char *ch_new = new char(length+1);
                memset(ch_new,0,length+1);
                for(int i=0;i<top_int;++i){
                    strcat(ch_new,ch2);
                    strcat(ch_new,ch);
                }
                ch_new[length] = '\0';
                delete ch;
                delete ch2;
                char *top = NULL;
                char *merge_ch = ch_new;
                top=s_string.top();
                if((top) != NULL){
                    length =strlen(ch_new)+strlen(top);
                    merge_ch = new char(length+1);
                    strcat(merge_ch,ch_new);
                    strcat(merge_ch,top);
                    s_string.pop();
                    delete top;
                }
                merge_ch[length]='\0';
                s_string.push(merge_ch);
                cout<<"push merge string: "<<ch_new<<",orgin str:["<<ch<<","<<ch2<<"],orgin str len:[";
                cout<<strlen(ch)<<","<<strlen(ch2)<<"],top_number:"<<top_int<<endl;
                
            } else {
                cout<<"caught an error: input="<<input<<", number.size="<<s_number.size();
                cout<<", s_string.size="<<s_string.size()<<endl;
            }
            input++;
        }  
        i = 0;
    }
}

int main()
{
    // const char* input = "2[3[yb]5[x]1[nihao2[i]]]";
    const char* input = "3[1[x2[y]]hello2[i]]";
    
    repeat_str(input);
    cout<<"input str is :"<<input<<endl;
    for (int i = 0; !s_number.empty();i++) {
        int top = s_number.top();
        cout << "number stack "<<i<<" : "<<top << endl;
        s_number.pop();
    }

    for (int i = 0; !s_string.empty(); i++) {
        const char *top = s_string.top();
        cout << "string stack " << i << " : " << top << endl;
        s_string.pop();
    }

    std::cout << "Hello World!\n";
}

