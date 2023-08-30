#include<stdio.h>
int search(int nums[],int length, int target) {
    //边界条件判断
    if (nums == NULL || length == 0)
        return -1;
    int left = 0;
    int right = length - 1;
	int count = 0;
    while (left < right) {
		count++;
        int mid = left + (right - left) / 2;
        if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
	printf("count = %d\n",count);
    return nums[left] == target ? left : -1;
}

int main()
{
	int nums[] = {1,2,3,4,5,6,11,123,235,789,1234};
	int n = sizeof(nums)/sizeof(nums[0]);
	int target = 1123;

	int location = search(nums,n,target);
	printf("search %d location %d\n",target,location);
	return 0;
}
