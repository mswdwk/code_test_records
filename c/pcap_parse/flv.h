#ifndef _FLV_H
#define _FLV_H

#include "list.h"
#include "queue.h"
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
	uint Headersize; // Headersize
} FLV_HEADER;

typedef struct {
	byte TagType;
	byte DataSize[3];
	byte Timestamp[3];
	uint Reserved;
} FLV_TAG_HEADER;

typedef struct{
	unsigned int prev_tag_size ; // previous tag size
	FLV_TAG_HEADER tag_header;
	// below two fields do not exist in real data;
	void*tag_data;
	unsigned int tag_id ;
}FLV_TAG;

typedef struct FLV_FILE{
	FLV_HEADER flvh;
	unsigned int prev_tag_size ;
	unsigned int prev_tag_id;
	FLV_TAG prev_tag;
	List tag_list;
	//FLV_BODY body;
}FLV_FILE;

typedef struct flv_val
{
	char *val;
	int len;
} flv_val;

//flv的header+previoustagsize
//static unsigned char flv_header_data[] = { 0x46,0x4C,0x56,0x01,0x05,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00 };
  static unsigned char flv_header_data[] = { 0x46,0x4C,0x56,0x01,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00 };

//static flv_val flv_val_header = { (char*)flv_header_data ,13 };

#include "ip_flow.h"


// flv stream list length must less than this
#define MAX_PKT_CACHE_NUM_IN_FLV_STREAM   2048

typedef struct FLV_FLOW_ITEM{
	IP_FLOW tcpflow;
	int flv_offset;
	int pkt_id;
	unsigned char state; // 
	struct FLV_FLOW_ITEM *next;
	struct FLV_FLOW_ITEM *prev;
	int ref_counter ; // reference counter
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
	int recv_data_len; // total receive tcp data len
	void(*process)(void*);
	Queue *flv_pkt_queue; // ordered pakcet queue which are used to prepared  for flv tag data analysis.
	FILE*tcp_log; // record tcp_stream_recombine log 
	FILE*fp; // record flv file data
	FILE*ring_log; 
	FLV_FILE flvfp;
	pthread_t consumer_id ;
	char thread_run; // control thread run or not
	struct ring_buffer *ring_buf; // store flv tag data buffer
}FLV_FLOW_HEADER;


#endif

