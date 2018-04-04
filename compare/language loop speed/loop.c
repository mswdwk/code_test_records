#include<stdio.h>
#define loop 10000
#define alooptime 10000
int main()
{
    int i,j;
    for(i=0;i<loop;i++)
        for(j=0;j<alooptime;j++){}
    
    printf("loop %d*%d times\n",loop,alooptime);
    return 0;
}
