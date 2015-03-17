#ifndef __RMI_H__
#define __RMI_H__

#include <pthread.h>
#include <stdio.h>
#include <string.h>

#define DEBUG

#define RMI_VERSION				"1.0.1"

#define RMI_MAGIC				('r'<<24 | 'm'<<16 | 'i'<<8)
/*#define RMI_MAGIC				'rmi'*/
#define SUPPORT_RMI_VER			1

#define MARK(field_num, string)

#define IN 
#define OUT 

#define STR_ARRAY_LEN(str)	(sizeof(str)/sizeof(str[0]))
#define OFFSET(Type, member) (int)(&( ((Type*)0)->member) )
#define SIZE(Type, member, num) (sizeof(((Type*)0)->member)/num)

#ifdef DEBUG
#define trace(fmt...) \
do{\
	char __buf1[64], __buf2[1024];\
    snprintf(__buf1, sizeof(__buf1), "[%s:%d-%s] ", __FILE__, __LINE__, __FUNCTION__);\
    snprintf(__buf2, sizeof(__buf2), fmt);\
    printf("%s%s", __buf1, __buf2);\
} while(0)
#else
#define trace(fmt...) do {} while(0)
#endif

#define RMI_ONE_WAY_REQ			1
#define RMI_TWO_WAY_REQ			2
#define RMI_RESPONSE			3
#define RMI_NOTIFY				4
#define RMI_MTYPE_MIN			RMI_ONE_WAY_REQ
#define RMI_MTYPE_MAX			RMI_NOTIFY

#define RMI_FIELD_VAR			0
#define RMI_FIELD_FIX			1
#define RMI_FIELD_LEN			2
#define RMI_FIELD_SIGNED		3

#define RMI_MAX_CONNECT			256
#define RMI_DEFAULT_TIMEOUT		500	//unit: 1ms
#define RMI_DEFAULT_POOL_SIZE	4096
#define RMI_DEFAULT_RECV_BUF_SIZE	4096

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
	pthread_t pid;
	pthread_mutex_t lock;
	int timeout;	// unit: 1ms
	int max_connect_num;
	int connect_num;
	void * mem_pool;
	int mem_pool_size;
	void * user_data;
};

struct func_entry {
	int func_id;
	int (* invoke_func)(struct rmi * rmi, struct func_entry * func, const unsigned char * pbuf, const int len, unsigned char ** ret_buf, int * ret_len);
	int para_num;
	struct struct_entry * para;
};

extern const int struct_num;
extern struct struct_pair_entry struct_pair[];
extern const int func_num;
extern struct func_entry func_table[];
extern struct struct_entry return_para[];

int rmi_init(struct rmi * rmi);

int rmi_set_timeout(struct rmi * rmi, int timeout/* unit: 1ms*/);
int rmi_set_mem_pool_size(struct rmi * rmi, int pool_size);
int rmi_set_max_connect_num(struct rmi * rmi, int num);

int rmi_lock(struct rmi * rmi);
int rmi_unlock(struct rmi * rmi);

int rmi_server_start(struct rmi * rmi, unsigned short port);
int rmi_server_close(struct rmi * rmi);
int rmi_client_start(struct rmi * rmi, char * host, unsigned short port);
int rmi_client_close(struct rmi * rmi);

struct func_entry * get_func_entry(int id);
int func_serialize(const unsigned char * pdata, unsigned char * pbuf, const struct struct_entry * entry_in);
int func_deserialize(unsigned char * pdata, const unsigned char * pbuf, const struct struct_entry * entry_in);
int find_response(struct rmi_header * hdr, struct rmi_header * r_hdr);
void gen_header(struct rmi_header * hdr, int id, int len, int seq);
int invoke(struct rmi * rmi, int id, unsigned char * pbuf, int len, unsigned char ** r_buf, int * r_len);

#endif

