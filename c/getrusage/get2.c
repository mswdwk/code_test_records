#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

void main(){
	//int who = RUSAGE_SELF;
	int who = RUSAGE_THREAD;
	struct rusage rup[20];
	int ret;
	int i ;
	int a =0;
	// t0
		getrusage(RUSAGE_SELF, &rup[0]);
	// for(i = 0; i<10000000;++i);
	sleep(1);
	// t1
		getrusage(RUSAGE_SELF, &rup[1]);
	// for(i = 0; i<10000000;++i);
	// t2
		getrusage(RUSAGE_SELF, &rup[2]);

	a++;
	printf("i=%d a=%d\n",i,a);
	//printf("cpu total cost %llu sec %llu usec\n",sec,usec);
	printf("0 cpu user cost %llu sec %llu usec\n",rup[0].ru_utime.tv_sec,rup[0].ru_utime.tv_usec);
	printf("0 cpu system cost %llu sec %llu usec\n",rup[0].ru_stime.tv_sec,rup[0].ru_stime.tv_usec);
	printf("1 cpu user cost %llu sec %llu usec\n",rup[1].ru_utime.tv_sec,rup[1].ru_utime.tv_usec);
	printf("1 cpu system cost %llu sec %llu usec\n",rup[1].ru_stime.tv_sec,rup[1].ru_stime.tv_usec);
	printf("2 cpu user cost %llu sec %llu usec\n",rup[2].ru_utime.tv_sec,rup[2].ru_utime.tv_usec);
	printf("2 cpu system cost %llu sec %llu usec\n",rup[2].ru_stime.tv_sec,rup[2].ru_stime.tv_usec);
}
