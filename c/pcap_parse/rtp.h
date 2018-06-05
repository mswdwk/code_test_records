#ifndef RTP_H
#define RTP_H
#include "basic_data_type.h"

typedef struct RTPHeader_s{
	char ver:2;
	char pad:1;
	char ext:1;
	char csic:4;
	char marker:1;
	char pt:7;	// payload type
	unsigned short seq;
	int timestamp;
	int syncsi;
	char data[0];
}__attribute__((aligned(4)))RTPHeader_t;


#endif

