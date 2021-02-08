#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/fcntl.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include <time.h>
#include <sys/time.h>
#include<stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define SERVER_PORT 33333
#define MAXLEN 65535

void client_handle(int sock);
int set_linger(int sock, int l_onoff, int l_linger); 
static void set_nonblock(int sock){
	int flags = fcntl(sock, F_GETFL, 0); 
	fcntl(sock, F_SETFL, flags | O_NONBLOCK); 
}
int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        printf("input args %d: %s\n", i, argv[i]);
    }
    struct sockaddr_in seraddr;
    int server_port = SERVER_PORT;
    if (2 == argc) {
        server_port = atoi(argv[1]);
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &seraddr.sin_addr);
    seraddr.sin_port = htons(server_port);
	set_linger(sock,1,0);
    if (-1 == connect(sock, (struct sockaddr *)&seraddr, sizeof(seraddr))) {
        perror("connect failure");
        exit(EXIT_FAILURE);
    }
    client_handle(sock);

    return 0;
}

void client_handle(int sock) {
	int shutdown_count = 0;
    char sendbuf[MAXLEN], recvbuf[MAXLEN];
    bzero(sendbuf, MAXLEN);
    bzero(recvbuf, MAXLEN);
    int n = 0;
	set_nonblock(sock);
    while (1) {
        /*if (NULL == fgets(sendbuf, MAXLEN, stdin)) {
            break;
        }
        // 按`#`号退出
        if ('#' == sendbuf[0]) {
            break;
        }*/
        struct timeval start, end;
        gettimeofday(&start, NULL);
        // write(sock, sendbuf, strlen(sendbuf));
        n = read(sock, recvbuf, 0);//MAXLEN);
        printf("read bytes %d \n",n );
		int ret =0; 
		// sleep(1);
		// ret = close(sock);
        // n = read(sock, recvbuf, MAXLEN);
        printf("read bytes %d errno=%d\n",ret,errno );
        if (n < 0) {
        	printf("n = %d < 0 errno %d\n",n ,errno);
			if(errno!=EAGAIN || errno !=EWOULDBLOCK){
            	perror("read failure.");
            	exit(EXIT_FAILURE);
			}
			continue;
        }
        if (0 == n) {
        	printf("\nbreak \n\n",n );
			// shutdown(sock,SHUT_RDWR);
			// shutdown(sock,SHUT_WR);
            break;
        }
        write(STDOUT_FILENO, recvbuf, n);
        gettimeofday(&end, NULL);
        printf("\ntime diff=%ld microseconds\n", ((end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec)));
		break;
    }

    // n = read(sock, recvbuf, MAXLEN);
    printf("close clinet %d n %d\n",sock,n );
	set_linger(sock,1,0);
	int ret = shutdown(sock,SHUT_RDWR);
    printf("shut wr sock %d ret %d errno %d shutdown_count %d :%s\n",sock,ret,errno,shutdown_count++,strerror(errno));
    //close(sock);

	ret = shutdown(sock,SHUT_RDWR);
    printf("shut wr sock %d ret %d errno %d shutdown_count %d :%s\n",sock,ret,errno,shutdown_count++,strerror(errno));
    ret = close(sock);
    printf("close wr sock %d ret %d errno %d shutdown_count %d\n",sock,ret,errno,shutdown_count++  );
	ret = shutdown(sock,SHUT_RDWR);
    printf("shut wr sock %d ret %d errno %d shutdown_count %d :%s\n",sock,ret,errno,shutdown_count++,strerror(errno));
/*	shutdown(sock,SHUT_RDWR);


    printf("shut wr sock %d ret %d errno %d shutdown_count %d :%s\n",sock,ret,errno,shutdown_count++,strerror(errno));
	shutdown(sock,SHUT_RDWR);
    ret = close(sock);
*/
}

int set_linger(int sock, int l_onoff, int l_linger) {
    struct linger so_linger;
    so_linger.l_onoff = l_onoff;
    so_linger.l_linger = l_linger;
    int r = setsockopt(sock, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));

    return r;
}

