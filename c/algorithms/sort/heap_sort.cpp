#include<iostream>
using namespace std;

#define leftChild(i) (2*(i)+1)
void percDown(int*arr,int i,int N)
{
int tmp,child;
    for(tmp=arr[i];leftChild(i)<N;i=child){
child=leftChild(i);
        if(child!=N-1&&arr[child+1]>arr[child])
child++;
        if(arr[child]>tmp)
            arr[i]=arr[child];
        else
            break;
}
arr[i]=tmp;
}

void swap1(int*l,int*r){
	int tmp= *l;
	*l = *r;
	*l = tmp;
}

void HeapSort(int*arr,int N)
{
int i;
for(i=N/2;i>=0;i--)
percDown(arr,i,N);
for(i=N-1;i>0;i--){
swap1(&arr[0],&arr[i]);
percDown(arr,0,i);
    }
}


int main(void)
{
	int arr[]={9,2,5,8,3,4,7,1,6,10};
	HeapSort(arr,10);
	for(int i=0;i<10;i++)
		cout<<arr[i]<<' ';
		cout<<endl;
	return 0;
}
