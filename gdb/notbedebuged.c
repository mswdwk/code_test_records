#include <stdio.h>
#include <signal.h>
#include <unistd.h>
 
void handler(int signo)
{
	printf("sig=%d\n",signo);
}
 
int main(void)
{
    signal(SIGTRAP, handler);
    signal(SIGSTOP, handler);
    __asm__("nop\n\t"
        "int3\n\t");
    printf("Hello from main!\n");
    int seconds = 0;
    int step = 2;
    for(;;){
	sleep(step);
	seconds += step;
    	printf("sleep %d seconds\n",seconds);
    }
    return 0;
}
