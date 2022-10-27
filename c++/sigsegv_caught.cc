#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void segfault_sigaction(int signal, siginfo_t *si, void *arg)
{
    //printf("Caught segfault at address %p\n", si->si_addr);
    //exit(0);
}

void sigv(int sig)
{
	printf("sig=%d\n",sig);
}
int main(void)
{
    char *foo = NULL;
    struct sigaction sa;

    memset(&sa, 0, sizeof(struct sigaction));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = segfault_sigaction;
    sa.sa_flags   = SA_NOCLDSTOP;//0;//SA_SIGINFO;

    //sigaction(SIGSEGV, &sa, NULL);
	//sigignore(SIGSEGV);
	//signal(SIGSEGV, SIG_IGN);
	signal(SIGSEGV, sigv);

    /* Cause a seg fault */
    //*foo = 1;
	printf("hello %s\n",*foo);
	int d=0;
	printf("hello %d\n",++d);

    return 0;
}
