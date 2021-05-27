#include<stdio.h>
int tadd(int x,int y){
  int s = x+y;
  if ( x>0 && y > 0 && s<=0) return 0;
  if ( x<0 && y< 0 && s>=0) return 0;
  return 1;
}

int    tsub(int a,int b)
{
    if(b == (int)(~b + 1))
    {//判断b 是不是机器可以表示的最小负整数。此式可以适用于32位及64位机器。
        printf("a - b = %d\n",(int)(a - b));
        if(a<0)return 0;
        return 1;
        /*    注意: 此处要有=,因为：如果不加=，
	当0 - (-2147483648)=2147483648在32位机中是无法表示的，
得出的结果仍然是-2147483648,这显然是不对的。
*/
    }
    else
    {
        return tadd(a,-b);
    }
}
#define OVERFLOW_TEST(a,b) \
	printf(" %d - %d , overflow %d\n ",(a),(b),tsub((a),(b)))

void main()
{
	int a = -1; 
	int b = 0x80000000;
	OVERFLOW_TEST(1,0x80000000);
	OVERFLOW_TEST(0,0x80000000);
	OVERFLOW_TEST(-1,0x80000000);
	//printf(" %d - %d , overflow %d\n ",a,b,tsub(a,b));
}
