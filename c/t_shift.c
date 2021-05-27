#include<stdio.h>
void main(){
    int a = 0b0101;
    int a1 = 0b01001;
    int b = a >> 2;
    int c = a1 << 2;
    printf("b = %d c = %d\n",b,c);
}
