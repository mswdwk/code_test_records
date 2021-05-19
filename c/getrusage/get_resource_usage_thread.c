#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#define __USE_GNU /* required before including  resource.h */
#include <sys/resource.h>
//#include <linux/resource.h>

void main(){
	int who = RUSAGE_SELF;
	struct rusage rup;
	int ret;
	int i ;
	int a =0;
	for(i=0;i<1000000;i++)
		getrusage(RUSAGE_THREAD, &rup);
	a++;
	long sec = rup.ru_utime.tv_sec + rup.ru_stime.tv_sec;
	long usec = rup.ru_utime.tv_usec + rup.ru_stime.tv_usec;
	sec += usec/1000000;
	usec %= 1000000;
	printf("i=%d a=%d\n",i,a);
	printf("cpu total cost %llu sec %llu usec\n",sec,usec);
	printf("cpu system cost %llu sec\n",rup.ru_stime);
	printf("inblock %llu\n",rup.ru_inblock);
	printf("outblock %llu\n",rup.ru_oublock);
}
