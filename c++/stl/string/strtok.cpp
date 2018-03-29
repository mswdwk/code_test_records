#include<iostream>
#include<cstring>
#include<stdio.h>
using namespace std;
int main()
{
    char sentence[]="This is a sentence with 7 tokens";
    cout << "The string to be tokenized is:\n" << sentence << "\n\nThe tokens are:\n\n";
    char *tokenPtr=strtok(sentence," ");
	while(tokenPtr!=NULL){
        cout<<tokenPtr<<endl;
        tokenPtr=strtok(NULL," ");
    }
    cout << "After strtok,sentence=" << sentence<<endl;
    printf("tokenPtr = %p \n" , tokenPtr);
    if(tokenPtr)printf("*tokenPtr = %d\n" ,*tokenPtr);
    return 0;
}
