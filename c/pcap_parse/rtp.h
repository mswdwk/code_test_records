#ifndef RTP_H
#define RTP_H
#include "basic_data_type.h"

typedef struct RTPHeader_s{
#define _LITTLE_ENDIAN
#ifdef _BIG_ENDIAN
u_int8 ver:2;
u_int8 pad:1;
u_int8 ext:1;
u_int8 csic:4;
u_int8 marker:1;
u_int8 pt:7;	// payload type
unsigned short seq;
int timestamp;
int syncsi;
#else
u_int8 csic:4;
u_int8 ext:1;
u_int8 pad:1;
u_int8 ver:2;
u_int8 pt:7;	// payload type
u_int8 marker:1;
unsigned short seq;
int timestamp;
int syncsi;
#endif
	char data[0];
}RTPHeader_t;//__attribute__((aligned(4)))RTPHeader_t;

#endif

