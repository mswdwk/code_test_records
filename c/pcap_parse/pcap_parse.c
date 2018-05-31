// pcap_parser.cpp: 定义控制台应用程序的入口点。
//

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
#include "flv.h"
#include "ring_buffer.h"
#include "dump_print.h"
#include "queue.h"

//
void match_http(FILE *fp, char *head_str, char *tail_str, char *buf, int total_len); //查找 http 信息函数
int record_flv_data(FLV_FLOW_HEADER*h,FLV_TAG*tag,int data_size);
void FLV_FLOW_FREE(void*);

#define MAX_FLV_STREAM_NUM 128

FLV_FLOW_HEADER flv_stream_table[MAX_FLV_STREAM_NUM];

#define GET_FLV_TAG_DATA_SIZE(tag_data_size,ftagheader) do{ \
		tag_data_size  = (ftagheader)->DataSize[0];\
		tag_data_size<<=8;\
		tag_data_size += (ftagheader)->DataSize[1];\
		tag_data_size<<=8;\
		tag_data_size += (ftagheader)->DataSize[2];}while(0)
		
#define IP_PORT_HEADER2FLV_HIGH_LOW(flv,iph,tcph)		\
	do{													\
		if( (iph)->DstIP > (iph)->SrcIP ) {				\
			(flv).high_ip = (iph)->DstIP;				\
			(flv).low_ip = (iph)->SrcIP;				\
		}else{											\
			(flv).high_ip = (iph)->SrcIP;				\
			(flv).low_ip = (iph)->DstIP;				\
		}												\
		\
		if( (tcph)->DstPort > (tcph)->SrcPort ){		\
			(flv).high_port = (tcph)->DstPort;			\
			(flv).low_port = (tcph)->SrcPort;			\
		}else{											\
			(flv).high_port = (tcph)->SrcPort;			\
			(flv).low_port = (tcph)->DstPort;			\
		} 												\
	}while(0)

 char *adres2(IP_FLOW *addr)
{
  static char buf[256];
  memset(buf,0,sizeof(buf));
#if 0
  strcpy (buf, int_ntoa (addr.saddr));
  int ret = sprintf (buf + strlen (buf), "-%d>", addr.source);
  strcat (buf, int_ntoa (addr.daddr));
  ret += sprintf (buf + strlen (buf), "-%d", addr.dest);
  buf[ret] = 0;
#else
  struct in_addr ipaddr;
  ipaddr.s_addr = (addr->high_ip);
  int ret = sprintf (buf , "%s-%d-", inet_ntoa(ipaddr),ntohs(addr->high_port));
  ipaddr.s_addr = addr->low_ip;
  sprintf(buf+ret,"%s-%d",inet_ntoa(ipaddr),ntohs(addr->low_port));
#endif
  return buf;
}

