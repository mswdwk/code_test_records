/* echo server with poll */
#include <poll.h>
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>

#define OPEN_MAX 1024
#define LISTEN_PORT 33333
#define MAX_BUF 1024

int set_linger(int sock, int l_onoff, int l_linger);
int handle_conn(struct pollfd *nfds, char* buf);
int init_char(char ch[],int len,char init){
	for(int i = 0; i < len; ++i){
		ch[i]=init;
	}
	return len;
}

void run();
void prepare_data(char buf3[],int length){
			char buf1[1374]={0};
			char buf2[71]={0};
			//char buf3[1448]={0};

			init_char(buf1,sizeof(buf1),'1');
			init_char(buf2,sizeof(buf2),'2');
			strcat(buf3,buf1);
			strcat(buf3,buf2);
			strcat(buf3,"end");
}
static void set_nonblock(int sock){
	int flags = fcntl(sock, F_GETFL, 0); 
	fcntl(sock, F_SETFL, flags | O_NONBLOCK); 
	fprintf(stderr,"set sock noblock %d\n",sock);
}
int main(int _argc, char* _argv[]) {
    run();

    return 0;
}
void run() {
    // bind socket
    char str[INET_ADDRSTRLEN];
    struct sockaddr_in seraddr, cliaddr;
    socklen_t cliaddr_len = sizeof(cliaddr);
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(LISTEN_PORT);

    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (-1 == bind(listen_sock, (struct sockaddr*)&seraddr, sizeof(seraddr))) {
        perror("bind server addr failure.");
        exit(EXIT_FAILURE);
    }
    listen(listen_sock, 5);

    int ret, i;
    struct pollfd nfds[OPEN_MAX];
    for (i=0;i<OPEN_MAX;++i){
        nfds[i].fd = -1;
    }

    nfds[0].fd = listen_sock;
    nfds[0].events = POLLIN;

    char* buf = (char*)malloc(MAX_BUF);   
    while (1) {
        ret = poll(nfds, OPEN_MAX, NULL);
        if (-1 == ret) {
            perror("poll failure.");
            exit(EXIT_FAILURE);
        }

        /* An event on one of the fds has occurred. */
        if (nfds[0].revents & POLLIN) {
            int sock = accept(listen_sock, (struct sockaddr *)&cliaddr, &cliaddr_len);
            if (-1 == sock) {
                perror("accept failure.");
                exit(EXIT_FAILURE);
            }
            printf("accept from %s:%d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));

            // set_linger(sock, 1, 0);    //设置SO_LINGER option值为0
            for (int k=0;k<OPEN_MAX;++k){
                if (nfds[k].fd < 0){
                    nfds[k].fd = sock;
                    nfds[k].events = POLLIN;
					set_nonblock(sock);
                    break;
                }
                if (k == OPEN_MAX-1){
                    perror("too many clients, nfds size is not enough.");
                    exit(EXIT_FAILURE);
                }
            }
			char buf[1461] = {0};
			prepare_data(buf,sizeof(buf));
			printf("prepare to send data %d bytes\n",sizeof(buf));
            int send_n = write(sock, buf, sizeof(buf));
			printf("send data %d bytes\n",send_n );
			ret = shutdown(sock,SHUT_RDWR);
			// shutdown(sock,SHUT_WR);
			// sleep(1);
			fprintf(stderr,"shutdown socket %d ret %d errno %d %s\n",sock,ret,errno,strerror(errno));
			ret = close(sock);
			fprintf(stderr,"close %d ret %d errno %d %s\n",sock,ret,errno,strerror(errno));
			//shutdown(sock,SHUT_RDWR);
        }

        //handle_conn(nfds, buf);
    }

    close(listen_sock);
}

int handle_conn(struct pollfd *nfds, char* buf) {
    int n = 0;
    for (int i=1;i<OPEN_MAX;++i) {
        if (nfds[i].fd<0) {
            continue;
        }

        if (nfds[i].revents & POLLIN) {
            bzero(buf, MAX_BUF);
            //n = read(nfds[i].fd, buf, MAX_BUF);
            n = read(nfds[i].fd, buf, 5);
            if (0 == n) {
                close(nfds[i].fd);
                nfds[i].fd = -1;
                continue;
            } 
            if (n>0){
                printf("recv from client: %s\n", buf);
                nfds[i].events = POLLIN;

                close(nfds[i].fd);  //接收数据后就主动关闭连接，用于RST测试          
            } else {
                perror("read failure.");
                exit(EXIT_FAILURE);
            }
        } else if (nfds[i].revents & POLLOUT) {
            printf("write data to client: %s\n", buf);
            write(nfds[i].fd, buf, sizeof(buf));
            bzero(buf, MAX_BUF);          

            nfds[i].events = POLLIN;
        }
    }

    return 0;
}

int set_linger(int sock, int l_onoff, int l_linger) {
    struct linger so_linger;
    so_linger.l_onoff = l_onoff;
    so_linger.l_linger = l_linger;
    int r = setsockopt(sock, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));

    return r;
}
