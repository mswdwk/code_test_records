#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "ip_flow.h"
#include "rte_jhash.h"
#include "pcap_parse.h"
#include "h264_parse.h"
#include "ring_buffer.h"
#include "dump_print.h"
#include "queue.h"
#include "nids.h"

#define MAX_TCP_STREAM_NUM 128

TCP_FLOW_HEADER tcp_stream_table[MAX_TCP_STREAM_NUM];


int create_pkt_mempool(int pkt_num)
{
	void *p = calloc(pkt_num,sizeof(ETH_DATA) + DATA_ROOM_BUF);
	return 0;
}

static int TCP_FLOW_ITEM_COPY(TCP_FLOW_ITEM*dst,TCP_FLOW_ITEM*src) 
{
	if(!dst||!src)return -1;
	
	memcpy(dst,src,sizeof(TCP_FLOW_ITEM));
	
	dst->tcpflow.tcph = (TCPHeader_t*)malloc(sizeof(TCPHeader_t));
	memcpy(dst->tcpflow.tcph,src->tcpflow.tcph,sizeof(TCPHeader_t));

	dst->tcpflow.data = (void*)malloc(src->tcpflow.data_len);
	if(src->tcpflow.data && src->tcpflow.data_len <= 1500){
		memcpy(dst->tcpflow.data,src->tcpflow.data,src->tcpflow.data_len);
	}
	return 0;
}

int TCP_FLOW_ITEM_COPY_FOR_LAST(TCP_FLOW_ITEM*dst,TCP_FLOW_ITEM*src) 
{
	if(!dst||!src)return -1;
	// be carefull about pointer copy!
	//memcpy(dst,src,sizeof(FLV_FLOW_ITEM));
	void *data = dst->tcpflow.data;
	void*tcph = dst->tcpflow.tcph;
	memcpy(dst,src,sizeof(TCP_FLOW_ITEM));
	dst->tcpflow.data = data;
	dst->tcpflow.tcph = tcph;
	
	TCPHeader_t *d, *s;
	d = dst->tcpflow.tcph;
	s = src->tcpflow.tcph;
	memcpy(d,s,sizeof(TCPHeader_t));
	
	if(src->tcpflow.data && src->tcpflow.data_len <= 1500){
		//memcpy(dst->tcpflow.data,src->tcpflow.data,src->tcpflow.data_len);
	}
	return 0;
}


static TCP_FLOW_ITEM*tcp_flow_data_enqueue(TCP_FLOW_ITEM*flow)
{
	if(!flow)return NULL;
	int stream_id = flow->tcpflow.stream_id;
	TCP_FLOW_HEADER*header = &tcp_stream_table[stream_id];
	
	header->cache_num++;
	flow->next = NULL;
	flow->prev = header->tail;
	if(!header->head)header->head = flow;
	if(header->tail)header->tail->next = flow;
	header->tail = flow;

	return flow;
}

static int tcp_stream_recombine(TCP_FLOW_HEADER*h,TCP_FLOW_ITEM*item,TCP_FLOW_ITEM**result)
{
	int i = 0,counter = 0;
	if(!h || !item || !h->last) return 0;

	IP_FLOW*last_tcp = &h->last->tcpflow;
	if(!last_tcp->tcph) return 0;
	TCP_FLOW_ITEM*tmp = NULL;
	
 	u_int32 last_seqno = ntohl(last_tcp->tcph->SeqNO);          //    last_seqno = h->last_seqno;
	u_int32 last_ackno = ntohl(last_tcp->tcph->AckNO);
	unsigned long long next_expect_tcp_seqno = last_seqno + last_tcp->data_len;
	u_int32 item_seqno = ntohl(item->tcpflow.tcph->SeqNO);
	u_int32 item_ackno = ntohl(item->tcpflow.tcph->AckNO);
	//printf("last_pkt_id %5u next_expect_tcp_seqno %10u last_seqno %10u data_len %5u ,item pkt_id %5u item_seqno %10u cache_num %3u\n",
	//	h->last->pkt_id,next_expect_tcp_seqno,last_seqno,last_tcp->data_len,item->pkt_id,item_seqno,h->cache_num);
	// as is client not send data back!
	if(last_ackno != item_ackno) return 0;
	
	// repeat seq no
	if(last_seqno == item_seqno ) return 0;

	// as is next_expect_tcp_seqno is sorted ok!
	if(next_expect_tcp_seqno > item_seqno ) return 0;
	
	else if(next_expect_tcp_seqno < item_seqno ) {
		tmp = (TCP_FLOW_ITEM*) malloc(sizeof(TCP_FLOW_ITEM));
		TCP_FLOW_ITEM_COPY(tmp,item);
		tcp_flow_data_enqueue(tmp);
	}
	// next expect seq no
	else if( next_expect_tcp_seqno == item_seqno ){
		// because item is in stack ,so copy it into queue.
		tmp = (TCP_FLOW_ITEM*) malloc(sizeof(TCP_FLOW_ITEM));
		TCP_FLOW_ITEM_COPY(tmp,item);

		result[counter++] = tmp;
		TCP_FLOW_ITEM_COPY_FOR_LAST(h->last,item);
		next_expect_tcp_seqno = item_seqno + item->tcpflow.data_len;
	}

	item = h->head;
	for(i = 0 ;i < h->cache_num; ++i){
		if(!item) break;
		
		item_seqno = ntohl(item->tcpflow.tcph->SeqNO);
		
		if(next_expect_tcp_seqno == item_seqno ){
			result[counter++] = item;
			TCP_FLOW_ITEM_COPY_FOR_LAST(h->last,item);
			next_expect_tcp_seqno = item_seqno + item->tcpflow.data_len;
			flv_tcp_data_dequeue(item);
		}
		item = item->next;
	}

	return counter;
}

