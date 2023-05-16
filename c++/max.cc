#include<algorithm>
#include<string.h>

#include<stdio.h>
#include<math.h>

int cmp(const void*_a,const void*_b){
	int a=*((int*) _a);
	int b=*((int*) _b);
	if ( a == b) return 0;
	if ( a > b) return 1;
	return -1;
}

int MLS(int arr[],int n) {
    if ( n == 0)
        return 0;
    int longest = 1;//记录最长的有序序列
    int count = 1;//目前有序序列的长度
    //先对数组进行排序
    qsort(arr,n,sizeof(int),cmp);
    for (int i = 1; i < n; i++) {
        //跳过重复的
        if (arr[i] == arr[i - 1])
            continue;
        //比前一个大1，可以构成连续的序列，count++
        if ((arr[i] - arr[i - 1]) == 1) {
            count++;
        } else {
            //没有比前一个大1，不可能构成连续的，
            //count重置为1
            count = 1;
        }
        //记录最长的序列长度
        longest = longest > count ?  longest : count;
    }
    return longest;
}

int main(){
	int arr[] = {12,1,2,3,4,5,11,13,14,0,8,9,10,14};
	int n = sizeof(arr)/sizeof(arr[0]);
	printf("max length is %d\n",MLS(arr,n));
	return 0;
}
