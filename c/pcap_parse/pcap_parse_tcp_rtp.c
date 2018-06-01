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
//#include "nids.h"

#define MAX_TCP_STREAM_NUM 128

TCP_FLOW_HEADER tcp_stream_table[MAX_TCP_STREAM_NUM];
static  int tcp_stream_num;

static void init_tcp_stream_table(void)
{
	int i;
	memset(tcp_stream_table,0,sizeof(tcp_stream_table));
	for(i = 0 ;i <MAX_TCP_STREAM_NUM ; ++i){
		tcp_stream_table[i].tcpflow.hash = TCP_HASH_INDEX_INVAILD;
	}
}

int create_pkt_mempool(int pkt_num)
{
	void *p = calloc(pkt_num,sizeof(ETH_DATA) + DATA_ROOM_BUF_SIZE);
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
	//memcpy(dst,src,sizeof(TCP_FLOW_ITEM));
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

static void tcp_flow_data_dequeue(TCP_FLOW_ITEM*flow)
{
	if(!flow)return;
	int stream_id = flow->tcpflow.stream_id;
	TCP_FLOW_HEADER*header = &tcp_stream_table[stream_id];
	if(header->cache_num < 0 )return;
	
	if(header->tail == flow) header->tail = flow->prev;
	if(header->head == flow) header->head = flow->next;
	
	// release this node
	if(flow->prev){
		flow->prev->next = flow->next;
	}
	if(flow->next)
		flow->next->prev = flow->prev;
	
	//free(flow);
	if(header->cache_num > 0)
		header->cache_num--;
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
			tcp_flow_data_dequeue(item);
		}
		item = item->next;
	}

	return counter;
}

void tcp_data_callback(TCP_FLOW_ITEM**res)
{
	
}

static inline void ETH_DATA2ITEM(int pkt_id,ETH_DATA*e,TCP_FLOW_ITEM*cur)
{
	cur->pkt_id = pkt_id;
	cur->tcpflow.tcph = e->tcph;
	cur->tcpflow.data = e->tcp_data;
}

static int tcp_stream_construct(int i,ETH_DATA*et)
{
	IP_FLOW *tcp,
	TCPHeader_t*tcph = et->tcph;
	void*data = et->tcp_data;
	int len = et->tcp_data_len;
	int pkt_id = et->pkt_id;
	TCP_FLOW_HEADER*h = &tcp_stream_table[i];
	tcp_lock_init(&h->lock);
	assert(h->lock);
	//tcp_lock(h->lock);
	memcpy(&tcp_stream_table[i].tcpflow,tcp,sizeof(IP_FLOW));
	tcp_stream_table[i].tcpflow.tcph = calloc(1,sizeof(TCPHeader_t));
	memcpy(tcp_stream_table[i].tcpflow.tcph,tcph,sizeof(TCPHeader_t));
	tcp_stream_table[i].tcpflow.data = calloc(1,len);
	memcpy(tcp_stream_table[i].tcpflow.data,data,len);
	
	char tcp_file_name[256];
 	sprintf(tcp_file_name,"%s_%d.tcp",adres2(&h->tcpflow),i);
	tcp_stream_table[i].fp = fopen(tcp_file_name,"wb");
	tcp_stream_table[i].tail = NULL;
	tcp_stream_table[i].head = NULL;
	printf("stream %d sport %d dport %d\n",i,ntohs(h->tcpflow.tcph->SrcPort),ntohs(h->tcpflow.tcph->DstPort));
#if 1
	tcp_stream_table[i].last = calloc(1,sizeof(TCP_FLOW_ITEM));
	if(tcp_stream_table[i].last){
		memcpy(&tcp_stream_table[i].last->tcpflow,tcp,sizeof(IP_FLOW) );
		tcp_stream_table[i].last->tcpflow.tcph = calloc(1,sizeof(TCPHeader_t));
		tcp_stream_table[i].last->tcpflow.data = calloc(1,DATA_ROOM_BUF_SIZE);
		tcp_stream_table[i].last->pkt_id = pkt_id;
		if(tcp_stream_table[i].last->tcpflow.tcph)
			memcpy(tcp_stream_table[i].last->tcpflow.tcph,tcph,sizeof(TCPHeader_t) );
	}
#endif
	char*ch = (void*)data;
	tcp_stream_table[i].recv_data_len = len ;
	tcp_stream_table[i].pkt_id = pkt_id;
	tcp_stream_table[i].last_seqno = ntohl(tcph->SeqNO);
	printf("tcp stream %2u pkt_id %6u seq %10u ack %10u len %4u flag 0x%02x cache_num %3u\n",
		i,pkt_id,ntohl(tcph->SeqNO),ntohl(tcph->AckNO),len,tcph->Flags,tcp_stream_table[i].cache_num);
	
	struct ring_buffer *ring_buf = NULL;

	void * buffer = (void *)malloc(RING_BUFFER_SIZE);
	if (!buffer){
		fprintf(stderr, "Failed to malloc memory.\n");
		return -1;
	}
	ring_buf = ring_buffer_init(buffer, RING_BUFFER_SIZE);
	if (!ring_buf){
		fprintf(stderr, "Failed to init ring buffer.\n");
		return -1;
	}
	tcp_stream_table[i].ring_buf = ring_buf;
	
	char tcp_stream_recombine_file_name[128]={0};
	sprintf(tcp_stream_recombine_file_name,"tcp_stream_%u_recombine.log",i);
	tcp_stream_table[i].tcp_log = fopen(tcp_stream_recombine_file_name,"w+");//////////maybe error

	ring_buffer_put(ring_buf,et->tcp_data,et->tcp_data_len);
	int ret = sprintf(tcp_stream_recombine_file_name,"ring_%s-%u.log",adres2(&h->tcpflow),i);
	tcp_stream_recombine_file_name[ret] = 0;
	FILE* ring_log = fopen(tcp_stream_recombine_file_name,"w+");
	fprintf(ring_log,"pkt %5u put %u \n",pkt_id,len - tcp_offset);
	tcp_stream_table[i].ring_log = ring_log;
	tcp_stream_table[i].stream_last_packet = 0;
	//tcp_unlock(h->lock);
	return 0;
}

