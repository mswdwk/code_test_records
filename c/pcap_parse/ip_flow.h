#ifndef _IP_FLOW_H
#define _IP_FLOW_H
#include "pcap_parse.h"

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
	FILE*fp; // record flv file data
	FILE*ring_log; 

	pthread_t consumer_id ;
	volatile char thread_run:1; // control thread run or not
	volatile char stream_last_packet:1;// stream last packet
	volatile char thread_finish:1;
	struct ring_buffer *ring_buf; // store flv tag data buffer
}TCP_FLOW_HEADER;

#endif
