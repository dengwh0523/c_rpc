#ifndef __LIST_H__
#define __LIST_H__

#include <pthread.h>

#define LIST_WAIT_FLAG		0
#define LIST_OVERWRITE_FLAG	1

#define LIST_NOREMAL_FREE	0
#define LIST_FORCE_FREE	1

#define DEFAULT_MAX_PACKET	16
#define USER_NAME_MAX_LEN	32

typedef struct NODE_S {
	struct NODE_S * next;
	int len;
	int use_cnt;
	unsigned int create_time;
	unsigned char data[];
} NODE_S;

typedef struct LIST_S {
	NODE_S * head;
	NODE_S * tail;
	pthread_mutex_t lock;
	int num;
	int init_status;
	unsigned char * pbuf;
	int total_len;
	int max_time;
	int max_packet;	// for user
	int max_user;
	struct LIST_S * user_list;
} LIST_S;

typedef struct LOOPBUF_S {
	unsigned char *pbuf;
	int total_len;
	int head;
	int tail;
	int num;
	int left_num;
	pthread_mutex_t lock;
	int status;
} LOOPBUF_S;

typedef struct {
	LIST_S work_list;
	LIST_S free_list;
	unsigned char * pbuf;
	int total_len;
	int blk_size;
} POOL_S;

typedef int (* LIST_CALLBACK)(void * src, void * dst);

// list
int list_init(LIST_S * plist, int len, int max_time, int max_packet, int max_user);
int list_finit(LIST_S * plist);
/*NODE_S * list_get_free_node(LIST_S * plist, int data_len);	// 支持从内存池获取空闲空间，或者动态分配空间*/
/*int list_add_head(LIST_S * plist,NODE_S * pnode);*/
/*int list_add_tail(LIST_S * plist,NODE_S * pnode);*/
/*NODE_S * list_remove_head(LIST_S * plist);*/
/*NODE_S * list_remove_tail(LIST_S * plist);*/
/*void list_free_node(LIST_S * plist, NODE_S * pnode, int flag);*/
/*void list_free_head(LIST_S * plist, unsigned int max_time);*/
/*NODE_S * list_find_user(LIST_S * plist, char * user_name);*/

void list_lock(LIST_S * plist);
void list_unlock(LIST_S * plist);
	
// large data sequential operation
LIST_S * get_user_list(LIST_S * plist, char * user_name);
int list_add_user(LIST_S * plist, char * user_name);
int list_delete_user(LIST_S * plist, char * user_name);
int list_write_data(LIST_S * plist, unsigned char * pbuf, int len, int flag);
int list_read_data(LIST_S * plist, unsigned char * pbuf, int max_len);
int list_user_read(LIST_S * plist, char * user_name, unsigned char * pbuf, int max_len);

int list_size(LIST_S * plist);
void * list_at(LIST_S * plist, int index);
int list_erase(LIST_S * plist, const int index);
int list_erase_it(LIST_S * plist, void * pbuf, LIST_CALLBACK cb);
int list_operate_it(LIST_S * plist, LIST_CALLBACK cb);

// loopbuf
int loopbuf_init(LOOPBUF_S * pstLoopbuf, unsigned char * pbuf, int len);
int loopbuf_finit(LOOPBUF_S * pstLoopbuf);
int loopbuf_put_data(LOOPBUF_S * pstLoopbuf, unsigned char * pbuf, int len);
int loopbuf_get_data(LOOPBUF_S * pstLoopbuf, unsigned char * pbuf, int len);
int loopbuf_data_cnt(LOOPBUF_S * pstLoopbuf);
void loopbuf_lock(LOOPBUF_S * pstLoopbuf);
void loopbuf_unlock(LOOPBUF_S * pstLoopbuf);

// message pool
int pool_init(POOL_S * pool, int num, int blk_size);
int pool_finit(POOL_S * pool);
int pool_write_data(POOL_S * pool, unsigned char * pbuf, int len);
int pool_read_data(POOL_S * pool, unsigned char * pbuf, int max_len);
int pool_size(POOL_S * pool);
int pool_operate_it(POOL_S * pool, LIST_CALLBACK cb);
int pool_erase_it(POOL_S * pool, void * pbuf, LIST_CALLBACK cb);

#endif
