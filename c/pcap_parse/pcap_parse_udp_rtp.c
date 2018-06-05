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
#include "hash.h"
#include "rtp.h"

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
	//void *p = calloc(pkt_num,sizeof(ETH_DATA) + DATA_ROOM_BUF_SIZE);
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
	u_int32 first_seq_no  = ntohl(h->tcpflow.tcph->SeqNO);
	//u_int32 first_ack_no = ntohl(h->tcpflow.tcph->AckNO);
	//pdg("seq %u ack%u\n",first_seq_no,first_ack_no);
 	u_int32 last_seqno = ntohl(last_tcp->tcph->SeqNO);          //    last_seqno = h->last_seqno;
	u_int32 last_ackno = ntohl(last_tcp->tcph->AckNO);
	unsigned long long next_expect_tcp_seqno = last_seqno + last_tcp->data_len;
	u_int32 item_seqno = ntohl(item->tcpflow.tcph->SeqNO);
	u_int32 item_ackno = ntohl(item->tcpflow.tcph->AckNO);
	#if 0
	printf("last_pkt_id %5u next_expect_tcp_seqno %10u last_seqno %10u data_len %5u ,item pkt_id %5u item_seqno %10u cache_num %3u\n",
		h->last->pkt_id,next_expect_tcp_seqno,last_seqno,last_tcp->data_len,item->pkt_id,item_seqno,h->cache_num);
	#else
	/*printf("last_pkt_id %5u next_expect_tcp_seqno %llu last_seqno %10u data_len %5u ,item pkt_id %5u item_seqno %10u cache_num %3u\n",
		h->last->pkt_id,next_expect_tcp_seqno - first_seq_no,last_seqno -first_seq_no,last_tcp->data_len,item->pkt_id,item_seqno - first_seq_no,h->cache_num);
	*/
	#endif
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

void tcp_data_callback(TCP_FLOW_ITEM**res,int num)
{
	if(!res || num <1)
		return ;
	int i = 0;
	char buf[BUFSIZE];
	int stream_id = res[0]->tcpflow.stream_id ;
	struct ring_buffer *ring_buf = tcp_stream_table[stream_id].ring_buf;
	for(i = 0 ; i < num ; ++i){
		ring_buffer_put(ring_buf,res[i]->tcpflow.data,res[i]->tcpflow.data_len);
		ring_buffer_get(ring_buf,res[i]->tcpflow.data,res[i]->tcpflow.data_len);
	}
}

static int tcp_stream_construct(int i,ETH_DATA*et)
{
	TCPHeader_t*tcph = et->tcph;
	int len = et->l4_data_len;
	int pkt_id = et->pkt_id;
	TCP_FLOW_HEADER*h = &tcp_stream_table[i];
	tcp_lock_init(&h->lock);
	assert(h->lock);
	//tcp_lock(h->lock);
	//memcpy(&tcp_stream_table[i].tcpflow,tcp,sizeof(IP_FLOW));
	tcp_stream_table[i].tcpflow.hash = et->tcp_hash_index;
	tcp_stream_table[i].tcpflow.stream_id = i;
	tcp_stream_table[i].tcpflow.tcph = calloc(1,sizeof(TCPHeader_t));
	tcp_stream_table[i].tcpflow.data_len = len;
	memcpy(tcp_stream_table[i].tcpflow.tcph,tcph,sizeof(TCPHeader_t));
#if 0	
	tcp_stream_table[i].tcpflow.data = calloc(1,len);
	memcpy(tcp_stream_table[i].tcpflow.data,data,len);
	char tcp_file_name[256];
 	sprintf(tcp_file_name,"%s_%d.tcp",adres2(&h->tcpflow),i);
	tcp_stream_table[i].fp = fopen(tcp_file_name,"wb");
#endif
	tcp_stream_table[i].tail = NULL;
	tcp_stream_table[i].head = NULL;
	//printf("stream %d sport %d dport %d\n",i,ntohs(h->tcpflow.tcph->SrcPort),ntohs(h->tcpflow.tcph->DstPort));
#if 1
	tcp_stream_table[i].last = calloc(1,sizeof(TCP_FLOW_ITEM));
	if(tcp_stream_table[i].last){
		//memcpy(&tcp_stream_table[i].last->tcpflow,tcp,sizeof(IP_FLOW) );
		tcp_stream_table[i].last->tcpflow.tcph = calloc(1,sizeof(TCPHeader_t));
		tcp_stream_table[i].last->tcpflow.data = calloc(1,DATA_ROOM_BUF_SIZE);
		tcp_stream_table[i].last->tcpflow.data_len = len;
		tcp_stream_table[i].last->pkt_id = pkt_id;
		if(tcp_stream_table[i].last->tcpflow.tcph)
			memcpy(tcp_stream_table[i].last->tcpflow.tcph,tcph,sizeof(TCPHeader_t) );
	}
#endif
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
	ring_buffer_put(ring_buf,et->l4_data,et->l4_data_len);
	
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
		if ( e->tcp_hash_index != TCP_HASH_INDEX_INVAILD && tcp_stream_table[i].tcpflow.hash == e->tcp_hash_index){
			return &tcp_stream_table[i];
		}
	}
	
	return NULL;
}

void INIT_TCP_STREAM(ETH_DATA*e)
{
	int i;
    for(i = 0; i < MAX_TCP_STREAM_NUM; ++i){
		if ( e->tcp_hash_index != TCP_HASH_INDEX_INVAILD && tcp_stream_table[i].tcpflow.hash == TCP_HASH_INDEX_INVAILD){
			pdg("init tcp stream %d,data[0]%d\n",i,e->l4_data[0]);
			tcp_stream_construct(i,e);
			break;
			//return &tcp_stream_table[i];
		}
	}
    //return -1;
}

