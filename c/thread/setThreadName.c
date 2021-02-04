#include<stdio.h>
#include<stdlib.h>
#include<sys/prctl.h>

void *thread1(void *arg)
{

	prctl(PR_SET_NAME, "THREAD1-test1234567");
	while (1) {
		printf("thread1\n");
		sleep(1000);
	}
}

void thread2(void)
{
	while (1) {
		printf("thread2\n");

		sleep(1000);
	}
}

int main()
{

	pthread_t th1, th2;
	void *retval;
	pthread_create(&th1, NULL, thread1, NULL);
	pthread_create(&th2, NULL, thread2, NULL);

	printf("main thread\n");

	pthread_join(&th1, &retval);
	pthread_join(&th2, &retval);
}