static int tcp_stream_destruct(int i)
{
	TCP_FLOW_HEADER*h = &tcp_stream_table[i];
	//tcp_lock(h->lock);
	if(h->tcpflow.tcph)free(h->tcpflow.tcph);

	if(h->fp)fclose(h->fp);

	if(h->last){
		if(h->last->tcpflow.tcph)free(h->last->tcpflow.tcph);
		//h->last->tcpflow.tcph = NULL;
		if(h->last->tcpflow.data)free(h->last->tcpflow.data);
		if(h->last )free(h->last);
	}
	
	if (h->ring_buf){
		ring_buffer_free(h->ring_buf);
		h->ring_buf = NULL;
	}

	if(h->ring_log)fclose(h->ring_log);
	if(h->tcp_log)fclose(h->tcp_log);
	TCP_FLOW_ITEM*tmp = h->head;
	while(tmp){
		TCP_FLOW_FREE(tmp);
		tmp = tmp->next;
	}
	//tcp_unlock(h->lock);
	if(h->lock && h->thread_run)free(h->lock);
	memset(h,0,sizeof(TCP_FLOW_HEADER));
	h->stream_last_packet = 0;
	return 0;
}


TCP_FLOW_HEADER*IS_TCP_STREAM(ETH_DATA*e)
{
	// search tcp stream table;
	int i ;
	for(i = 0; i < MAX_TCP_STREAM_NUM; ++i){
		if ( e->tcp_hash_index != -1 && tcp_stream_table[i].tcpflow.hash == e->tcp_hash_index){
			return &tcp_stream_table[i];
		}
	}
	
	return NULL;
}

void INIT_TCP_STREAM(ETH_DATA*e)
{
	pdg("init %d\n",e->tcp_data[0]);
	int i;
    for(i = 0; i < MAX_TCP_STREAM_NUM; ++i){
		if ( e->tcp_hash_index != TCP_HASH_INDEX_INVAILD && tcp_stream_table[i].tcpflow.hash == TCP_HASH_INDEX_INVAILD){
			tcp_stream_construct(i,e);
			break;
			//return &tcp_stream_table[i];
		}
	}
    //return -1;
}

void eth_callback(int pkt_id,ETH_DATA*e)
{
	TCP_FLOW_ITEM*result[128];
	TCP_FLOW_ITEM cur;
	TCP_FLOW_HEADER*h;
	ETH_DATA2ITEM(pkt_id,e,&cur);
	
	TCPHeader_t*tcph = e->tcph;
	IPHeader_t*iph = e->iph;

	int high_ip ,low_ip;
	unsigned short high_port ,low_port;
	IP_PORT_HEADER2TCP_HIGH_LOW(iph, tcph);
	
	int hash = -1;
	e->tcp_hash_index = mkhash(high_ip,high_port,low_ip,low_port);
	
	if((h=IS_TCP_STREAM(e))){
		int count = tcp_stream_recombine(h,&cur,result);
		pdg("count = %d\n",count);
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
	init_hash();
	init_tcp_stream_table();
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
		ethdata.tcp_data = ((char*)tcp_header+tcp_header_len);
		ethdata.tcph = tcp_header;
		ethdata.iph = ip_header;
		ethdata.tcp_data_len = ip_len - ip_header_len - tcp_header_len;
		ethdata.pkt_id = i;
		eth_callback(i,&ethdata);

		 memset(&ethdata,0,sizeof(ethdata));
	}// end while
	fclose(fp);
	fclose(output);
	return 0;
}

