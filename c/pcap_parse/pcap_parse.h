#ifndef _PCAP_PARSE_H
#define _PCAP_PARSE_H
#include "basic_data_type.h"

#define BUFSIZE 16384 //(1024*16)
#define STRSIZE 1024

//pacp文件头结构体
struct pcap_file_header
{
	int32 magic;       /* 0xa1b2c3d4 */
	u_short version_major;   /* magjor Version 2 */
	u_short version_minor;   /* magjor Version 4 */
	int32 thiszone;      /* gmt to local correction */
	u_int32 sigfigs;     /* accuracy of timestamps */
	u_int32 snaplen;     /* max length saved portion of each pkt */
	u_int32 linktype;    /* data link type (LINKTYPE_*) */
};
//时间戳
struct time_val
{
	u_int32 tv_sec;         /* seconds 含义同 time_t 对象的值 */
	u_int32 tv_usec;        /* and microseconds */
};

//pcap数据包头结构体
struct pcap_pkthdr
{
	struct time_val ts;  /* time stamp */
	u_int32 caplen; /* length of portion present */
	u_int32 len;    /* length this packet (off wire) */
};

//数据帧头
typedef struct FramHeader_t
{ //Pcap捕获的数据帧头
	u_int8 DstMAC[6]; //目的MAC地址
	u_int8 SrcMAC[6]; //源MAC地址
	u_short FrameType;    //帧类型
} FramHeader_t;
//IP数据报头
typedef struct IPHeader_t
{ //IP数据报头
	u_int8 Ver_HLen;       //版本+报头长度
	u_int8 TOS;            //服务类型
	u_int16 TotalLen;       //总长度
	u_int16 ID; //标识
	u_int16 Flag_Segment;   //标志+片偏移
	u_int8 TTL;            //生存周期
	u_int8 Protocol;       //协议类型
	u_int16 Checksum;       //头部校验和
	u_int32 SrcIP; //源IP地址
	u_int32 DstIP; //目的IP地址
} IPHeader_t;

/**
 * IPv4 Header
 */
struct ipv4_hdr {
	uint8_t  version_ihl;		/**< version and header length */
	uint8_t  type_of_service;	/**< type of service */
	uint16_t total_length;		/**< length of packet */
	uint16_t packet_id;		/**< packet ID */
	uint16_t fragment_offset;	/**< fragmentation offset */
	uint8_t  time_to_live;		/**< time to live */
	uint8_t  next_proto_id;		/**< protocol ID */
	uint16_t hdr_checksum;		/**< header checksum */
	uint32_t src_addr;		/**< source address */
	uint32_t dst_addr;		/**< destination address */
} __attribute__((__packed__));

//TCP数据报头
typedef struct TCPHeader_t
{ //TCP数据报头
	u_int16 SrcPort; //源端口
	u_int16 DstPort; //目的端口
	u_int32 SeqNO; //序号
	u_int32 AckNO; //确认号
	u_int8 HeaderLen; //数据报头的长度(4 bit) + 保留(4 bit)
	u_int8 Flags; //标识TCP不同的控制消息
	u_int16 Window; //窗口大小
	u_int16 Checksum; //校验和
	u_int16 UrgentPointer;  //紧急指针
}TCPHeader_t;

typedef struct UDPHeader_s{
	u_int16 SrcPort;
	u_int16 DstPort;
	u_int16 len;
	u_int16 check;
}__attribute__((aligned(4)))UDPHeader_t;

typedef struct IP_PACKET{
	IPHeader_t iph;
	//int len; // data len
	unsigned char data[];
}IP_PACKET;

/**
 * IPv6 Header
 */
typedef struct ipv6_hdr {
	uint32_t vtc_flow;     /**< IP version, traffic class & flow label. */
	uint16_t payload_len;  /**< IP packet length - includes sizeof(ip_header). */
	uint8_t  proto;        /**< Protocol, next header. */
	uint8_t  hop_limits;   /**< Hop limits. */
	uint8_t  src_addr[16]; /**< IP address of source host. */
	uint8_t  dst_addr[16]; /**< IP address of destination host(s). */
} __attribute__((__packed__))IPV6Header_t;

/* TCP flags */

#define TH_FIN                               0x01
#define TH_SYN                               0x02
#define TH_RST                               0x04
#define TH_PUSH                              0x08
#define TH_ACK                               0x10
#define TH_URG                               0x20
/** Establish a new connection reducing window */
#define TH_ECN                               0x40
/** Echo Congestion flag */
#define TH_CWR                               0x80

// short int
#define BigLittleSwap16(A)  ((((uint16_t)(A) & 0xff00) >> 8) | (((uint16_t)(A) & 0x00ff) << 8))

// 长整型大小端互换
#define BigLittleSwap32(A)  ((((uint32_t)(A) & 0xff000000) >> 24) | \
                             (((uint32_t)(A) & 0x00ff0000) >> 8)  | \
                             (((uint32_t)(A) & 0x0000ff00) << 8)  | \
                             (((uint32_t)(A) & 0x000000ff) << 24) )

typedef struct{
	struct pcap_pkthdr *ph;
	FramHeader_t*fh;
	IPHeader_t*iph;
	TCPHeader_t*tcph;
	struct udphdr *udph;
	char*l4_hdr;
	char*l4_data;
	int l4_data_len;
	int pkt_id:23;
	int from_server:1;
	int protocol:8;
	int tcp_hash_index;
#define DATA_ROOM_BUF_SIZE (1518 + sizeof(struct pcap_pkthdr))
	char data[DATA_ROOM_BUF_SIZE];
}ETH_DATA;

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

#define IP_PORT_HEADER2TCP_HIGH_LOW(iph,tcph)		\
	do{ 												\
		if( (iph)->DstIP > (iph)->SrcIP ) { 			\
			high_ip = (iph)->DstIP;				\
			low_ip = (iph)->SrcIP;				\
		}else{											\
			high_ip = (iph)->SrcIP;				\
			low_ip = (iph)->DstIP;				\
		}												\
		\
		if( (tcph)->DstPort > (tcph)->SrcPort ){		\
			high_port = (tcph)->DstPort;			\
			low_port = (tcph)->SrcPort;			\
		}else{											\
			high_port = (tcph)->SrcPort;			\
			low_port = (tcph)->DstPort;			\
		}												\
	}while(0)

#endif
