#include<stdio.h>
void main(){
	int arr[] = {11,12,13,14,15};
	int *ptr = arr;
	//*(ptr++) += 100;
	ptr = &arr[1];
	
	*(ptr++) = *(ptr++) + 100;
	int i = 0;
	for(;i <sizeof(arr)/sizeof(arr[0]);++i)	
		printf("a[%d] %d \r\n",i,arr[i]);

	printf("end:%d %d \n",*ptr,*(++ptr));
	int arr2[][3]={{1},{2}};
	//int arr3[][]={{1},{2}};
	int x = 6, y = 8 ,z = 9;
	x += (y+4,z+5);
	int xx = (y+4,z+5);

	printf("x: %d %d\n",x,xx);
}
