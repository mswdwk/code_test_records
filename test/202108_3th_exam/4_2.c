#include<stdio.h>


move(a,n)
int a[];
int n;
{
int low,high,temp;
low=0;high=n-1;temp=a[high];
while(low<high)//不停的把前面的偶数挪动道后面
     if(a[low]%2==0) //碰到偶数,挪到后面
       {
         a[high]=a[low];
         high--;
         a[low]=a[high];
       }
    else low++; //碰到奇数，位置不动，low向前移动
a[high]=temp;
}
void display(int arr[],int n)
{
	int i = 0;
	for(;i<n;++i){
		printf("%d ",arr[i]);
	}
		printf("\n");
}
int main()
{
	int arr[]={2,4,6,8,1,0,3,7};
	printf("before move: \n");
	display(arr,sizeof(arr)/sizeof(arr[0]));
	move(arr,sizeof(arr)/sizeof(arr[0]));
	printf("after move: \n");
	display(arr,sizeof(arr)/sizeof(arr[0]));
}
