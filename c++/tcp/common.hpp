#ifndef _COMM_H
#define _COMM_H
#ifdef __cplusplus
	extern "C"{
#endif
#define SERVER_PORT 6666

typedef enum e_command{
	HELP,
	COMPILE_REQ, 
	COMPILE_RES,
	UNKNOWN
} Command;

typedef struct s_com_data{
	Command cmd;
	unsigned int len;
	unsigned char* data;
} COM_DATA;

#ifdef __cplusplus
	}	
#endif

#endif
