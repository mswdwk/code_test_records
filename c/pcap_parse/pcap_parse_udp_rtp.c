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
	//u_int32 first_seq_no  = ntohl(h->tcpflow.tcph->SeqNO);
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
	if(!res || num <1) return ;
	int i = 0;
	static int total_writen_len = 0;
	char buf[BUFSIZE] = {0};
	int stream_id = res[0]->tcpflow.stream_id;
	FILE*fp = tcp_stream_table[stream_id].fp;
	struct ring_buffer *ring_buf = tcp_stream_table[stream_id].ring_buf;
	for(i = 0 ; i < num ; ++i){
		ring_buffer_put(ring_buf,res[i]->tcpflow.data,res[i]->tcpflow.data_len);
	}
	int r_data_len = ring_data_len( ring_buf );
	ring_buffer_get(ring_buf,buf,r_data_len);
	fwrite(buf,r_data_len,1,fp);
	total_writen_len += r_data_len;
	if(total_writen_len > 0x4AADC1 && total_writen_len < 0x4AADC1 + 1000){
		pdg("pkt_id %5d l4_data_len %4d total write_len 0x%08X\n",res[0]->pkt_id,r_data_len,total_writen_len);
		dump_print("l4_data", r_data_len, buf);
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
	char tmp_name[64];
	sprintf(tmp_name,"%d_tcp_stream.h264",i);
	tcp_stream_table[i].fp = fopen(tmp_name,"wb+");
	//tcp_unlock(h->lock);
	tcp_stream_num++;
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
	tcp_stream_num--;
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
	TCPHeader_t*tcph = e->tcph ;
	
	//TCP头 20字节
	//u_int16 tcp_header_len = ((tcph->HeaderLen & 0xf0)>>2);
	//u_int16 ip_len = e->ip_len;//rte_be_to_cpu_16(iph->TotalLen); //IP数据报总长度
	//e->l4_data = (((char*)tcph) + tcp_header_len);
	//e->l4_data_len = ip_len - ip_header_len - tcp_header_len;
	//pdg("cap_len %4d iplen %4d ip_header_len %2d tcp_header_len %2d l4_data_len %4d\n",e->ph->caplen,e->ip_len,ip_header_len,tcp_header_len, e->l4_data_len);

	int high_ip ,low_ip;
	unsigned short high_port ,low_port;
	IP_PORT_HEADER2TCP_HIGH_LOW(e->iph, tcph);
	e->tcp_hash_index = mkhash(high_ip,high_port,low_ip,low_port);
	ETH_DATA2ITEM(e,&cur);

	if(tcph->Flags & TH_PUSH) e->tcp_flag_push = 1;
	if(tcph->Flags & TH_FIN) e->tcp_flag_fin = 1;
	if(tcph->Flags & TH_SYN) e->tcp_flag_syn = 1;
	if(tcph->Flags & TH_ACK) e->tcp_flag_ack = 1;
	if(tcph->Flags & TH_RST) e->tcp_flag_rst = 1;
	if(tcph->Flags & TH_URG) e->tcp_flag_urg = 1;
	
	if(e->l4_data_len <= 0 || tcph->SrcPort != htons(30001)) {
		e->from_server = 0;
		return;
	}
	else 
		e->from_server = 1;
	
	if( (h = IS_TCP_STREAM(e) ) && e->from_server ){
		cur.tcpflow.stream_id = h->tcpflow.stream_id;
		if(e->caplen > MAX_TCP_SEGMENT_SIZE)pdg("Atttion: e->caplen %4d too long!\n",e->caplen);
		
		if(e->tcp_flag_ack){
			int count = tcp_stream_recombine(h,&cur,result);
			//pdg("count = %d ring_data_len %u\n",count,ring_data_len(h->ring_buf));
			tcp_data_callback(result,count);
		}
		else if ( e->tcp_flag_fin ){
			struct in_addr ipaddr,dst_ipaddr;
			ipaddr.s_addr = e->iph->SrcIP;
			dst_ipaddr.s_addr = e->iph->DstIP;
			pdg("descruct tcp_stream %u srcip %s:%5u dstip %s:%5u\n",h->tcpflow.stream_id,inet_ntoa(ipaddr),ntohs(tcph->SrcPort),
				inet_ntoa(dst_ipaddr),ntohs(tcph->DstPort));
			tcp_stream_destruct(h->tcpflow.stream_id);
		}
	}
	else
		INIT_TCP_STREAM(e);
}

