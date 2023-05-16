#include <stdio.h>

int main(){
	
	unsigned char s1[] = "°¢";
	printf("%02X%02X\n", s1[0], s1[1]);
	return 0;
}
