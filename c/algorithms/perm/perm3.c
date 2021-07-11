// test.cpp: 定义控制台应用程序的入口点。
//
#ifdef WIN
#include "stdafx.h"
#endif
#include <stdio.h>  
#include <set>
#include <map>
#include <string>
//#include <std>
using namespace std;

set<string> all_perm_strings;
int n = 0;
string get_the_key(int list[],int m){
	string key;
	key.reserve(10*m+2);
	for(int i = 0;i<=m;++i)
		key.append(to_string(list[i]));
	return key;
}

void swap(int *a, int *b)
{
	if(a==b) return;
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
		//for (i = 0; i <= m; i++)
		//	printf("%d ", list[i]);
		string key = get_the_key(list,m);
		printf("%s\n",key.c_str());
		if(all_perm_strings.count(key) == 0){
			n++;
			all_perm_strings.insert(key);
		}
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
