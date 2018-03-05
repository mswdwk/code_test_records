// pcap_parser.cpp: 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<time.h>
#include<errno.h>

#define BUFSIZE 10240
#define STRSIZE 1024
//typedef long bpf_int32;               // maybe 8 bytes in windows10 x64 system
//typedef unsigned long bpf_u_int32;    // maybe 8 bytes in windows10 x64 system

typedef int int32;
typedef unsigned int u_int32;
typedef unsigned short  u_short;
typedef unsigned short u_int16;
typedef unsigned char u_int8;

//pacp文件头结构体
struct pcap_file_header
{
	int32 magic;       /* 0xa1b2c3d4 */
	u_short version_major;   /* magjor Version 2 */
	u_short version_minor;   /* magjor Version 4 */
	int32 thiszone;      /* gmt to local correction */
	u_int32 sigfigs;     /* accuracy of timestamps */
	u_int32 snaplen;     /* max length saved portion of each pkt */
	u_int32 linktype;    /* data link type (LINKTYPE_*) */
};
//时间戳
struct time_val
{
	u_int32 tv_sec;         /* seconds 含义同 time_t 对象的值 */
	u_int32 tv_usec;        /* and microseconds */
};
//pcap数据包头结构体
struct pcap_pkthdr
{
	struct time_val ts;  /* time stamp */
	u_int32 caplen; /* length of portion present */
	u_int32 len;    /* length this packet (off wire) */
};
//数据帧头
typedef struct FramHeader_t
{ //Pcap捕获的数据帧头
	u_int8 DstMAC[6]; //目的MAC地址
	u_int8 SrcMAC[6]; //源MAC地址
	u_short FrameType;    //帧类型
} FramHeader_t;
//IP数据报头
typedef struct IPHeader_t
{ //IP数据报头
	u_int8 Ver_HLen;       //版本+报头长度
	u_int8 TOS;            //服务类型
	u_int16 TotalLen;       //总长度
	u_int16 ID; //标识
	u_int16 Flag_Segment;   //标志+片偏移
	u_int8 TTL;            //生存周期
	u_int8 Protocol;       //协议类型
	u_int16 Checksum;       //头部校验和
	u_int32 SrcIP; //源IP地址
	u_int32 DstIP; //目的IP地址
} IPHeader_t;
//TCP数据报头
typedef struct TCPHeader_t
{ //TCP数据报头
	u_int16 SrcPort; //源端口
	u_int16 DstPort; //目的端口
	u_int32 SeqNO; //序号
	u_int32 AckNO; //确认号
	u_int8 HeaderLen; //数据报头的长度(4 bit) + 保留(4 bit)
	u_int8 Flags; //标识TCP不同的控制消息
	u_int16 Window; //窗口大小
	u_int16 Checksum; //校验和
	u_int16 UrgentPointer;  //紧急指针
}TCPHeader_t;

struct TCP_DATA{
    TCPHeader_t *tcp_header;
    void*data;
    int tcp_data_len;
};

//
void match_http(FILE *fp, char *head_str, char *tail_str, char *buf, int total_len); //查找 http 信息函数
																					 //

void tcp_consumer_pthread(void*ptr)
{
   
}

void tcp_producer_pthread(void*ptr)
{
   
}

//Important!
#pragma pack(1)
#define TAG_TYPE_SCRIPT 18
#define TAG_TYPE_AUDIO  8
#define TAG_TYPE_VIDEO  9
typedef unsigned char byte;
typedef unsigned int uint;
typedef struct {
	byte Signature[3];
	byte Version;
	byte Flags;
	uint DataOffset;
} FLV_HEADER;
typedef struct {
	byte TagType;
	byte DataSize[3];
	byte Timestamp[3];
	uint Reserved;
} TAG_HEADER;

typedef struct flv_val
{
	char *val;
	int len;
} flv_val;
static unsigned char flv_header_data[] = { 0x46,0x4C,0x56,0x01,0x05,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00 };//flv的header+previoustagsize
static flv_val flv_val_header = { (char*)flv_header_data ,13 };