void * consumer_proc(void *arg)
{
	if(!arg){
		printf("error thread arg\n");
		return NULL;
	}
	FLV_FLOW_HEADER*h = (FLV_FLOW_HEADER*)arg;
    struct ring_buffer *ring_buf = h->ring_buf;
    FLV_TAG ftag;
    //FLV_TAG_HEADER *ftagheader;
	uint get_data_len = 0, need_data_len = 0;
	int tag_data_size = 0;
	unsigned int last_prev_tag_size = h->flvfp.prev_tag_size;
	unsigned int prev_tag_size = sizeof(FLV_TAG_HEADER); // current tag size
	unsigned int last_tag_size = 0;//ring_data_buf_len = 0;
	//int offset = 0;
	
	#if 1
	char *tag_data_buf= calloc(1,RING_BUFFER_SIZE);
	h->thread_run = 1;
	const int tag_data_size_max_offset_len = 100; int tag_data_size_i = 0 ; // in order to deal with  prev_tag_size != last_tag_size
    while(ring_buf != NULL && h->thread_run && ring_buf->f_lock)
    {
        //printf("get a flv stream info from ring buffer.\n");
		get_data_len = 0;
		tag_data_size_i = 0;
		need_data_len = sizeof(int) + sizeof(FLV_TAG_HEADER);
		while((ring_data_len(ring_buf) < need_data_len) &&  ! h->stream_last_packet)usleep(100);
		do{
			get_data_len = ring_buffer_try_get(ring_buf, (void *)&ftag, need_data_len);
			prev_tag_size = ntohl(ftag.prev_tag_size);
			tag_data_size_i++;
		}while(prev_tag_size != last_tag_size && tag_data_size_i < tag_data_size_max_offset_len );

		if(tag_data_size_i >= tag_data_size_max_offset_len){
			printf("flv data error:prev_tag_size %u !=  %u last_tag_size. ring_data_len %u\n",prev_tag_size,last_tag_size,ring_data_len(ring_buf));
			dump_print("FLV_TAG_HEADER", get_data_len, &ftag);
			goto end;
		}
		// ignore unvalid data
		ring_buffer_get(ring_buf, (void *)&ftag, --tag_data_size_i);
		//get vaild data
		get_data_len = ring_buffer_get(ring_buf, (void *)&ftag, need_data_len);
		GET_FLV_TAG_DATA_SIZE(tag_data_size,&ftag.tag_header);
		need_data_len = tag_data_size;
		if(tag_data_size >= RING_BUFFER_SIZE){
			printf("tag_data_size %u is >= RING_BUFFER_SIZE %u\n",tag_data_size ,RING_BUFFER_SIZE);
			break;
		}
		while((ring_data_len(ring_buf) < need_data_len) &&  ! h->stream_last_packet)usleep(100);
		get_data_len = ring_buffer_get(ring_buf,( void*)tag_data_buf, need_data_len);
		if(get_data_len != need_data_len)goto end;
		ftag.tag_data = tag_data_buf;
		ftag.tag_id = h->flvfp.prev_tag_id + 1;
		record_flv_data(h,&ftag,tag_data_size);
		//printf("last_prev_tag_size %8u prev_tag_size %8u tag_id %4u tag_data_size %8u\n",last_prev_tag_size,prev_tag_size,ftag.tag_id, tag_data_size);
		
		last_prev_tag_size = prev_tag_size;
		last_tag_size = tag_data_size + sizeof(FLV_TAG_HEADER);
		tag_data_size = 0;
    }
	#else
	//queue_dequeue(h->flv_pkt_queue, &item);
	#endif
end:
	if(tag_data_buf)free(tag_data_buf);
	tag_data_buf = NULL;
	printf("thread_id %zu quit,stream %d\n",pthread_self(),h->tcpflow.stream_id);
	pthread_detach(pthread_self());
	h->thread_finish = 1;
    return (void *)ring_buf;
}

pthread_t consumer_thread(void *arg)
{
    int err;
    pthread_t tid;
    err = pthread_create(&tid, NULL, consumer_proc, arg);
    if (err != 0)
    {
        fprintf(stderr, "Failed to create consumer thread.errno:%u, reason:%s\n",
            errno, strerror(errno));
        return -1;
    }
    printf("consumer thread id %zu\n",tid);
    return tid;
}

int find_flv_header(void*data,int len)
{
    unsigned char *ch = (unsigned char*)data;
    //char *flv_val = flv_val_header.val;
    int i ;
	int flv_ver_flag = htonl(0x464C5601);
    for( i = 0; i < len - 3 ; ++i){
        if( *(int*)ch == flv_ver_flag){
			FLV_HEADER*flv_head = (FLV_HEADER*)ch;
			if((flv_head->Flags&0xfa) != 0x00)	continue;
			if(flv_head->Headersize != htonl(9))	continue;
			int previous_tag_size = *(int*)(flv_head + 1);
			if( 0 == previous_tag_size )
			{
				printf("find 0x464C5601 offset %u\n ",i);
				return i; // maybe return 0 ? may cause problem. Attention!!!
			}else{
				printf("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
					*(ch+4),*(ch+5),*(ch+6),
					flv_header_data[4],flv_header_data[5],flv_header_data[6]);
			}
        }
        ch++;
    }
    
    return -1;
}

int ip_flow_hash(IP_FLOW*flow)
{
	int hash = 0;
	if(!flow)
		return -1;
	int c = (flow->high_port<<16)|flow->low_port;
	hash = rte_jhash_3words(flow->high_ip, flow->low_ip, c,hash);
	flow->hash = hash;
	return hash;
}

