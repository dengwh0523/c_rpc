#ifndef __RMI_H__
#define __RMI_H__

#include <stdio.h>
#include <string.h>
#include "thread.h"

#if 0
#ifdef _MSC_VER /* Windows平台版本 */
	#ifdef RMI_LIB /* 编译生成静态库(LIB) */
		#define RMI_API 
	#else /* 编译生成动态库(DLL) */
		#ifdef RMI_EXPORTS
			#define RMI_API __declspec(dllexport)
		#else
			#define RMI_API __declspec(dllimport)
		#endif
	#endif
#else /* Linux平台版本 */
	#define RMI_API 
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG

#define RMI_VERSION				"1.0.1"

#define RMI_MAGIC				('r'<<24 | 'm'<<16 | 'i'<<8)
/*#define RMI_MAGIC				'rmi'*/
#define SUPPORT_RMI_VER			1

#define MARK(field_num, is_str)

#define _IN 
#define _OUT 

#define STR_ARRAY_LEN(str)	(sizeof(str)/sizeof(str[0]))
#define OFFSET(Type, member) (int)(&( ((Type*)0)->member) )
#define SIZE(Type, member, num) (sizeof(((Type*)0)->member)/num)

#ifdef _WIN32
#define  snprintf  _snprintf
#endif

#ifndef trace
#ifdef DEBUG
#define trace(fmt, ...) \
do{\
	char __buf1[64], __buf2[1024];\
    snprintf(__buf1, sizeof(__buf1), "[%s:%d-%s] ", __FILE__, __LINE__, __FUNCTION__);\
    snprintf(__buf2, sizeof(__buf2), fmt, ##__VA_ARGS__);\
    printf("%s%s", __buf1, __buf2);\
} while(0)
#else
#define trace(fmt, ...) do {} while(0)
#endif
#endif

#define RMI_ONE_WAY_REQ			1
#define RMI_TWO_WAY_REQ			2
#define RMI_RESPONSE			3
#define RMI_NOTIFY				4
#define RMI_CLOSE				5
#define RMI_MTYPE_MIN			RMI_ONE_WAY_REQ
#define RMI_MTYPE_MAX			RMI_CLOSE

#define RMI_FIELD_VAR			0
#define RMI_FIELD_FIX			1
#define RMI_FIELD_LEN			2
#define RMI_FIELD_SIGNED		3

#define RMI_MAX_CONNECT			256
#define RMI_DEFAULT_TIMEOUT		1000	//unit: 1ms
#define RMI_DEFAULT_POOL_SIZE	4096
#define RMI_DEFAULT_RECV_BUF_SIZE	(512*1024)
#define RMI_DEFAULT_KEEPALIVE_TIME 5000	// 5s

#define RMI_SOCKET_TCP	0
#define RMI_SOCKET_UDP	1

struct rmi_header {
	unsigned int magic;		// "rmi"
	unsigned int plen;		// payload length
	unsigned int seq;		// unique sequence number
	unsigned char ver;		// rmi version, now 1
	unsigned char opt;		// option, now 0
	unsigned char mtype;	// 1-4
	unsigned char stat;		// 0-success, 1-no such object, 2-no such interface, 3-no such method
	unsigned char caof;		// now 0
	unsigned char rsv[7];	// must 0
	unsigned char objid[16];
	unsigned char itfid[16];
	unsigned int funcid;
	unsigned int crc;
};

struct struct_entry {
	int field_num;
	int field_type;
	char * type_name;
	int num;
	int type_len;
	int offset;
};

struct struct_pair_entry {
	char * name;
	int member_num;
	struct struct_entry * entry;
};

struct rmi {
	int fd;
	int seq;
	int thread_start;
	thread_pool * thread_pool;
	lock * lock;
	int timeout;	// unit: 1ms
	int max_connect_num;
	int connect_num;
	void * mem_pool;
	int mem_pool_size;

	int socket_type;	// tcp or udp

	// for client
	char server_ip[16];
	unsigned short server_port;

	// for udp
	char peer_ip[16];
	unsigned short peer_port;
	int broadcast;
	int recv_buf_size;
	int send_buf_size;
	int no_ack;

	int keepalive_time;
	
	void * user_data;

	// support multi server/client
	int struct_num;
	struct struct_pair_entry * struct_pair;
	int func_num;
	struct func_entry * func_table;
	struct struct_entry * return_para;
};

struct func_entry {
	int func_id;
	int (* invoke_func)(struct rmi * rmi, struct func_entry * func, const unsigned char * pbuf, const int len, unsigned char ** ret_buf, int * ret_len);
	int para_num;
	struct struct_entry * para;
};

/*#define INCLUDE_RMI_MODEL(ID)	(#include "ID##_rmi.h")*/

#define RMI_INIT_SERVER(server_rmi, ID) \
do {\
	rmi_init(server_rmi);\
	ID##_set_server_table(server_rmi);\
} while(0)

#define RMI_INIT_CLIENT(client_rmi, ID) \
do {\
	rmi_init(client_rmi);\
	ID##_set_client_table(client_rmi);\
} while(0)

int rmi_init(struct rmi * rmi);
int rmi_finit(struct rmi * rmi);

int rmi_set_timeout(struct rmi * rmi, int timeout/* unit: 1ms*/);
int rmi_set_mem_pool_size(struct rmi * rmi, int pool_size);
int rmi_set_max_connect_num(struct rmi * rmi, int num);
int rmi_set_recv_buf_size(struct rmi * rmi, int buf_size);
int rmi_set_send_buf_size(struct rmi * rmi, int buf_size);
int rmi_set_socket_type(struct rmi * rmi, int socket_type);
int rmi_set_broadcast(struct rmi * rmi);
int rmi_set_noack(struct rmi * rmi);
int rmi_set_keepalive_time(struct rmi * rmi, int time);

void rmi_lock(struct rmi * rmi);
void rmi_unlock(struct rmi * rmi);

int rmi_server_start(struct rmi * rmi, unsigned short port);
int rmi_server_close(struct rmi * rmi);
int rmi_client_start(struct rmi * rmi, char * host, unsigned short port);
int rmi_client_close(struct rmi * rmi);

struct func_entry * get_func_entry(struct rmi * rmi, int id);
int func_serialize(struct rmi * rmi, const unsigned char * pdata, unsigned char * pbuf, const struct struct_entry * entry_in);
int func_deserialize(struct rmi * rmi, unsigned char * pdata, const unsigned char * pbuf, const struct struct_entry * entry_in);
int find_response(struct rmi_header * hdr, struct rmi_header * r_hdr);
void gen_header(struct rmi_header * hdr, int id, int len, int seq);
int invoke(struct rmi * rmi, int id, unsigned char * pbuf, int len, unsigned char ** r_buf, int * r_len);

#ifdef __cplusplus
}
#endif

#endif