int main(int argc,char *argv[])
{
	struct pcap_file_header *file_header;
	struct pcap_pkthdr *pkt_header;
	IPHeader_t *ip_header;
	TCPHeader_t *tcp_header;
	FILE *fp, *output;
	int   pkt_offset, i = 0;
	int ip_len, http_len, ip_proto;
	int src_port, dst_port, tcp_flags;
	char buf[BUFSIZE], my_time[STRSIZE];
	char src_ip[STRSIZE], dst_ip[STRSIZE];
	char  host[STRSIZE], uri[BUFSIZE];
	//初始化
	file_header = (struct pcap_file_header *)malloc(sizeof(struct pcap_file_header));
	pkt_header = (struct pcap_pkthdr *)malloc(sizeof(struct pcap_pkthdr));
	ip_header = (IPHeader_t *)malloc(sizeof(IPHeader_t));
	tcp_header = (TCPHeader_t *)malloc(sizeof(TCPHeader_t));
	memset(buf, 0, sizeof(buf));
	if(argc < 2){
		printf("use %s pcapfilename\n",argv[0]);
		return 0;
	}
	char *pcapfilename = argv[1];
	if ((fp = fopen(pcapfilename, "rb")) == NULL)
	{
		printf("error: can not open pcap file\n");
		exit(0);
	}
	if ((output = fopen("output.txt", "w + ")) == NULL)
	{
		printf("error: can not open output file\n");
		exit(0);
	}
    pthread_t tcp_consumer_tid;
    pthread_create(&tcp_consumer_tid,NULL,tcp_consumer_pthread,NULL);
	//开始读数据包
	pkt_offset = 24; //pcap文件头结构 24个字节
	while (fseek(fp, pkt_offset, SEEK_SET) == 0) //遍历数据包
	{
		i++;
		//pcap_pkt_header 16 byte
		if (fread(pkt_header, 16, 1, fp) != 1) //读pcap数据包头结构
		{
			printf("\nRead end of pcap file. Total parser %d packets.\n",i-1);
			break;
		}
        if(pkt_header->caplen < 14 + sizeof(IPHeader_t)) {
            printf("total parser %d packets.pkt_header->caplen 0x%08x len 0x%08x second %d size %d %d\n",i,
                pkt_header->caplen ,pkt_header->len,pkt_header->ts.tv_sec,sizeof(pkt_header->ts.tv_sec)
                ,sizeof(pkt_header->caplen));
            break;
       }
		pkt_offset += 16 + pkt_header->caplen;   //下一个数据包的偏移值
		
		//time_t ti = ntohl(pkt_header->ts.tv_sec);
		time_t ti = (pkt_header->ts.tv_sec);
		struct tm * tinfo = localtime(&ti);
		//strftime(my_time, sizeof(my_time), "%Y - %m - %d %T", localtime(&(pkt_header->ts.tv_sec))); //获取时间
		if(tinfo)
			strftime(my_time, sizeof(my_time), "%Y-%m-%d %T", tinfo); //获取时间
		else
			printf("localtime failed. tv_sec = %u. Error %s\n",pkt_header->ts.tv_sec,strerror(errno));																						// printf("%d: %s\n", i, my_time);
																									//数据帧头 14字节
		fseek(fp, 14, SEEK_CUR); //忽略数据帧头
								 //IP数据报头 20字节
		if (fread(ip_header, sizeof(IPHeader_t), 1, fp) != 1)
		{
			printf("%d: can not read ip_header\n", i);
			break;
		}
		inet_ntop(AF_INET, (void *)&(ip_header->SrcIP), src_ip, 16);
		inet_ntop(AF_INET, (void *)&(ip_header->DstIP), dst_ip, 16);
		ip_proto = ip_header->Protocol;
		ip_len = ip_header->TotalLen; //IP数据报总长度
									  // printf("%d:  src=%s\n", i, src_ip);
		if (ip_proto != 0x06) continue; //判断是否是 TCP 协议
		
		
		//TCP头 20字节
		if (fread(tcp_header, sizeof(TCPHeader_t), 1, fp) != 1)
		{
			printf("%d: can not read tcp_header\n", i);
			break;
		}
        short unsigned int ip_header_len = ((ip_header->Ver_HLen & 0xf)<<2);
        //short unsigned int tcp_header_len = (((tcp_header->HeaderLen & 0xf0)>>4)<<2);
        short unsigned int tcp_header_len = ((tcp_header->HeaderLen & 0xf0)>>2);
		src_port = ntohs(tcp_header->SrcPort);
		dst_port = ntohs(tcp_header->DstPort);
		tcp_flags = tcp_header->Flags;
		// printf("%d:  src=%x\n", i, tcp_flags);
	#if 0
		if (tcp_flags == 0x18) // (PSH, ACK) 3路握手成功后
		{
			if (dst_port == 80) // HTTP GET请求
			{
				http_len = ip_len - 40; //http 报文长度
				match_http(fp, "Host: ", "\r\n", host, http_len); //查找 host 值
				match_http(fp, "GET ", "HTTP", uri, http_len); //查找 uri 值
				sprintf(buf, "packet id %d: %s  src = %s : %d  dst = %s : %d  %s%s\r\n", i, my_time, src_ip, src_port, dst_ip, dst_port, host, uri);
				//printf("%s", buf);
				if (fwrite(buf, strlen(buf), 1, output) != 1)
				{
					printf("output file can not write\n");
					break;
				}
			}
		}
         #else
             if(src_port == 80 || dst_port == 80){
                unsigned short int tcp_data_len = ntohs(ip_len) - ip_header_len - tcp_header_len;
                char *tcp_data_p;char *tcp_data_buf = calloc(1,tcp_data_len);
                tcp_data_p = tcp_data_buf;
                fread(tcp_data_buf,tcp_data_len,1,fp);
#if 0
                TCP_DATA *tcp_ptr = calloc(1,sizeof(TCP_DATA));
                tcp_ptr->data = tcp_data_buf;
                tcp_ptr->tcp_data_len = tcp_data_len;
                tcp_ptr->tcp_header = calloc(1,TCPHeader_t);
                memcpy(tcp_ptr->tcp_header,tcp_header,sizeof(TCPHeader_t));
                int ret = ring_enqueue(ring_ptr,(void*)tcp_ptr);
#endif
                int found_flv = find_flv_data(tcp_data_buf,tcp_data_len);
                free(tcp_data_buf);
                sprintf(buf,"[%s]%5u tcp_data_len %5u flag 0x%02x win %5u %15s:%5u -> %15s:%5u flv %1u %08x..\n",my_time,
                    i,tcp_data_len,0xff&tcp_flags,(tcp_header->Window),src_ip, src_port, dst_ip, dst_port,found_flv,*(int*)tcp_data_p);
                fwrite(buf, strlen(buf), 1, output);
                memset(buf,0,sizeof(buf));
                if(found_flv)
                    printf("packet %d is flv\n",i);
             }
         #endif
	} // end while
	fclose(fp);
	fclose(output);
    free(file_header);
    free(pkt_header);
    free(ip_header);
    free(tcp_header);
    pthread_join(tcp_consumer_tid,NULL);
	return 0;
}