static void process_tcp(ETH_DATA*e)
{
	TCP_FLOW_ITEM*result[128];
	TCP_FLOW_ITEM cur;
	TCP_FLOW_HEADER*h;
	IPHeader_t*iph = e->iph;
	int ip_header_len = ((iph->Ver_HLen & 0xf)<<2);
	TCPHeader_t*tcph = (TCPHeader_t*)(((char*)iph) + ip_header_len);
	
	//TCP头 20字节
	u_int16 tcp_header_len = ((tcph->HeaderLen & 0xf0)>>2);
	u_int16 ip_len = rte_be_to_cpu_16(iph->TotalLen); //IP数据报总长度
	e->l4_data = (((char*)tcph)+tcp_header_len);
	e->tcph = tcph;
	//e.l4_hdr = tcph;
	e->l4_data_len = ip_len - ip_header_len - tcp_header_len;
	//pdg("cap_len %d iplen %d ip_header_len %d tcp_header_len %d tcp_data_len %d\n",pkt_header->caplen,ip_len,ip_header_len,tcp_header_len, ethdata.tcp_data_len);

	int high_ip ,low_ip;
	unsigned short high_port ,low_port;
	IP_PORT_HEADER2TCP_HIGH_LOW(iph, tcph);

	e->tcp_hash_index = mkhash(high_ip,high_port,low_ip,low_port);
	ETH_DATA2ITEM(e,&cur);

	if(e->l4_data_len == 0 || e->tcph->SrcPort != htons(30001)) {
		e->from_server = 0;
		return;
	}
	else 
		e->from_server = 1;
	
	if( (h = IS_TCP_STREAM(e) )){
		cur.tcpflow.stream_id = h->tcpflow.stream_id;
		int count = tcp_stream_recombine(h,&cur,result);
		//pdg("count = %d ring_data_len %u\n",count,ring_data_len(h->ring_buf));
		tcp_data_callback(result,count);
	}
	else
		INIT_TCP_STREAM(e);
}

static void process_udp(ETH_DATA*e)
{
	UDPHeader_t*udph = e->udph;
	IPHeader_t*iph = e->iph;
	RTPHeader_t*rtph = (RTPHeader_t*)e->l4_data;
	int rtp_len = e->l4_data_len - sizeof(RTPHeader_t);
	pdg("rtp payload len %d\n",rtp_len);
	
	
	return ;
}

static void eth_callback(ETH_DATA*e)
{
	int protocol = e->iph->Protocol;
	IPHeader_t*iph = e->iph;
	int ip_len = ntohs(iph->TotalLen);
	int ip_header_len = ((iph->Ver_HLen & 0xf)<<2);
	int high_ip ,low_ip;
	unsigned short high_port ,low_port;

	e->udph = (struct udphdr*)(((char*)(e->iph)) + ip_header_len);
	e->tcph = (TCPHeader_t*)(((char*)(e->iph)) + ip_header_len) ;
	UDPHeader_t*udph = e->udph;
	
	IP_PORT_HEADER2TCP_HIGH_LOW(iph, udph);
	e->l4_data_len = ip_len - ip_header_len;
	e->l4_hdr = ((char*)(e->iph)) + ip_header_len;
	e->tcp_hash_index = mkhash(high_ip,high_port,low_ip,low_port);
	
	switch(protocol)
	{
		case IP_PROTO_UDP:
			process_udp(e);
			break;
		
		case IP_PROTO_TCP:
			process_tcp(e);
			break;
		
		default:
			pdg("proto %d data_len %d\n",protocol,e->l4_data_len);
			return ;
	}

}

void record_tcp_stream_data(void)
{
	int i;
	for(i = 0 ;i <MAX_TCP_STREAM_NUM ; ++i){
		TCP_FLOW_HEADER *h  = &tcp_stream_table[i];
		if(h->tcpflow.hash != TCP_HASH_INDEX_INVAILD){
			struct ring_buffer*ring_buf = h->ring_buf;
			char name[64] = {0};
			sprintf(name,"tcp_stream_%u",i);
			FILE*fp = fopen(name,"wb+");
			if(fp){
				fwrite(ring_buf->buffer,ring_data_len(ring_buf),1,fp);
				fclose(fp);
			}
		}
	}
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
	//char* tcp_data_buf;
	
	//初始化
	init_hash();
	init_tcp_stream_table();
	ETH_DATA ethdata;
	pkt_header = (struct pcap_pkthdr *)ethdata.data;

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
	if ((output = fopen("output.txt", "wb+ ")) == NULL)
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
		ip_len = rte_be_to_cpu_16(ip_header->TotalLen); //IP数据报总长度
		
		short unsigned int ip_header_len = ((ip_header->Ver_HLen & 0xf)<<2);
		if(rte_ipv4_frag_pkt_is_fragmented((const struct ipv4_hdr*)ip_header))
			printf("packet_id %d is fragment\n",i);
		ethdata.protocol = ip_header->Protocol;
		ethdata.iph = ip_header;
		ethdata.pkt_id = i;
		eth_callback(&ethdata);
		memset(&ethdata,0,sizeof(ethdata));
	}// end while
#if 0
	struct ring_buffer *ring_buf = tcp_stream_table[0].ring_buf;
	fwrite(ring_buf->buffer,ring_data_len(ring_buf),1,output);
#endif	
	//record_tcp_stream_data();

	fclose(fp);
	fclose(output);
	return 0;
}


