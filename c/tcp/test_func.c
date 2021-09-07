#include "test_func.h"

void test_inet(){
	struct  in_addr addr1,addr2;
	ulong l1,l2;
	l1= inet_addr("192.168.0.74");
	l2 = inet_addr("192.168.21.179");
	memcpy(&addr1, &l1, sizeof(ulong));
	memcpy(&addr2, &l2, sizeof(ulong));
	printf("%s : %s,", inet_ntoa(addr1), inet_ntoa(addr2)); //注意这一句的运行结果
	printf("%s,", inet_ntoa(addr1));
	printf("%s\n", inet_ntoa(addr2));
	printf("addr1 %p\n", inet_ntoa(addr1));
	printf("addr2 %p\n", inet_ntoa(addr2));
	printf("addr2 %p\n", inet_ntoa(addr2));
}