//查找 HTTP 信息
void match_http(FILE *fp, char *head_str, char *tail_str, char *buf, int total_len)
{
	int i;
	int http_offset;
	int head_len, tail_len, val_len;
	char head_tmp[STRSIZE], tail_tmp[STRSIZE];
	//初始化
	memset(head_tmp, 0, sizeof(head_tmp));
	memset(tail_tmp, 0, sizeof(tail_tmp));
	head_len = strlen(head_str);
	tail_len = strlen(tail_str);
	//查找 head_str
	http_offset = ftell(fp); //记录下HTTP报文初始文件偏移
	while ((head_tmp[0] = fgetc(fp)) != EOF) //逐个字节遍历
	{
		if ((ftell(fp) - http_offset) > total_len) //遍历完成
		{
			//sprintf(buf, "can not find %s \r\n", head_str);
            printf("can not find %s. http_offset %d\r\n", head_str,http_offset);
			exit(0);
		}
		if (head_tmp[0] == *head_str) //匹配到第一个字符
		{
			for (i = 1; i<head_len; i++) //匹配 head_str 的其他字符
			{
				head_tmp[i] = fgetc(fp);
				if (head_tmp[i] != *(head_str + i))
					break;
			}
			if (i == head_len) //匹配 head_str 成功，停止遍历
				break;
		}
	}
	// printf("head_tmp=%s \n", head_tmp);
	//查找 tail_str
	val_len = 0;
	while ((tail_tmp[0] = fgetc(fp)) != EOF) //遍历
	{
		if ((ftell(fp) - http_offset) > total_len) //遍历完成
		{
			sprintf(buf, "can not find %s \r\n", tail_str);
			exit(0);
		}
		buf[val_len++] = tail_tmp[0]; //用buf 存储 value 直到查找到 tail_str
		if (tail_tmp[0] == *tail_str) //匹配到第一个字符
		{
			for (i = 1; i<tail_len; i++) //匹配 head_str 的其他字符
			{
				tail_tmp[i] = fgetc(fp);
				if (tail_tmp[i] != *(tail_str + i))
					break;
			}
			if (i == tail_len) //匹配 head_str 成功，停止遍历
			{
				buf[val_len - 1] = 0; //清除多余的一个字符
				break;
			}
		}
	}
	// printf("val=%s\n", buf);
	fseek(fp, http_offset, SEEK_SET); //将文件指针 回到初始偏移
}

int find_flv_data(void*data,int len)
{
    char *ch = (char*)data;
    char *flv_val = flv_val_header.val;
    int i ;
    for( i = 0; i < len - 3 ; ++i){
        int flv_ver = htonl(0x464C5601);
        if( *(int*)ch == flv_ver){
            return 1;
        }
        ch++;
    }
    
    return 0;
}