void tcp_data_callback(TCP_FLOW_ITEM**res)
{
	
}

#define ETH_DATA2ITEM(e,cur) do{\
	\
	}while(0)
	
TCP_FLOW_HEADER*IS_TCP_STREAM(ETH_DATA*e)
{

}

void INIT_TCP_STREAM(ETH_DATA*e)
{

}

void eth_callback(int pkt_id,ETH_DATA*e)
{
	TCP_FLOW_ITEM*result[128];
	TCP_FLOW_ITEM cur;
	TCP_FLOW_HEADER*h;
	ETH_DATA2ITEM(e,cur);
	if((h=IS_TCP_STREAM(e))){
		tcp_stream_recombine(h,&cur,result);
		tcp_data_callback(result);
	}
	else
		INIT_TCP_STREAM(e);
}

int main(int argc,char *argv[])
{	//struct pcap_file_header *file_header;
	struct pcap_pkthdr *pkt_header;
	FramHeader_t *frameh;
	IPHeader_t *ip_header;
	TCPHeader_t *tcp_header;
	
	FILE *fp, *output;
	unsigned long int   pkt_offset ; int i = 0;
	int ip_len,  ip_proto; //http_len,
	int src_port, dst_port, tcp_flags;
	char buf[BUFSIZE], my_time[STRSIZE];
	char src_ip[STRSIZE], dst_ip[STRSIZE];
	char* tcp_data_buf;
	
	//初始化
	ETH_DATA ethdata;
	pkt_header = ethdata.data;

	memset(buf, 0, sizeof(buf));
	if(argc < 2){
		printf("use %s pcapfilename\n",argv[0]);
		return 0;
	}
	char *pcapfilename = argv[1];
	// file size must be smaller than unsigned long int,or integer will overflow
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

	//开始读数据包
	pkt_offset = 24; //pcap文件头结构 24个字节
	while (fseek(fp, pkt_offset, SEEK_SET) == 0) //遍历数据包
	{
		i++;
		//pcap_pkt_header 16 byte
		if (fread(pkt_header, 16, 1, fp) != 1) //读pcap数据包头结构
		{
			printf("\nRead end of pcap file. Total parse %d packets.\n",i - 1);
			break;
		}
        if(pkt_header->caplen < 14 + sizeof(IPHeader_t) || pkt_header->caplen > 0xffff) {
            printf("total parser %d packets.pkt_header->caplen 0x%08x len 0x%08x second %d size %zu %zu\n",i,
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
		frameh = (FramHeader_t*)(pkt_header+1);
		if(fread(frameh,pkt_header->caplen - 16,1,fp)!=1){
			printf("%d,read failed\n", i);
			break;
		}
		if( frameh->FrameType != htons(0x800) ){
			//printf("packet %u is not ipv4 packet! type 0x%04X\n",i,ntohs(frameh.FrameType));
			continue;
		}

		ip_header = (IPHeader_t*)(frameh+1);
		//IP数据报头 20字节 // may cause error!!!
		inet_ntop(AF_INET, (void *)&(ip_header->SrcIP), src_ip, 16);
		inet_ntop(AF_INET, (void *)&(ip_header->DstIP), dst_ip, 16);
		ip_proto = ip_header->Protocol;
		ip_len = ip_header->TotalLen; //IP数据报总长度
		if (ip_proto != 0x06) continue; //判断是否是 TCP 协议
		short unsigned int ip_header_len = ((ip_header->Ver_HLen & 0xf)<<2);
		if(rte_ipv4_frag_pkt_is_fragmented((const struct ipv4_hdr*)ip_header))
			printf("packet_id %d is fragment\n",i);
		//TCP头 20字节
		tcp_header = (TCPHeader_t*)(((char*)ip_header) + ip_header_len);
		short unsigned int tcp_header_len = ((tcp_header->HeaderLen & 0xf0)>>2);
		src_port = ntohs(tcp_header->SrcPort);
		dst_port = ntohs(tcp_header->DstPort);
		tcp_flags = tcp_header->Flags;

		eth_callback(i,&ethdata);

		 memset(&ethdata,0,sizeof(ethdata));
	}// end while
	fclose(fp);
	fclose(output);
	return 0;
}

