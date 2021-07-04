/*
排列搜索
120分钟
C C++ Java C#
题目详情
设数组a包含n个元素恰好是0..n - 1的一个排列，给定b[0],b[1],b[2],b[3]问有多少个0..n-1的排列a，满足(a[a[b[0]]]*b[0]+a[a[b[1]]]*b[1]+a[a[b[2]]]*b[2]+a[a[b[3]]]*b[3])%n==k  ？

输入包含5个参数：N,K,B0,B1,B2,B3，其中 4<= N<12, 0 <= K,B0,B1,B2,B3 < N。
*/

//#include "stdafx.h"
#include<stdio.h>
#define n 12
void perm(int);
void swap(int, int);
void countnum();
int count=0;
int N;int K;int B0;int B1;int B2;int B3;
int a[n];
int howmany (int Nh,int Kh,int B0h,int B1h,int B2h,int B3h)
{   N=Nh;K=Kh;B0=B0h;B1=B1h;B2=B2h;B3=B3h;
    int offset,i; 
    for( i=0;i<N;i++)a[i]=i;
    perm(0);
    return count;
}
//start 提示：自动阅卷起始唯一标识，请勿删除或增加。
int main()
{    
    printf("%d\n",howmany(8,6,1,3,2,4));
}
void perm(int offset)
{
    int i, temp;
    if(offset == N-1)
    {
       // print();
        countnum();
        return;
    }
    for(i = offset; i < N; i++)
    {
        swap(i, offset);
        perm(offset + 1);
        swap(i, offset);
    }
}
 
void countnum()
{
    if(    (a[a[B0]]*B0+a[a[B1]]*B1+a[a[B2]]*B2+a[a[B3]]*B3)%N==K  )
        count++;    
}
 
void swap(int i, int offset)
{
	if(i==offset)return;
    int temp;
    temp = a[offset];
    a[offset] = a[i];
    a[i] = temp;
}
//end //提示：自动阅卷结束唯一标识，请勿删除或增加。
