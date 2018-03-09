#ifndef _FLV_H
#define _FLV_H

//Important!
#pragma pack(1)
#define TAG_TYPE_SCRIPT 18
#define TAG_TYPE_AUDIO  8
#define TAG_TYPE_VIDEO  9
typedef unsigned char byte;
typedef unsigned int uint;
typedef struct {
	byte Signature[3];
	byte Version;
	byte Flags;
	uint DataOffset;
} FLV_HEADER;

typedef struct {
	byte TagType;
	byte DataSize[3];
	byte Timestamp[3];
	uint Reserved;
} FLV_TAG_HEADER;

typedef struct flv_val
{
	char *val;
	int len;
} flv_val;

//flv的header+previoustagsize
//static unsigned char flv_header_data[] = { 0x46,0x4C,0x56,0x01,0x05,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00 };
  static unsigned char flv_header_data[] = { 0x46,0x4C,0x56,0x01,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00 };

static flv_val flv_val_header = { (char*)flv_header_data ,13 };

#include "ip_flow.h"


// flv stream list length must less than this
#define MAX_PKT_CACHE_NUM_IN_FLV_STREAM   100

typedef struct FLV_FLOW_ITEM{
	IP_FLOW tcpflow;
	int flv_offset;
	int pkt_id;
	unsigned char state; // 
	struct FLV_FLOW_ITEM *next;
	struct FLV_FLOW_ITEM *prev;
}FLV_FLOW_ITEM;

typedef struct FLV_FLOW_HEADER{
	IP_FLOW tcpflow;
	//char flv_header_flag;
	int flv_offset;
	unsigned int pkt_id;
	FLV_FLOW_ITEM *head;
	FLV_FLOW_ITEM *tail;
	FLV_FLOW_ITEM *last;
	uint last_seqno;
	int flv_flow_pkt_num;
	int cache_num;
	// total receive tcp data len
	int recv_data_len;
	void(*process)(void*);
	FILE*fp;
}FLV_FLOW_HEADER;




#endif

