#include<iostream>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

void swap1(int *left, int *right)
{
    int temp = *left;
    *left = *right;
    *right = temp;
}

void swap2(int &left, int &right)
{
    int temp = left;
    left = right;
    right = left;
}

void swap3(int &left, int &right)
{
    if (&left != &right) 
    {
        left ^= right;
        right ^= left;
        left ^= right;
    }
}

/*****************************************************************/
/* 冒泡排序时间复杂度最好的情况为O(n),最坏的情况是O(n^2)
* 基本思想是:两两比较相邻记录的关键字,如果反序则交换 */

void BubbleSort1(int arr[], int num)
{
    int i, j;
    for (i = 0; i < num; i++)
    {
        for (j = 1; j < num - i; j++)
        {
            if (arr[j - 1] > arr[j])
                swap1(&arr[j - 1], &arr[j]);
        }
    }
}

// 改进思路：设置标志位，明显如果有一趟没有发生交换（flag = flase)，说明排序已经完成.
void BubbleSort2(int arr[], int num)
{
    int k = num;
    int j;
    bool flag = true;
    while (flag)
    {
        flag = false;
        for (j = 1; j < k; j++)
        {
            if (arr[j - 1] > arr[j])
            {
                swap1(&arr[j - 1], &arr[j]);
                flag = true;
            }
        }
        k--;
    }
}
//改进思路：记录一轮下来标记的最后位置，下次从头部遍历到这个位置就Ok
void BubbleSort3(int arr[], int num)
{
    int k, j;
    int flag = num;
    while (flag > 0)
    {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++)
        {
            if (arr[j - 1] > arr[j])
            {
                swap1(&arr[j - 1], &arr[j]);
                flag = j;
            }
        }
    }
}
/*************************************************************************/

/**************************************************************************/
/*插入排序: 将一个记录插入到已经排好序的有序表中, 从而得到一个新的,记录数增1的有序表
* 时间复杂度也为O(n^2), 比冒泡法和选择排序的性能要更好一些 */

void InsertionSort(int arr[], int num)
{
    int temp;
    int i, j;
    for (i = 1; i < num; i++)
    {
        temp = arr[i];
        for (j = i; j > 0 && arr[j - 1] > temp; j--)
            arr[j] = arr[j - 1];
        arr[j] = temp;
    }
}

/****************************************************************************/

/*希尔排序:先将整个待排元素序列分割成若干子序列（由相隔某个“增量”的元素组成的）分别进行
直接插入排序，然后依次缩减增量再进行排序，待整个序列中的元素基本有序（增量足够小）时，
再对全体元素进行一次直接插入排序（增量为1）。其时间复杂度为O(n^3/2),要好于直接插入排序的O(n^2) */
void ShellSort(int *arr, int N)
{
    int i, j, increment;
    int tmp;
    for (increment = N / 2; increment > 0; increment /= 2)
    {
        for (i = increment; i < N; i++)
        {
            tmp = arr[i];
            for (j = i; j >= increment; j -= increment)
            {
                if (arr[j - increment] > tmp)
                    arr[j] = arr[j - increment];
                else
                    break;
            }
            arr[j] = tmp;
        }

    }
}

/**************************************************************************/

/* 简单选择排序(simple selection sort) 就是通过n-i次关键字之间的比较,从n-i+1
* 个记录中选择关键字最小的记录,并和第i(1<=i<=n)个记录交换之
* 尽管与冒泡排序同为O(n^2),但简单选择排序的性能要略优于冒泡排序 */

void SelectSort(int arr[], int num)
{
    int i, j, Mindex;
    for (i = 0; i < num; i++)
    {
        Mindex = i;
        for (j = i + 1; j < num; j++)
        {
            if (arr[j] < arr[Mindex])
                Mindex = j;
        }

        swap1(&arr[i], &arr[Mindex]);
    }
}

