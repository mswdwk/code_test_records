/*select实现超时检测函数
*/
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_PORT 6666
#define ERR_EXIT(str)  printf("%s",(str)); return;
/**
*read_timeout读超时检测函数，不含读操作
*wait_seconds 等待超时秒数。如果为0，不检测超时
*成功（返回0，未超时），失败返回-1（超时）并且errno=ETIMDEOUT
*仅仅检测IO是否产生了超时。
*/
int read_timeout(int fd,unsigned int wait_seconds)
{
    int ret=0;
    if(wait_seconds>0)
    {
        fd_set read_fdset;
        struct timeval timeout;
        FD_ZERO(&read_fdset);
        FD_SET(fd,&read_fdset);
        
        timeout.tv_sec=wait_seconds;
        timeout.tv_usec=0;
        do
        {
            ret=select(fd+1,&read_fdset,NULL,NULL,&timeout);
        }while(ret<0&&errno==EINTR);
            
        if(ret==0)//无事件可读，超时
        {
            ret=-1;
            errno=ETIMEDOUT;
        }
        else if(ret==1)//检测到一个可读事件
            ret=0;
    }
    
    return ret;//wait_seconds==0 直接返回
}

/*
写超时检测函数,不含写操作
成功未超时返回0，失败超时返回-1且errno=ETIMEDOUT
*/
int write_timeout(int fd,unsigned int wait_seconds)
{
    int ret=0;
    if(wait_seconds>0)
    {
        fd_set write_fdset;
        struct timeval timeout;
        FD_ZERO(&write_fdset);
        FD_SET(fd,&write_fdset);
        
        timeout.tv_sec=wait_seconds;
        timeout.tv_usec=0;
        do
        {
            ret=select(fd+1,NULL,&write_fdset,NULL,&timeout);
        }while(ret<0&&errno==EINTR);
            
        if(ret==0)//超时
        {
            ret=-1;
            errno=ETIMEDOUT;
        }
        else if(ret==1)//检测到一个事件
            ret=0;
    }
    
    return ret;//wait_seconds==0 直接返回
}

/*
accept_timeout 带超时的accept函数（包含accept操作）...select在如果是监听套接口(服务器)，已完成连接队列不为空时返回
fd:套接字
addr:输出参数,返回对方地址
wait_seconds:等待返回的秒数。如果为0表示正常模式
成功未超时返回已连接套接字，超时返回-1并且errno=ETIMEDOUT
 int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
*/
int accept_timeout(int fd,struct sockaddr_in * addr,unsigned int wait_seconds)
{
    int ret;
    socklen_t addrlen=sizeof(struct sockaddr_in);
    if(wait_seconds>0)
    {
        fd_set accept_fdset;
        struct timeval timeout;
        FD_ZERO(&accept_fdset);
        FD_SET(fd,&accept_fdset);
        timeout.tv_sec=wait_seconds;
        timeout.tv_usec=0;
        do
        {
            ret=select(fd+1,&accept_fdset,NULL,NULL,&timeout);
        }while(ret==-1&&errno==EINTR);
        
        if(ret==-1)
            return -1;
        else if(ret==0)
        {    
            errno=ETIMEDOUT;
            return -1;
        }
    }
    if(addr!=NULL)
        ret=accept(fd,(struct sockaddr *)addr,&addrlen);
    else 
        ret=accept(fd,NULL,NULL);
    if(ret==-1)
        ERR_EXIT("accept error");
    return ret;
}