// flag means is or not flv header
int record_flv_data(FLV_FLOW_HEADER*h,FLV_TAG*tag,int data_size)
{
	if(!h||!tag || data_size < 0)return -1;
	FILE*fp = h->fp;
	fwrite(&tag->prev_tag_size,sizeof(int),1,fp);
	fwrite(&tag->tag_header,sizeof(FLV_TAG_HEADER),1,fp);
	fwrite(tag->tag_data,data_size,1,fp);
	h->flvfp.prev_tag_id = tag->tag_id;
	h->flvfp.prev_tag_size = data_size + sizeof(FLV_TAG_HEADER);
	#if 0
	FLV_FLOW_ITEM*flow ;
	int i = 0;
	int total_data_len = 0;
	if(tag->tag_id == 1){
		total_data_len += (h->tcpflow.data_len - h->flv_offset);
	} // some continuous tcp flv data len
	while( (flow = (FLV_FLOW_ITEM*)queue_peek(h->flv_pkt_queue)) ) {
		// must be care of the flv header pkt data_len
		total_data_len += (flow->tcpflow.data_len - flow->flv_offset) ;
		
		if((data_size >= total_data_len)){
			// from head start remove list
			queue_dequeue(h->flv_pkt_queue,(void*)&flow);
			if(flow){
				i++;
				int relative_seqno = ntohl(flow->tcpflow.tcph->SeqNO) - ntohl(h->tcpflow.tcph->SeqNO) + 1;
				int next_seqno = relative_seqno + flow->tcpflow.data_len - flow->flv_offset;
		#if 0		
		printf("stream %2u %3u queue size %u,free pkt %5u flag 0x%02x seqno %7u next %7u flv_data_len %4u\n",
				h->tcpflow.stream_id,i,queue_size(h->flv_pkt_queue),flow->pkt_id,flow->tcpflow.tcph->Flags,
				relative_seqno,next_seqno,flow->tcpflow.data_len - flow->flv_offset);
		#endif
			}
			FLV_FLOW_FREE(flow);
		}else
			break;
	}
	#endif
	
	return 0;
}

int record_flv_head(FLV_FILE*f,void*data,int len)
{
	if(!f || !data || len <= 0)return -1;
	FLV_HEADER *fh = &f->flvh;
	memcpy(fh,data,sizeof(FLV_HEADER));
	List*l = &f->tag_list;
	list_init(l,free);
	
	return 0; 
}

void flv_lock_init(LOCK_T**lock)
{
	if( lock && *lock == NULL){
		*lock = (LOCK_T*)malloc(sizeof(LOCK_T));
		pthread_mutex_init(*lock,NULL);
	}
}

void flv_lock(LOCK_T*lock)
{
	pthread_mutex_lock(lock);
}

void flv_unlock(LOCK_T*lock)
{
	pthread_mutex_unlock(lock);
}


int flv_stream_construct(int i,IP_FLOW *flv,TCPHeader_t*tcph,void*data,int len,int flv_offset,int pkt_id)
{
	FLV_FLOW_HEADER*h = &flv_stream_table[i];
	flv_lock_init(&h->lock);
	assert(h->lock);
	//flv_lock(h->lock);
	memcpy(&flv_stream_table[i].tcpflow,flv,sizeof(IP_FLOW));
	flv_stream_table[i].tcpflow.tcph = calloc(1,sizeof(TCPHeader_t));
	memcpy(flv_stream_table[i].tcpflow.tcph,tcph,sizeof(TCPHeader_t));
	flv_stream_table[i].tcpflow.data = calloc(1,len);
	memcpy(flv_stream_table[i].tcpflow.data,data,len);
	
	char flv_file_name[256];
 	sprintf(flv_file_name,"%s_%d.flv",adres2(&h->tcpflow),i);
	flv_stream_table[i].fp = fopen(flv_file_name,"wb");
	flv_stream_table[i].flv_offset = flv_offset;
	flv_stream_table[i].tail = NULL;
	flv_stream_table[i].head = NULL;
	printf("stream %d sport %d dport %d\n",i,ntohs(h->tcpflow.tcph->SrcPort),ntohs(h->tcpflow.tcph->DstPort));
#if 1
	flv_stream_table[i].last = calloc(1,sizeof(FLV_FLOW_ITEM));
	if(flv_stream_table[i].last){
		memcpy(&flv_stream_table[i].last->tcpflow,flv,sizeof(IP_FLOW) );
		flv_stream_table[i].last->tcpflow.tcph = calloc(1,sizeof(TCPHeader_t));
		flv_stream_table[i].last->tcpflow.data = calloc(1,1500);
		flv_stream_table[i].last->pkt_id = pkt_id;
		if(flv_stream_table[i].last->tcpflow.tcph)
			memcpy(flv_stream_table[i].last->tcpflow.tcph,tcph,sizeof(TCPHeader_t) );
	}
#endif
	char*ch = (void*)data;
	flv_stream_table[i].recv_data_len = len ;
	flv_stream_table[i].pkt_id = pkt_id;
	flv_stream_table[i].last_seqno = ntohl(tcph->SeqNO);
	printf("tcp stream %2u pkt_id %6u seq %10u ack %10u len %4u flag 0x%02x cache_num %3u\n",
		i,pkt_id,ntohl(tcph->SeqNO),ntohl(tcph->AckNO),len,tcph->Flags,flv_stream_table[i].cache_num);
	
	struct ring_buffer *ring_buf = NULL;
	pthread_t consumer_pid;

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
	flv_stream_table[i].flv_pkt_queue = malloc(sizeof(Queue));
	if (!flv_stream_table[i].flv_pkt_queue){
		fprintf(stderr, "Failed to init Queue.\n");
		return -1;
	}
	queue_init(flv_stream_table[i].flv_pkt_queue,FLV_FLOW_FREE);
	
	flv_stream_table[i].ring_buf = ring_buf;
	flv_stream_table[i].flvfp.prev_tag_size = 0;
	flv_stream_table[i].flvfp.prev_tag_id = 0;
	flv_stream_table[i].flvfp.tag_list.size = 0;
	
	fwrite(ch + flv_offset,sizeof(FLV_HEADER),1,flv_stream_table[i].fp);
	
	char tcp_stream_recombine_file_name[128]={0};
	sprintf(tcp_stream_recombine_file_name,"tcp_stream_%u_recombine.log",i);
	flv_stream_table[i].tcp_log = fopen(tcp_stream_recombine_file_name,"w+");//////////maybe error

	//queue_enqueue(flv_stream_table[i].flv_pkt_queue, * data);
	FLV_FLOW_HEADER*fh = &flv_stream_table[i];
	consumer_pid = consumer_thread((void*)fh);
	
	flv_stream_table[i].consumer_id = consumer_pid;
	flv_offset += sizeof(FLV_HEADER);
	ring_buffer_put(ring_buf,ch + flv_offset,len - flv_offset);
	int ret = sprintf(tcp_stream_recombine_file_name,"ring_%s-%u.log",adres2(&h->tcpflow),i);
	tcp_stream_recombine_file_name[ret] = 0;
	FILE* ring_log = fopen(tcp_stream_recombine_file_name,"w+");
	fprintf(ring_log,"pkt %5u put %u \n",pkt_id,len - flv_offset);
	flv_stream_table[i].ring_log = ring_log;
	
	flv_stream_table[i].stream_last_packet = 0;
	//flv_unlock(h->lock);
	return 0;
}