/********************************************************************************/
/*假设初始序列含有n个记录,则可以看成n个有序的子序列,每个子序列的长度为1,然后
* 两两归并,得到(不小于n/2的最小整数)个长度为2或1的有序子序列,再两两归并,...
* 如此重复,直至得到一个长度为n的有序序列为止,这种排序方法称为2路归并排序
* 时间复杂度为O(nlogn),空间复杂度为O(n+logn),如果非递归实现归并,则避免了递归时深度为logn的栈空间
* 空间复杂度为O(n) */


/*lpos is the start of left half, rpos is the start of right half*/
void merge(int a[], int tmp_array[], int lpos, int rpos, int rightn)
{
    int i, leftn, num_elements, tmpos;

    leftn = rpos - 1;
    tmpos = lpos;
    num_elements = rightn - lpos + 1;

    /*main loop*/
    while (lpos <= leftn && rpos <= rightn)
        if (a[lpos] <= a[rpos])
            tmp_array[tmpos++] = a[lpos++];
        else
            tmp_array[tmpos++] = a[rpos++];

    while (lpos <= leftn) /*copy rest of the first part*/
        tmp_array[tmpos++] = a[lpos++];
    while (rpos <= rightn) /*copy rest of the second part*/
        tmp_array[tmpos++] = a[rpos++];

    /*copy array back*/
    for (i = 0; i < num_elements; i++, rightn--)
        a[rightn] = tmp_array[rightn];
}


void msort(int a[], int tmp_array[], int left, int right)
{
    int center;

    if (left < right)
    {
        center = (right + left) / 2;
        msort(a, tmp_array, left, center);
        msort(a, tmp_array, center + 1, right);
        merge(a, tmp_array, left, center + 1, right);
    }
}



void merge_sort(int a[], int n)
{
    int *tmp_array;
    tmp_array = (int *)malloc(n * sizeof(int));

    if (tmp_array != NULL)
    {
        msort(a, tmp_array, 0, n - 1);
        free(tmp_array);
    }

    else
        printf("No space for tmp array!\n");
}

/************************************************************************************/
/* 堆是具有下列性质的完全二叉树:每个节点的值都大于或等于其左右孩子节点的值,称为大顶堆；
* 或者每个节点的值都小于或等于其左右孩子节点的值,称为小顶堆*/

/*堆排序就是利用堆进行排序的方法.基本思想是:将待排序的序列构造成一个大顶堆.此时,整个序列的最大值就是堆顶
* 的根结点.将它移走(其实就是将其与堆数组的末尾元素交换, 此时末尾元素就是最大值),然后将剩余的n-1个序列重新
* 构造成一个堆,这样就会得到n个元素的次大值.如此反复执行,便能得到一个有序序列了
*/
/* 时间复杂度为 O(nlogn),好于冒泡,简单选择,直接插入的O(n^2) */

// 构造大顶堆
#define leftChild(i) (2*(i) + 1)

void percDown(int *arr, int i, int N)
{
    int tmp, child;
    for (tmp = arr[i]; leftChild(i) < N; i = child)
    {
        child = leftChild(i);
        if (child != N - 1 && arr[child + 1] > arr[child])
            child++;
        if (arr[child] > tmp)
            arr[i] = arr[child];
        else
            break;
    }
    arr[i] = tmp;
}

void HeapSort(int *arr, int N)
{
    int i;
    for (i = N / 2; i >= 0; i--)
        percDown(arr, i, N);
    for (i = N - 1; i > 0; i--)
    {
        swap1(&arr[0], &arr[i]);
        percDown(arr, 0, i);
    }
}

void print_arr(int arr[],int n)
{
	int i;
	for(i = 0; i < n ;++i){
		printf("%d ",arr[i]);
	}
	printf("\n");
}

int main(void)
{
    int arr[] = { 9, 2, 5, 8, 3, 4, 7, 1, 6, 10};
	cout<<"before sort"<<endl;
	print_arr(arr,sizeof(arr)/sizeof(arr[0]));
	int n = sizeof(arr)/sizeof(arr[0]);
    HeapSort(arr, n);
	cout<<"after sort"<<endl;
	
    for (int i = 0; i < n; i++)
        cout << arr[i] << ' ';
    cout << endl;

    return 0;
}