/**连接建立三次时，客户端收到ack返回。一次握手往返的时间称为RTT,三次握手 时若有拥塞，系统默认connect返回时间75s超时
   连接超时函数
*connect:建立三次握手时connect在服务器返回确认时就返回了。设定自己的连接超时时间。
*fd:套接字
*addr:要连接的对端服务器地址
*wait_seconds:等待超时秒数，如果为0表示正常模式
*成功(未超时)返回0，失败超时返回-1，并且errno=ETIMEDOUT
**/
//将套接口设置为非阻塞模式，防止直接调用connect阻塞
void activate_nonblock(int fd)
{
    int ret;
    int flags=fcntl(fd,F_GETFL);
    if(flags==-1)
        ERR_EXIT("fcntl error");
    flags|=O_NONBLOCK;//添加非阻塞模式
    ret=fcntl(fd,F_SETFL,flags);
    if(ret==-1)
        ERR_EXIT("fcntl error");
}
//将套接口还原为阻塞模式
void deactivate_nonblock(int fd)
{
    int ret;
    int flags=fcntl(fd,F_GETFL);
    if(flags==-1)
        ERR_EXIT("fcntl");
    flags&=~O_NONBLOCK;
    ret=fcntl(fd,F_SETFL,flags);
    if(ret==-1)
        ERR_EXIT("fcntl");
}
int connect_timeout(int fd,struct sockaddr_in * addr,unsigned int wait_seconds)
{
    int ret;
    socklen_t addrlen=sizeof(struct sockaddr_in);
    if(wait_seconds>0)
        activate_nonblock(fd);//将套接口设置为非阻塞
    ret=connect(fd,(struct sockaddr *)addr,addrlen);//已经将套接口设置为非阻塞了，如果不能够立即连接成功，则直接返回EINPROGRESS错误。
	printf("connect one time, ret=%d errno=%d\n",ret,errno);
    //if(ret<0&&errno==EINPROGRESS)//连接正在处理。处理超时
    if(ret<0)//连接正在处理。处理超时
    {
        fd_set connect_fdset;//可写事件集合
        struct timeval timeout;
        FD_ZERO(&connect_fdset);
        FD_SET(fd,&connect_fdset);
        timeout.tv_sec=wait_seconds;
        timeout.tv_usec=0;
	int count = 1;
        do
        {
            //一旦connect建立连接，套接口就可以写了。
            ret=select(fd+1,NULL,&connect_fdset,NULL,&timeout);
		sleep(1);
	count++;
	printf("connect %d time,ret = %d errno=%d\n",count,ret,errno);
        
        }while(ret<0 || errno == 111);
        //}while(ret<0&&errno==EINTR);
        if(ret==0)
        {
            ret=-1;
            errno=ETIMEDOUT;
        }
	else if(ret<0)
        {
            return -1;
        }
        else if(ret==1)
        {    
            /*ret返回1，可能有两种情况。一种是fd有事件发生，connect建立连接可写了。
            *另一种情况是套接字本身产生错误.套接口上发生一个错误待处理，错误可以通过getsockopt指定SO_ERROR选项来获取
            *但是select函数没有出错，所以错误信息不能保存到errno
            *变量中。只有通过getsockopt来获取套接口fd的错误。
            */
            int err;
            socklen_t socklen=sizeof(err);
            int sockoptret=getsockopt(fd,SOL_SOCKET,SO_ERROR,&err,&socklen);//成功返回0，错误返回-1
            if(sockoptret==-1)
            {
                return -1;
            }
            if(err==0)//套接字没有错误
                ret=0;//返回0成功未超时
            else//套接字产生错误
            {
                errno=err;//套接字错误代码
                ret=-1;
            }
        }
            
    }
	//网络状况良好，直接成功了，返回ret==0
   if(wait_seconds>0)
    {
        deactivate_nonblock(fd);//重置为阻塞
    }
	return ret;
}


int main(){
	//测试代码
	int ret;
	int clientSocket;
	struct sockaddr_in serverAddr;
	if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return 1;
	}
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	//指定服务器端的ip，本地测试：127.0.0.1
	//inet_addr()函数，将点分十进制IP转换成网络字节序IP
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ret = connect_timeout(clientSocket,(struct sockaddr_in *)&serverAddr,10);
	//ret=read_timeout(fd,5);
	if(ret==0)
	{
		//gettimeofday();
		send(clientSocket,"hello\n",7,0);
	    printf("connect ok!\n");//不会阻塞
	}
	else if(ret==-1&&errno==ETIMEDOUT)
	{
	    printf("connect error...\n");
	}
	else
	{
	    ERR_EXIT("connect timeout\n");
	}
}