void IS_SAME_UDP(ETH_DATA*e)
{
	
}

void INIT_UDP()
{
	
}

int write_rtpdata_into_file_byseq(ETH_DATA*e,void*rtp_payload,int rtp_payload_len,int cur_seq)
{
	static u_int16 start_seq = 0,last_seq = 0 ,first_flag = 1;
	char  udprtp_file_name[64] = {0};//udprtp_file_name_2[64] = {0};
	int writelen = 0;
	static FILE*udpfp;
start:	
	if(first_flag == 1){
		start_seq = cur_seq;
		sprintf(udprtp_file_name,"udprtp-last%u_start%u.seq",last_seq,start_seq);
		udpfp = fopen(udprtp_file_name,"wb+");
		first_flag = 0;
		writelen = fwrite(rtp_payload,rtp_payload_len,1,udpfp);
		last_seq = cur_seq;
		return writelen;
	}
	
	if ( cur_seq != last_seq + 1 ){
		pdg("pkt_id %5d cur_seq %5d not equal last_seq %5d + 1 \n",e->pkt_id,cur_seq,last_seq);
		fclose(udpfp);
		//sprintf(udprtp_file_name,"udprtp_seq%u-",start_seq);
		//sprintf(udprtp_file_name_2,"udprtp_seq%u-%u",start_seq,last_seq);
		first_flag = 1;
		goto start;
	}
	
	writelen = fwrite(rtp_payload,rtp_payload_len,1,udpfp);
	last_seq = cur_seq;
	return writelen;
}

static void process_udp_rtp(ETH_DATA*e)
{
	u_int16 cur_seq =0 ;
	//UDPHeader_t *udph	= (UDPHeader_t*)e->udph;
	//IPHeader_t	*iph	= e->iph;
	RTPHeader_t *rtph	= (RTPHeader_t*)e->l4_data;
	int rtp_payload_len = e->l4_data_len - sizeof(RTPHeader_t);
	char *rtp_payload	= rtph->data;
	#if 0
	pdg("rtp payload len %4d,ver %d marker %d pt %3d %02X%02X%02X%02X %02X%02X\n",rtp_payload_len,
		rtph->ver,rtph->marker,	rtph->pt,rtph->data[0]&0xff,rtph->data[1]&0xff,rtph->data[2]&0xff,
										 rtph->data[3]&0xff,rtph->data[4]&0xff,rtph->data[5]&0xff);
	//dump_print("rtpheader",12+6,rtph );
	#endif
	cur_seq = ntohs(rtph->seq);
	if(cur_seq == 0){
		pdg("%d l4_data_len %d caplen %d\n",rtp_payload_len,e->l4_data_len,e->ph->caplen);
		dump_print("iph",20+8+12+6,e->iph );
	}
	write_rtpdata_into_file_byseq(e,rtp_payload,rtp_payload_len,cur_seq);
#if 0
	if( (h = IS_SAME_UDP(e) )){
		cur.tcpflow.stream_id = h->tcpflow.stream_id;
		int count = tcp_stream_recombine(h,&cur,result);
		//pdg("count = %d ring_data_len %u\n",count,ring_data_len(h->ring_buf));
		tcp_data_callback(result,count);
	}
	else
		INIT_UDP(e);
#endif
	
	return ;

}

static void process_udp(ETH_DATA*e)
{
	UDPHeader_t *udph	= (UDPHeader_t*)e->udph;
	//IPHeader_t	*iph	= e->iph;
	RTPHeader_t *rtph	= (RTPHeader_t*)e->l4_data;
	// only process port 30001
	if(udph->SrcPort != htons(30001)) 
		return;
	if( e->l4_data_len >= sizeof(RTPHeader_t) && rtph->ver == 2 )
	 	process_udp_rtp(e);
	return ;
}

