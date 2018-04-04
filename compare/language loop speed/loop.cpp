#include<iostream>
using namespace std;
#define loop 10000
#define alooptime 10000
int main()
{
    int i,j;
    for(i=0;i<loop;i++)
        for(j=0;j<alooptime;j++){}
    
    cout<<"loop "<<loop<<"*"<<alooptime<<" times"<<endl;
    return 0;
}
