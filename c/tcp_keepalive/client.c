#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define SERVER_PORT 6666

int set_keepalive(int s,int idleTime,int intvlTime,int probes)
{
	int keepAlive = 1;   // 开启keepalive属性. 缺省值: 0(关闭)  
	int keepIdle = idleTime;//10;   // 如果在10秒内没有任何数据交互,则进行探测. 缺省值:7200(s)  
	int keepInterval = intvlTime;   // 探测时发探测包的时间间隔为2秒. 缺省值:75(s)  
	int keepCount = probes;   // 探测重试的次数. 全部超时则认定连接失效..缺省值:9(次)  
	int ret = setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (void*)&keepAlive, sizeof(keepAlive));  
	ret |= setsockopt(s, SOL_TCP, TCP_KEEPIDLE, (void*)&keepIdle, sizeof(keepIdle));  
	ret |= setsockopt(s, SOL_TCP, TCP_KEEPINTVL, (void*)&keepInterval, sizeof(keepInterval));  
	ret |= setsockopt(s, SOL_TCP, TCP_KEEPCNT, (void*)&keepCount, sizeof(keepCount));  
	return ret;	
}

/*
连接到服务器后，会不停循环，等待输入，
输入quit后，断开与服务器的连接
*/
int main(int argc,char*argv[])
{
	if(argc < 2){
		printf("Usage: %s hostip port\n",argv[0]);
		return 0;
	}
	const char*host = argv[1];
	
	//客户端只需要一个套接字文件描述符，用于和服务器通信
	int clientSocket;
	//描述服务器的socket
	struct sockaddr_in serverAddr;
	char sendbuf[200];
	char recvbuf[200];
	int iDataNum;
	if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return 1;
	}
	set_keepalive(clientSocket,10,2,3);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	//指定服务器端的ip，本地测试：127.0.0.1
	//inet_addr()函数，将点分十进制IP转换成网络字节序IP
	serverAddr.sin_addr.s_addr = inet_addr(host);
	if(connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror("connect");
		return 1;
	}
	printf("连接到主机...\n");
	while(1){
		printf("发送消息:");
		scanf("%s", sendbuf);
		printf("\n");
		send(clientSocket, sendbuf, strlen(sendbuf), 0);
		printf("send %ld bytes\n",strlen(sendbuf));
		/*if(strcmp(sendbuf, "quit") == 0)
		break;
		printf("读取消息:");
		recvbuf[0] = '\0';
		iDataNum = recv(clientSocket, recvbuf, 200, 0);
		recvbuf[iDataNum] = '\0';
		printf("%s\n", recvbuf);*/
	}
	close(clientSocket);
	return 0;
}