static void eth_callback(ETH_DATA*e)
{
	int protocol = e->iph->Protocol;
	IPHeader_t*iph = e->iph;
	int ip_len = ntohs(iph->TotalLen);// this filed maybe wrong ,should use caplen to adjust.
	int ip_header_len = ((iph->Ver_HLen & 0xf)<<2);
	if(ip_len + sizeof(FramHeader_t) != e->ph->caplen){
		pdg("Attention ip_len %d pkt caplen %u ! %zu\n",ip_len,e->ph->caplen,sizeof(FramHeader_t));
		ip_len = e->ph->caplen - sizeof(FramHeader_t);
	}
	e->ip_len = ip_len;
	int high_ip ,low_ip;
	unsigned short high_port ,low_port,tcp_header_len = 0;

	e->udph = (struct udphdr*)(((char*)(e->iph)) + ip_header_len);
	e->tcph = (TCPHeader_t*)(((char*)(e->iph)) + ip_header_len) ;
	UDPHeader_t*udph = (UDPHeader_t*)e->udph;
	
	IP_PORT_HEADER2TCP_HIGH_LOW(iph, udph);
	
	e->l4_hdr = ((char*)(e->iph)) + ip_header_len;
	e->tcp_hash_index = mkhash(high_ip,high_port,low_ip,low_port);
	
	switch(protocol)
	{
		case IP_PROTO_UDP:
			e->l4_data_len = ip_len - ip_header_len - sizeof(UDPHeader_t);
			e->l4_data = e->l4_hdr + sizeof(UDPHeader_t);
			e->tcph = NULL;
			process_udp(e);
			break;
		
		case IP_PROTO_TCP:
			tcp_header_len = ((e->tcph->HeaderLen & 0xf0)>>2);
			e->l4_data_len = ip_len - ip_header_len - tcp_header_len;
			e->l4_data = e->l4_hdr + tcp_header_len;
			e->udph = NULL;
			process_tcp(e);
			break;
		
		default:
			pdg("proto %2d ip_len %4d data_len %4d\n",protocol,e->ip_len,e->l4_data_len);
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
	printf("RTPHeader_t size %zu\n",sizeof(RTPHeader_t));
	FILE *fp, *output;
	unsigned long int   pkt_offset ; int i = 0;
	//int ip_len,  ip_proto; //http_len,
	//int src_port, dst_port, tcp_flags;
	char buf[BUFSIZE], my_time[STRSIZE];
	char src_ip[STRSIZE], dst_ip[STRSIZE];
	
	//初始化
	init_hash();
	init_tcp_stream_table();
	ETH_DATA ethdata;
	memset(&ethdata,0,sizeof(ethdata));
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
			printf("localtime failed. tv_sec = %u. Error %s\n",pkt_header->ts.tv_sec,strerror(errno));	// printf("%d: %s\n", i, my_time);

		//数据帧头 14字节
		frameh = (FramHeader_t*)(pkt_header+1);
		if(fread(frameh,pkt_header->caplen,1,fp)!=1){
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
		//ip_proto = ip_header->Protocol;
		//ip_len = rte_be_to_cpu_16(ip_header->TotalLen); //IP数据报总长度
		//short unsigned int ip_header_len = ((ip_header->Ver_HLen & 0xf)<<2);
		if(rte_ipv4_frag_pkt_is_fragmented((const struct ipv4_hdr*)ip_header))
			printf("packet_id %d is fragment\n",i);
		ethdata.protocol = ip_header->Protocol;
		ethdata.iph = ip_header;
		ethdata.pkt_id = i;
		ethdata.ph = pkt_header;
		ethdata.fh = frameh;
		//ethdata.caplen = pkt_header->caplen;
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
	//fclose(udpfp);
	return 0;
}


