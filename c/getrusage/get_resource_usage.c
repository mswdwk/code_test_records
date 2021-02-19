#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

void main(){
	int who = RUSAGE_SELF;
	struct rusage usage;
	int ret;
	ret = getrusage(who, &usage);
	printf("user %llu\n",usage.ru_utime);
	printf("system %llu\n",usage.ru_stime);
	printf("inblock %llu\n",usage.ru_inblock);
	printf("outblock %llu\n",usage.ru_oublock);
}