int flv_stream_destruct(int i)
{
	FLV_FLOW_HEADER*h = &flv_stream_table[i];
	//flv_lock(h->lock);
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

	if(h->flv_pkt_queue){
		queue_destroy(h->flv_pkt_queue);
		//free(h->flv_pkt_queue);
		h->flv_pkt_queue = NULL;
	}

	if(h->ring_log)fclose(h->ring_log);
	if(h->tcp_log)fclose(h->tcp_log);
	FLV_FLOW_ITEM*tmp = h->head;
	while(tmp){
		FLV_FLOW_FREE(tmp);
		tmp = tmp->next;
	}
	//flv_unlock(h->lock);
	if(h->lock && h->thread_run)free(h->lock);
	memset(h,0,sizeof(FLV_FLOW_HEADER));
	h->stream_last_packet = 0;
	return 0;
}

/*
** @param flv_offset ,flv tage data or header offset from tcp data;
*/

int process_http_flv_stream_header(IPHeader_t*iph,TCPHeader_t*tcph,void*data,int len,int flv_offset,int pkt_id)
{
	IP_FLOW flv ;
	flv.data = data;
	flv.data_len = len;
	flv.protocol = 6;
	flv.counter = 0;
	flv.hash = 0;
	flv.stream_id = -1;
	flv.tcph = tcph;
	/////
	//flv.tcph = (TCPHeader_t*)calloc(sizeof(TCPHeader_t),1);
	//memcpy(flv.tcph,tcph,sizeof(TCPHeader_t));

	IP_PORT_HEADER2FLV_HIGH_LOW(flv,iph,tcph);
	// search flv stream table;
	int i ;
	ip_flow_hash(&flv);
	for( i= 0; i < MAX_FLV_STREAM_NUM; ++i ){
		if (flv_stream_table[i].tcpflow.hash == 0){
			//// !!!!!!!!!!!!!!!!! may cause  memory leak!!!!!!!!!!!!!
			flv.stream_id = i;
			flv_stream_construct(i,&flv,tcph,data,len,flv_offset,pkt_id);
			printf("constuct stream %d pkt_id %d\n",i,pkt_id);
			return i;
		}else if ( flv.hash != 0 && flv.hash == flv_stream_table[i].tcpflow.hash ){
			// 同一条流 第二次 找到 该视频的 flv header
			printf("same flv stream find flv header twice!\n");
			return i;
		}
	}
	return -1;
}

/*
** @return ,return the stream id.
*/

