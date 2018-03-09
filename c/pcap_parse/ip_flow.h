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

#endif
