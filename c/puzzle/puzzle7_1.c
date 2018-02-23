/*this code can be compiled successfully,becuse function foobar2's argument is not known.
*/
#include <stdio.h>
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
 
    foobar1();
    foobar2(33, ch);
 
     return 0;
}