int is_http_flv_stream(int pkt_id,IPHeader_t*iph,TCPHeader_t*tcph)
{
	IP_FLOW flv ;
	flv.protocol = 6;
	flv.counter = 0;
	flv.hash = 0;
	flv.stream_id = -1;
	
	IP_PORT_HEADER2FLV_HIGH_LOW(flv, iph, tcph);
	ip_flow_hash(&flv);

	// search flv stream table;
	int i ;
	FLV_FLOW_HEADER*h;
	for(i = 0; i < MAX_FLV_STREAM_NUM; ++i){
		if ( flv.hash != 0 && flv_stream_table[i].tcpflow.hash == flv.hash){
			if(tcph->Flags & TH_FIN && flv_stream_table[i].thread_run){
				flv_stream_table[i].stream_last_packet = 1;
				flv_stream_table[i].thread_run = 0;
				h = &flv_stream_table[i];
				printf("packet id %u send fin flag,close flv stream %d %d,sport %d dport %d\n",pkt_id,i,h->tcpflow.stream_id,
					ntohs(tcph->SrcPort),ntohs(tcph->DstPort));
				dump_print("destruct stream",ntohs(iph->TotalLen),(void*)iph);
				if(h->consumer_id)pthread_join(flv_stream_table[i].consumer_id,NULL);
				if(h->consumer_id && !h->thread_run && h->stream_last_packet == 1)flv_stream_destruct(i);
				return -1;
			}
			return i;
		}
	}
	
	return -1;
}

int process_http_flv_stream(int stream_id,TCPHeader_t*tcph,void*data,int data_len,int pkt_id)
{
	if(stream_id >= MAX_FLV_STREAM_NUM || stream_id < 0)	return -1;
	if(!data)	return -2;
	if(data_len < 0)	return -3;
	
	FLV_FLOW_HEADER*header = &flv_stream_table[stream_id];
	header->tcpflow.counter ++;
	header->flv_flow_pkt_num++;
	FLV_FLOW_ITEM item ;
	memcpy(&item.tcpflow,&header->tcpflow,sizeof(IP_FLOW));
	item.tcpflow.tcph = tcph;
	item.tcpflow.data = data;
	item.tcpflow.data_len = data_len;
	item.flv_offset = 0;
	item.pkt_id	= pkt_id;
	item.prev  = NULL;
	//item.prev = flow_header->tail;
	item.next = NULL;
	//flow_header->tail = item;
	
	if( header->process ) header->process(&item);
	
	return 0;
}

void FLV_FLOW_FREE(void*data)
{
	if(!data)return;
	FLV_FLOW_ITEM*flow = (FLV_FLOW_ITEM*)data;
	
	if(flow->tcpflow.data)free(flow->tcpflow.data);
	if(flow->tcpflow.tcph)free(flow->tcpflow.tcph);
	
	// release this node
	if(flow->prev){
		flow->prev->next = flow->next;
	}
	if(flow->next)
		flow->next->prev = flow->prev;
	//flow->refcnt--;
	if(flow)free(flow);
}

