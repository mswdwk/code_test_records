#include<stdio.h>
#include<set>
#include<iostream>    

using namespace std;

void test_set_string() {
#if 1
    string myints1[] = { "10","20","30","40","50" };
    string myints2[] = { "50","40","30","20","10" };
    set<string> s1(myints1, myints1 + 5);
    std::set<string> s2(myints2, myints2 + 5);
#else
    std::set<string> s1;
    std::set<string> s2;
    s1.insert("10");
    s2.insert("50");
#endif

    if (s1 == s2) {
        printf("sets: true");
    }
    else printf("sets: false");
    std::set<string>::iterator it2 = s2.begin();
    for (std::set<string>::iterator it1 = s1.begin();it1 != s1.end();it1++) {
        printf("\ns1: %s  s2: %s", it1->c_str(), it2->c_str());
        // printf("\n s2 %s",it2->c_str());
        it2++;
    }
    printf("\n");
}

int main()
{
	test_set_string();
	return 0;
}
