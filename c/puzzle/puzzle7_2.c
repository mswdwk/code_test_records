/*
this code can not be compiled successfully,becuse function foobar1 has no argument.
*/
#include "stdio.h"
void foobar1(void)
{
    printf("In foobar1\n");
}
 
void foobar2()
{
    printf("In foobar2\n");
}
 
int main()
{
    char ch = 'a';
 
    foobar1(33,ch);
    foobar2();
 
    return 0;
}