void flv_tcp_data_dequeue(FLV_FLOW_ITEM*flow)
{
	if(!flow)return;
	int stream_id = flow->tcpflow.stream_id;
	FLV_FLOW_HEADER*header = &flv_stream_table[stream_id];
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

FLV_FLOW_ITEM*flv_tcp_data_enqueue(FLV_FLOW_ITEM*flow)
{
	if(!flow)return NULL;
	int stream_id = flow->tcpflow.stream_id;
	FLV_FLOW_HEADER*header = &flv_stream_table[stream_id];
	#if 0
	if(header->cache_num >= MAX_PKT_CACHE_NUM_IN_FLV_STREAM){
		printf("tcpflow cache is full! pkt_id %6u seqno %10u\n",flow->pkt_id,ntohl(flow->tcpflow.tcph->SeqNO));
		return NULL;
	}
	#endif
	
	header->cache_num++;
	flow->next = NULL;
	flow->prev = header->tail;
	if(!header->head)header->head = flow;
	if(header->tail)header->tail->next = flow;
	header->tail = flow;

	return flow;
}

int FLV_FLOW_ITEM_COPY(FLV_FLOW_ITEM*dst,FLV_FLOW_ITEM*src) 
{
	if(!dst||!src)return -1;
	
	memcpy(dst,src,sizeof(FLV_FLOW_ITEM));
	
	dst->tcpflow.tcph = (TCPHeader_t*)malloc(sizeof(TCPHeader_t));
	memcpy(dst->tcpflow.tcph,src->tcpflow.tcph,sizeof(TCPHeader_t));

	dst->tcpflow.data = (void*)malloc(src->tcpflow.data_len);
	if(src->tcpflow.data && src->tcpflow.data_len <= 1500){
		memcpy(dst->tcpflow.data,src->tcpflow.data,src->tcpflow.data_len);
	}
	return 0;
}

int FLV_FLOW_ITEM_COPY_FOR_LAST(FLV_FLOW_ITEM*dst,FLV_FLOW_ITEM*src) 
{
	if(!dst||!src)return -1;
	// be carefull about pointer copy!
	//memcpy(dst,src,sizeof(FLV_FLOW_ITEM));
	void *data = dst->tcpflow.data;
	void*tcph = dst->tcpflow.tcph;
	memcpy(dst,src,sizeof(FLV_FLOW_ITEM));
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


static int tcp_stream_recombine(FLV_FLOW_HEADER*h,FLV_FLOW_ITEM*item,FLV_FLOW_ITEM**result)
{
	int i = 0,counter = 0;
	if(!h || !item) return 0;
	if(!h->last) return 0;
	IP_FLOW*last_tcp = &h->last->tcpflow;
	if(!last_tcp->tcph) return 0;
	FLV_FLOW_ITEM*tmp = NULL;
	
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
		tmp = (FLV_FLOW_ITEM*) malloc(sizeof(FLV_FLOW_ITEM));
		FLV_FLOW_ITEM_COPY(tmp,item);
		flv_tcp_data_enqueue(tmp);
	}
	// next expect seq no
	else if( next_expect_tcp_seqno == item_seqno ){
		// because item is in stack ,so copy it into queue.
		tmp = (FLV_FLOW_ITEM*) malloc(sizeof(FLV_FLOW_ITEM));
		FLV_FLOW_ITEM_COPY(tmp,item);

		result[counter++] = tmp;
		FLV_FLOW_ITEM_COPY_FOR_LAST(h->last,item);
		next_expect_tcp_seqno = item_seqno + item->tcpflow.data_len;
	}

	item = h->head;
	for(i = 0 ;i < h->cache_num; ++i){
		if(!item) break;
		
		item_seqno = ntohl(item->tcpflow.tcph->SeqNO);
		
		if(next_expect_tcp_seqno == item_seqno ){
			//tmp = (FLV_FLOW_ITEM*) malloc(sizeof(FLV_FLOW_ITEM));
			//FLV_FLOW_ITEM_COPY(tmp,item);
			result[counter++] = item;
			FLV_FLOW_ITEM_COPY_FOR_LAST(h->last,item);
			next_expect_tcp_seqno = item_seqno + item->tcpflow.data_len;
			flv_tcp_data_dequeue(item);
		}
		item = item->next;
	}

	return counter;
}

void flv_stream_process(void*data)
{
	FLV_FLOW_ITEM*flow = (FLV_FLOW_ITEM*)data;
	//int data_len = flow->tcpflow.data_len;
	int flv_offset = flow->flv_offset;
	TCPHeader_t*tcph = flow->tcpflow.tcph;
	int stream_id = flow->tcpflow.stream_id;
	FLV_FLOW_HEADER*header = &flv_stream_table[stream_id];
	//printf("stream_id %u sport %d offset %d\n",stream_id,tcph->SrcPort,flv_offset);
	if(ntohs(tcph->SrcPort) == 80 && flv_offset == 0){
		//printf("tcp seq %10u ack %10u len %4u cache_num %3u\n",ntohl(tcph->SeqNO),ntohl(tcph->AckNO),data_len,header->cache_num);
		FLV_FLOW_ITEM*recombine[MAX_PKT_CACHE_NUM_IN_FLV_STREAM],*tmp;
		int recombine_num = 0;
		recombine_num = tcp_stream_recombine(header,flow,recombine);
		int i = 0 ;
		for( i = 0 ; i < recombine_num ; ++i){
			tmp = recombine[i];
			if( tmp && tmp->tcpflow.data && tmp->tcpflow.data_len <= 1500 ){
				flv_offset = tmp->flv_offset;
				int data_len = tmp->tcpflow.data_len;
				header->recv_data_len += data_len ;//- flv_offset;
				//fwrite(tmp->tcpflow.data + flv_offset,data_len - flv_offset,1,header->fp);
				ring_buffer_put(header->ring_buf,tmp->tcpflow.data + flv_offset,data_len - flv_offset);
				unsigned int relative_seq = ntohl(tmp->tcpflow.tcph->SeqNO) - ntohl(header->tcpflow.tcph->SeqNO) + 1;
				int *data_tmp = (int*)tmp->tcpflow.data;
				fprintf(header->tcp_log,"pkt_id %5u seqno %7u tcp_data_len %4u offset %3u 0x%08X..\n",
					tmp->pkt_id,relative_seq,tmp->tcpflow.data_len,tmp->flv_offset,*data_tmp);
				fprintf(header->ring_log,"pkt %5u flag 0x%02x put %u \n",tmp->pkt_id,tmp->tcpflow.tcph->Flags,data_len - flv_offset);
				#if 0
				if(*data_tmp == 0 ){
					dump_print("pkt flv data error", data_len, tmp->tcpflow.data);
					exit(0);
				}
				queue_enqueue(header->flv_pkt_queue,(void*) tmp);
				// because function flv_record_data will use this recombine data  ,so do not free them right now
				//FLV_FLOW_FREE(tmp);
				#endif
				// because tmp pkt data are write into ring_buf!!! now
				FLV_FLOW_FREE(tmp);
			}
		}
	}
}

void print_flv_stream_info(void)
{
	int i ;
	char high_ip[16],low_ip[16];
	
	printf("stream_id hash  high_ip       low_ip     high_port low_port pkt_num cache_num\n");
	printf("-----------------------------------------------------------------------------\n");
	for(i = 0 ; i < sizeof(flv_stream_table)/sizeof(flv_stream_table[0] ); ++i){
		FLV_FLOW_HEADER*head = &flv_stream_table[i];
		if(0 == head->tcpflow.hash)continue;
		IP_FLOW*iph = &head->tcpflow;
		inet_ntop(AF_INET, (void *)&(iph->high_ip), high_ip, 16);
		inet_ntop(AF_INET, (void *)&(iph->low_ip), low_ip, 16);
		//TCPHeader_t*tcph = head->tcpflow.tcph;
		if(iph)
			printf("%2u 0x%08X %15s %15s %5u %5u %6u %3u\n",i,head->tcpflow.hash,
			high_ip,low_ip,ntohs(iph->high_port),ntohs(iph->low_port),head->flv_flow_pkt_num,
			head->cache_num);
		memset(high_ip,0,sizeof(high_ip));
		memset(low_ip,0,sizeof(low_ip));
	}
}

void init_flv_stream(void)
{
	memset(flv_stream_table,0,sizeof(flv_stream_table));
	int i ;
	for(i = 0 ; i < sizeof(flv_stream_table)/sizeof(flv_stream_table[0] ); ++i){
		flv_stream_table[i].process = flv_stream_process;
		//lock_init(flv_stream_table[i].lock) ;
	}
}

void free_flv_stream(void)
{
	int i ;
	for(i = 0 ; i < sizeof(flv_stream_table)/sizeof(flv_stream_table[0] ); ++i){
		FLV_FLOW_HEADER *h = &flv_stream_table[i];
		if(h->consumer_id ==0 && h->tcpflow.hash == 0 ) continue;

		FLV_FLOW_ITEM *it = h->head;
		if(h->last)FLV_FLOW_FREE(h->last);
		
		for ( ; it ;it = it->next){
			FLV_FLOW_FREE(it);
		}
		
		if(h->tcpflow.data) free(h->tcpflow.data);
		if(h->tcpflow.tcph) free(h->tcpflow.tcph);
		if(h->thread_run == 1 && !h->stream_last_packet){
			h->stream_last_packet = 1;
			h->thread_run = 0;
			if(h->consumer_id)pthread_join(h->consumer_id,NULL);
			//h->consumer_id = 0;
			//flv_stream_destruct( i);
		}
		if(h->thread_finish){
			if(h->ring_buf){
				ring_buffer_free(h->ring_buf);
			}
			if(h->fp)fclose(h->fp);
			if(h->tcp_log)fclose(h->tcp_log);
			if(h->ring_log)fclose(h->ring_log);
			if(h->flv_pkt_queue){
				queue_destroy(h->flv_pkt_queue);
				free(h->flv_pkt_queue);
			}
		}
	}
}
#define MAX_MTU_SIZE 65536

int main(int argc,char *argv[])
{
	struct pcap_file_header *file_header;
	struct pcap_pkthdr *pkt_header;
	IPHeader_t *ip_header;
	TCPHeader_t *tcp_header;
	FILE *fp, *output;
	unsigned long int   pkt_offset ; int i = 0;
	int ip_len,  ip_proto; //http_len,
	int src_port, dst_port, tcp_flags;
	char buf[BUFSIZE], my_time[STRSIZE];
	char src_ip[STRSIZE], dst_ip[STRSIZE];
	//char  host[STRSIZE], uri[BUFSIZE];
	
	init_flv_stream();
	//初始化
	file_header = (struct pcap_file_header *)malloc(sizeof(struct pcap_file_header));
	pkt_header = (struct pcap_pkthdr *)malloc(sizeof(struct pcap_pkthdr));
	ip_header = (IPHeader_t *)malloc(sizeof(IPHeader_t) + MAX_MTU_SIZE);
	//tcp_header = (TCPHeader_t *)malloc(sizeof(TCPHeader_t));
	char* tcp_data_buf;
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
    //pthread_t tcp_consumer_tid;
    //pthread_create(&tcp_consumer_tid,NULL,tcp_consumer_pthread,NULL);
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
		//fseek(fp, 14, SEEK_CUR); //忽略数据帧头
		
		FramHeader_t frameh;
		fread(&frameh,sizeof(FramHeader_t),1,fp);
		if( frameh.FrameType != htons(0x800) ){
			//printf("packet %u is not ipv4 packet! type 0x%04X\n",i,ntohs(frameh.FrameType));
			continue;
		}
		
		//IP数据报头 20字节 // may cause error!!!
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
		short unsigned int ip_header_len = ((ip_header->Ver_HLen & 0xf)<<2);
		if(ip_header_len > 20)fseek(fp, ip_header_len - 20, SEEK_CUR); //ignore ip option segment
		if(rte_ipv4_frag_pkt_is_fragmented((const struct ipv4_hdr*)ip_header))
			printf("packet_id %d is fragment\n",i);
		tcp_header = (TCPHeader_t*)(((char*)ip_header) + ip_header_len);
		//TCP头 20字节
		if (fread(tcp_header, sizeof(TCPHeader_t), 1, fp) != 1)
		{
			printf("%d: can not read tcp_header\n", i);
			break;
		}
        
		short unsigned int tcp_header_len = ((tcp_header->HeaderLen & 0xf0)>>2);
		src_port = ntohs(tcp_header->SrcPort);
		dst_port = ntohs(tcp_header->DstPort);
		tcp_flags = tcp_header->Flags;
		
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
                //char tcp_data_buf[65536] = {0};//= calloc(1,tcp_data_len);
                tcp_data_buf = (char*)tcp_header + tcp_header_len;
				// ignore tcpheader options
				if(tcp_header_len > 20 )fseek(fp, tcp_header_len - 20, SEEK_CUR);
				if(0 == tcp_data_len)continue;
				if(tcp_data_len > MAX_MTU_SIZE ){printf("pkt_id %d tcp_data_len %d > %d\n",i,tcp_data_len,MAX_MTU_SIZE);break;}
                fread(tcp_data_buf,tcp_data_len,1,fp);
				int http_flv_stream_id = -1;
				int found_flv_header = -1;
				
				if(-1 != (http_flv_stream_id = is_http_flv_stream(i,ip_header,tcp_header)))
					process_http_flv_stream(http_flv_stream_id,tcp_header,tcp_data_buf,tcp_data_len,i);
				else
                	found_flv_header = find_flv_header(tcp_data_buf,tcp_data_len);
                
                sprintf(buf,"[%s]%5u flag 0x%02x seq %10u ack %10u tcp_data_len %5u %15s:%5u -> %15s:%5u flv %d\n",my_time,
                    i,0xff&tcp_flags,ntohl(tcp_header->SeqNO),ntohl(tcp_header->AckNO),tcp_data_len,src_ip, src_port, dst_ip, dst_port,found_flv_header);
                fwrite(buf, strlen(buf), 1, output);
                memset(buf,0,sizeof(buf));
				
                if(found_flv_header != -1){
                    http_flv_stream_id = process_http_flv_stream_header(ip_header,tcp_header,tcp_data_buf,tcp_data_len,found_flv_header,i);
					//process_http_flv_stream(http_flv_stream_id,tcp_header,tcp_data_buf,tcp_data_len);
					//FLV_TAG_HEADER *flv_tag = tcp_data_buf+found_flv_header+
					//	sizeof(FLV_HEADER);
					printf("packet %d contains flv header ,tcp data offset %u\n",i,found_flv_header);
                }
             }
         #endif
		 memset(ip_header,0,sizeof(*ip_header));
	} // end while
	fclose(fp);
	fclose(output);
    free(file_header);
    free(pkt_header);
    free(ip_header);
    //free(tcp_header);

	// maybe data in ring_buf not complete process 
	free_flv_stream();
	
	print_flv_stream_info();
    //pthread_join(tcp_consumer_tid,NULL);
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

