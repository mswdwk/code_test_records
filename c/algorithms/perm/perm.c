// test.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>  

int n = 0;

void swap(int *a, int *b)
{
	int m;
	m = *a;
	*a = *b;
	*b = m;
}
void perm(int list[], int k, int m)
{
	int i;
	if (k > m)
	{
		for (i = 0; i <= m; i++)
			printf("%d ", list[i]);
		printf("\n");
		n++;
	}
	else
	{
		for (i = k; i <= m; i++)
		{
			swap(&list[k], &list[i]);
			perm(list, k + 1, m);
			swap(&list[k], &list[i]);
		}
	}
}
int main()
{
	int list[] = { 1, 2, 3, 4, 5, 6, 7, 8 ,9 };
	perm(list, 0, 6);
	printf("total:%d\n", n);
	return 0;
}


