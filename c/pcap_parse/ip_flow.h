#ifndef _IP_FLOW_H
#define _IP_FLOW_H
#include "pcap_parse.h"

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <time.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>

#define TCP_HASH_INDEX_INVAILD -1

typedef struct{
	int high_ip,low_ip;
	short int high_port,low_port;
	char protocol;
	int stream_id;
	int hash;
	void* data;
	uint data_len;
	uint counter;
	void(*process)(void*);
	TCPHeader_t*tcph;
}IP_FLOW;

typedef struct TCP_FLOW_ITEM_S{
	IP_FLOW tcpflow;
	int flv_offset;
	int pkt_id;
	unsigned char state; // 
	struct TCP_FLOW_ITEM_S *next;
	struct TCP_FLOW_ITEM_S *prev;
	int ref_counter ; // reference counter
}TCP_FLOW_ITEM;

typedef struct TCP_FLOW_HEADER_S{
	LOCK_T *lock;
	IP_FLOW tcpflow;
	int flv_offset;
	unsigned int pkt_id;
	TCP_FLOW_ITEM *head;
	TCP_FLOW_ITEM *tail;
	TCP_FLOW_ITEM *last;
	uint last_seqno;
	int flv_flow_pkt_num;
	int cache_num;
	int recv_data_len; // total receive tcp data len
	void(*process)(void*);
	//Queue *flv_pkt_queue; // ordered pakcet queue which are used to prepared  for flv tag data analysis.
	FILE*tcp_log; // record tcp_stream_recombine log 
	FILE*fp; // record stream data
	FILE*ring_log; 

	pthread_t consumer_id ;
	volatile char thread_run:1; // control thread run or not
	volatile char stream_last_packet:1;// stream last packet
	volatile char thread_finish:1;
	struct ring_buffer *ring_buf; // store flv tag data buffer
}TCP_FLOW_HEADER;

int ip_flow_hash(IP_FLOW*flow);

static inline void tcp_lock_init(LOCK_T**lock)
{
	if( lock && *lock == NULL){
		*lock = (LOCK_T*)malloc(sizeof(LOCK_T));
		pthread_mutex_init(*lock,NULL);
	}
}

static inline void tcp_lock(LOCK_T*lock)
{
	pthread_mutex_lock(lock);
}

static inline void tcp_unlock(LOCK_T*lock)
{
	pthread_mutex_unlock(lock);
}

static inline void TCP_FLOW_FREE(void*data)
{
	if(!data)return;
	TCP_FLOW_ITEM*flow = (TCP_FLOW_ITEM*)data;
	
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

#define IPV4_IHL_MULTIPLIER	(4)
		
		/* Fragment Offset * Flags. */
#define	IPV4_HDR_DF_SHIFT	14
#define	IPV4_HDR_MF_SHIFT	13
#define	IPV4_HDR_FO_SHIFT	3
		
#define	IPV4_HDR_DF_FLAG	(1 << IPV4_HDR_DF_SHIFT)
#define	IPV4_HDR_MF_FLAG	(1 << IPV4_HDR_MF_SHIFT)
		
#define	IPV4_HDR_OFFSET_MASK	((1 << IPV4_HDR_MF_SHIFT) - 1)
		
#define	IPV4_HDR_OFFSET_UNITS	8
		/**
		 * Check if the IPv4 packet is fragmented
		 *
		 * @param hdr
		 *	 IPv4 header of the packet
		 * @return
		 *	 1 if fragmented, 0 if not fragmented
		 */
 #define rte_be_to_cpu_16 ntohs
static inline int
rte_ipv4_frag_pkt_is_fragmented(const struct ipv4_hdr * hdr) {
	uint16_t flag_offset, ip_flag, ip_ofs;

	flag_offset = rte_be_to_cpu_16(hdr->fragment_offset);
	ip_ofs = (uint16_t)(flag_offset & IPV4_HDR_OFFSET_MASK);
	ip_flag = (uint16_t)(flag_offset & IPV4_HDR_MF_FLAG);

	return ip_flag != 0 || ip_ofs  != 0;
}

static inline char *adres2(IP_FLOW *addr)
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

#ifndef IP_PROTO_UDP
#define IP_PROTO_UDP 17
#endif

#ifndef IP_PROTO_TCP
#define IP_PROTO_TCP 6
#endif

static inline void ETH_DATA2ITEM(ETH_DATA*e,TCP_FLOW_ITEM*cur)
{
	cur->pkt_id = e->pkt_id;
	cur->tcpflow.tcph = e->tcph;
	cur->tcpflow.data = e->l4_data;
	cur->tcpflow.data_len = e->l4_data_len;
	cur->tcpflow.hash = e->tcp_hash_index;
}


#endif
