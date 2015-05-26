#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "list.h"

// 多线程处理
typedef struct {
	LIST_S * plist;
	NODE_S * pnode;
	unsigned int pnode_create_time;
} LIST_USER_S;

typedef struct {
	char name[USER_NAME_MAX_LEN];
	LIST_S user_list;
} USER_S;

NODE_S * list_get_free_node(LIST_S * plist, int data_len) {
	NODE_S * pnode;
	int head;
	int tail;
	int next;

	int len;
	
	len = data_len + sizeof(NODE_S);

	if (NULL != plist->pbuf && len > plist->total_len) {
		return NULL;
	}

	if (NULL == plist->pbuf) {
		pnode = malloc(len);
		if (NULL == pnode) {
			trace("malloc error\n");
			return NULL;
		}
		goto exit;
	}

	pnode = plist->tail;
	next = 0;
	
	if (pnode) {
		tail = (unsigned char *)plist->tail - plist->pbuf;
		head = (unsigned char *)plist->head - plist->pbuf;
		next = pnode->data + pnode->len - plist->pbuf;
		next = (next + 3) & (~3);

/*		do {*/
/*			if (tail >= head) {*/
/*				if (next + len > plist->total_len) {*/
/*					next = 0;*/
/*				} else {*/
/*					break;*/
/*				}*/
/*			}*/
/*			if (next + len > head) {*/
/*				return NULL;*/
/*			}*/
/*		} while(0);*/
		do {
			if (next > head) {
				if (next + len > plist->total_len) {
					next = 0;
				} else {	
					break;
				}
			}

			if (next + len > head) {
				return NULL;
			}
		} while(0);
	}
	
	pnode = (NODE_S *)(plist->pbuf + next);

exit:	
	memset(pnode, 0, sizeof(NODE_S));
	pnode->len = data_len;
	pnode->create_time = get_tick_time();
	return pnode;
}

int list_init(LIST_S * plist, int len, int max_time, int max_packet, int max_user) {
	memset(plist, 0, sizeof(LIST_S));
	if (0 != len) {
		plist->pbuf = malloc(len);
		if (NULL == plist->pbuf) {
			trace("malloc error\n");
			return -1;
		}
	} else {
		plist->pbuf = NULL;
	}
	plist->total_len = len;
	pthread_mutex_init(&plist->lock, NULL);
	if (max_user > 0) {
		plist->user_list = malloc(sizeof(LIST_S));
		if (NULL == plist->user_list) {
			trace("malloc error\n");
			return -1;
		}
		memset(plist->user_list, 0, sizeof(LIST_S));
	}
	plist->max_time = max_time;
	plist->max_packet = max_packet;
	plist->max_user = max_user;
	plist->init_status = 1;

	return 0;
}

int list_finit(LIST_S * plist) {
	if (plist->user_list) {
		list_finit(plist->user_list);
	}
	while (plist && plist->num > 0) {
		list_read_data(plist, NULL, 0);
	}
	if (plist->init_status) {
		if (NULL != plist->pbuf) {
			free(plist->pbuf);
		}
		if (plist->user_list) {
			free(plist->user_list);
			memset(plist, 0, sizeof(LIST_S));
		}
		pthread_mutex_destroy(&plist->lock);
	}
	memset(plist, 0, sizeof(LIST_S));

	return 0;
}

void list_lock(LIST_S * plist) {
	pthread_mutex_lock(&plist->lock);
}

void list_unlock(LIST_S * plist) {
	pthread_mutex_unlock(&plist->lock);
}

int list_add_tail(LIST_S * plist, NODE_S * pnode) {
	if (NULL == plist || NULL == pnode) {
		trace("para error\n");
		return -1;
	}
	pnode->next = NULL;
	if (NULL == plist->head) {
		plist->head = pnode;
		plist->tail = pnode;
	} else {
		plist->tail->next = pnode;
		plist->tail = pnode;
	}
	plist->num++;
	
	return 0;
}

int list_add_head(LIST_S * plist, NODE_S * pnode) {
	if (NULL == plist || NULL == pnode) {
		trace("para error\n");
		return -1;
	}
	
	pnode->next = plist->head;
	if (NULL == plist->head) {
		plist->tail = pnode;
	}
	plist->head = pnode;
	plist->num++;
	
	return 0;
}

NODE_S * list_remove_head(LIST_S * plist) {
	NODE_S * pnode;
	if (NULL == plist) {
		trace("para error\n");
		return NULL;
	}
	
	if (NULL == plist->head) {
		return NULL;
	}

	pnode = plist->head;
	plist->head = plist->head->next;
	if (NULL == plist->head) {
		plist->tail = NULL;
	}
	plist->num--;

	return pnode;
}

NODE_S * list_remove_tail(LIST_S * plist) {
	NODE_S * pnode, * pre;
	if (NULL == plist) {
		trace("para error\n");
		return NULL;
	}
	
	if (NULL == plist->head) {
		return NULL;
	}

	if (NULL == plist->head->next) {
		pnode = plist->head;
		plist->head = NULL;
		plist->tail = NULL;
	} else {
		for (pre = pnode = plist->head; NULL != pnode->next; pre = pnode, pnode = pnode->next);
		pre->next = NULL;
		plist->tail = pre;
	}
	
	plist->num--;

	return pnode;
}

void list_free_node(LIST_S * plist, NODE_S * pnode, int flag) {
	if (NULL == pnode) {
		return;
	}
	if (0 == pnode->use_cnt || LIST_FORCE_FREE == flag) {
		if (NULL == plist->pbuf) {		
			free(pnode);
		}
	} else {
		list_add_head(plist, pnode);
	}
}

int list_delete_node(LIST_S * plist, NODE_S * pnode) {
	if (NULL == plist || NULL == pnode) {
		trace("para error\n");
		return -1;
	}

	if (pnode == plist->head) {
		plist->head = plist->head->next;
		if (NULL == plist->head) {
			plist->tail = NULL;
		}
	} else {
		NODE_S * pre, * cur;
		for (pre = cur = plist->head; NULL != cur; pre = cur, cur = cur->next) {
			if (pnode == cur) {
				break;
			}
		}
		if (NULL == cur) {
			trace("node is not exist\n");
			return -1;
		}

		pre->next = cur->next;
		if (NULL == pre->next) {
			plist->tail = pre;
		}
	}
	list_free_node(plist, pnode, LIST_FORCE_FREE);
	plist->num--;

	return 0;
}

void list_free_head(LIST_S * plist, unsigned int max_time) {	
	while(1) {
		unsigned int cur_time;
		int flag = 0;
		NODE_S * pnode;

		pnode = plist->head;
		if (NULL == pnode) {
			break;
		}
/*		printf("plist: head: %p, next: %p, tail: %p, buf: %p\n", plist->head, plist->head->next, plist->tail, plist->pbuf);*/
		
		cur_time = get_tick_time();
		if (cur_time - pnode->create_time > max_time) {
			flag = LIST_FORCE_FREE;
			printf("force free\n");
		}
		list_free_node(plist, list_remove_head(plist), flag);
		if (pnode == plist->head) {
			break;
		}
	}
}

// 多线程读写
NODE_S * list_find_user(LIST_S * plist, char * user_name) {
	NODE_S * pnode;
	for (pnode = plist->user_list->head; NULL != pnode; pnode = pnode->next) {			
		USER_S * pstUser = (USER_S *)pnode->data;
		if (strlen(user_name) == strlen(pstUser->name) && 0 == memcmp(pstUser->name, user_name, strlen(pstUser->name))) {
			break;
		}		
	}

	return pnode;
}

int list_add_user(LIST_S * plist, char * user_name) {
	USER_S stUser;
	if (NULL == plist || NULL == plist->user_list|| strlen(user_name) >= USER_NAME_MAX_LEN) {
		trace("para error\n");
		return -1;
	}
	if (plist->user_list->num >= plist->max_user) {
		trace("user is full\n");
		return -1;
	}

	if (NULL != list_find_user(plist, user_name)) {
		trace("user is exist\n");
		return -1;
	}

	memset(&stUser, 0, sizeof(stUser));
	strcpy(stUser.name, user_name);
	if (0 != list_init(&stUser.user_list, 0, 0, 0, 0)) {
		trace("list_init error\n");
		return -1;
	}

	if (0 != list_write_data(plist->user_list, (unsigned char *)&stUser, sizeof(stUser), LIST_WAIT_FLAG)) {
		trace("list_write_data error\n");
		return -1;
	}

	return 0;
}

LIST_S * get_user_list(LIST_S * plist, char * user_name) {
	NODE_S * pnode;
	USER_S * pstUser;
	pnode = list_find_user(plist, user_name);
	if (pnode) {
		pstUser = (USER_S *)pnode->data;
		return &pstUser->user_list;
	}

	return NULL;
}

int list_delete_user(LIST_S * plist, char * user_name) {
	NODE_S * pnode;
	USER_S * pstUser;
	if (NULL == plist || NULL == plist->user_list) {
		trace("para error\n");
		return -1;
	}

	pnode = list_find_user(plist, user_name);
	if (NULL == pnode) {
		trace("user: %s is not exist\n", user_name);
		return -1;
	}

	pstUser = (USER_S *)pnode->data;
	if (0 != list_finit(&pstUser->user_list)) {
		trace("list_finit error\n");
		return -1;
	}

	return list_delete_node(plist->user_list, pnode);
}

int list_user_write(LIST_S * plist, LIST_S * main_list, int max_packet) {
	LIST_USER_S stListUser;
	stListUser.plist = main_list;
	stListUser.pnode = main_list->tail;
	stListUser.pnode->use_cnt++;
	stListUser.pnode_create_time = stListUser.pnode->create_time;
	if (NULL == plist->pbuf && 0 == max_packet) {
		max_packet = DEFAULT_MAX_PACKET;
	}
	
	while(max_packet > 0 && plist->num >= max_packet) {
		list_read_data(plist, NULL, sizeof(stListUser));
	}
	list_write_data(plist, (unsigned char *)&stListUser, sizeof(stListUser), LIST_OVERWRITE_FLAG);

	return 0;
}

int list_write_data(LIST_S * plist, unsigned char * pbuf, int len, int flag) {
	NODE_S * pnode;
	int ret = 0;
	int i;
	
	if (NULL != plist->pbuf && len > plist->total_len) {
		trace("data len[%d] is too large\n", len);
		return -1;
	}

	if (NULL != plist->user_list && plist->user_list->num > 0) {
		list_free_head(plist, plist->max_time);
	}
	
	do {
		pnode = list_get_free_node(plist, len);
		if (pnode) {
			break;
		}
		if (LIST_OVERWRITE_FLAG == flag) {
			printf("force free2\n");
			list_free_node(plist, list_remove_head(plist), LIST_FORCE_FREE);
		} else {
			ret = -1;
			break;
		}
	} while(1);
	if (pnode) {
		NODE_S * user_node;
		memcpy(pnode->data, pbuf, len);	
		list_add_tail(plist, pnode);
		if (plist->user_list) {
			for (user_node = plist->user_list->head; NULL != user_node; user_node = user_node->next) {			
				USER_S * pstUser = (USER_S *)user_node->data;
				list_user_write(&pstUser->user_list, plist, plist->max_packet);
			}
		}
	}	

	return ret;
}

int list_get_data(NODE_S * pnode, unsigned char * pbuf, int max_len) {
	int len;

	if (NULL == pbuf) {
		return 0;
	}
	
	len = pnode->len;
	if (len > max_len) {
		len = max_len;
		trace("trunct data\n");
	}
	memcpy(pbuf, pnode->data, len);
	return len;
}

int list_read_data(LIST_S * plist, unsigned char * pbuf, int max_len) {
	NODE_S * pnode;
	int len;
	
	pnode = list_remove_head(plist);
	if (pnode) {
		len = list_get_data(pnode, pbuf, max_len);
		list_free_node(plist, pnode, 0);
	} else {
		len = 0;
	}	

	return len;
}

int list_user_read(LIST_S * plist, char * user_name, unsigned char * pbuf, int max_len) {
	LIST_USER_S stListUser;
	LIST_S * main_list;
	int len;
	NODE_S * pnode;
	
	NODE_S * user_node;
	USER_S * pstUser;
	user_node = list_find_user(plist, user_name);
	if (NULL == user_node) {
		trace("user %s is not exist\n", user_name);
		return -1;
	}

	pstUser = (USER_S *)user_node->data;
	
	if (list_read_data(&pstUser->user_list, (unsigned char *)&stListUser, sizeof(stListUser)) <= 0) {
		return -1;
	}

	main_list = stListUser.plist;

	for (pnode = main_list->head; NULL != pnode; pnode = pnode->next) {
		if (pnode == stListUser.pnode && pnode->create_time == stListUser.pnode_create_time) {
			len = list_get_data(pnode, pbuf, max_len);			
			pnode->use_cnt--;
			break;
		}
	} 

	if (NULL == pnode) {
		trace("pnode is not in the main list\n");
		len = -1;
		goto exit;
	}
	
exit:

	return len;
}

int list_size(LIST_S * plist) {
	if (plist) {
		return plist->num;
	} else {
		return -1;
	}
}

void * list_at(LIST_S * plist, const int index) {
	NODE_S * pnode;
	int cnt = 0;

	if (NULL == plist) {
		trace("plist is NULL\n");
		return NULL;
	}

	if (index >= plist->num) {
		trace("index is too large\n");
		return NULL;
	}

	pnode = plist->head;
	while(pnode) {
		if (index == cnt) {
			return pnode->data;
		}
		pnode = pnode->next;
		cnt++;
	}

	return NULL;
}

int list_erase(LIST_S * plist, const int index) {	
	NODE_S * pnode;
	int cnt = 0;
	
	if (NULL == plist) {
		trace("para error\n");
		return -1;
	}

	if (0 == index) {
		pnode = plist->head;
		plist->head = plist->head->next;
		if (NULL == plist->head) {
			plist->tail = NULL;
		}
	} else {
		NODE_S * pre, * cur;
		for (pre = cur = plist->head; NULL != cur; pre = cur, cur = cur->next) {
			pnode = cur;
			if (index == cnt) {
				break;
			}
			cnt++;
		}
		if (NULL == cur) {
			trace("node is not exist\n");
			return -1;
		}

		pre->next = cur->next;
		if (NULL == pre->next) {
			plist->tail = pre;
		}
	}
	list_free_node(plist, pnode, LIST_FORCE_FREE);
	plist->num--;

	return 0;
}

int list_operate_it(LIST_S * plist, LIST_CALLBACK cb) {
	NODE_S * pnode;

	pnode = plist->head;
	while(pnode) {
		cb(NULL, pnode->data);
		pnode = pnode->next;
	}

	return 0;
}

NODE_S * list_remove_it(LIST_S * plist, void * pbuf, LIST_CALLBACK cb) {
	NODE_S * cur, * pre;

	pre = cur = plist->head;
	while(cur) {
		if (0 == cb(pbuf, cur->data)) {
			if (cur == plist->head) {
				plist->head = plist->head->next;
				if (NULL == plist->head) {
					plist->tail = NULL;
				}				
			} else {
				pre->next = cur->next;
				if (NULL == pre->next) {
					plist->tail = pre;
				}
			}
/*			list_free_node(plist, cur, LIST_FORCE_FREE);*/
			plist->num--;
			return cur;
		}
		pre = cur;
		cur = cur->next;
	}

	return NULL;
}


int list_erase_it(LIST_S * plist, void * pbuf, LIST_CALLBACK cb) {
	NODE_S * pnode;

	pnode = list_remove_it(plist, pbuf, cb);
	if (NULL == pnode) {
		return -1;
	} else {
		list_free_node(plist, pnode, LIST_FORCE_FREE);
	}

	return 0;
}

int loopbuf_init(LOOPBUF_S * pstLoopbuf, unsigned char * pbuf, int len) {
	if (NULL == pbuf) {
		trace("para error\n");
		return -1;
	}
	memset(pstLoopbuf, 0, sizeof(LOOPBUF_S));
	pstLoopbuf->pbuf = pbuf;
	pstLoopbuf->total_len = len;
	pstLoopbuf->left_num = pstLoopbuf->total_len - pstLoopbuf->num;
	pthread_mutex_init(&pstLoopbuf->lock, NULL);
	pstLoopbuf->status = 1;

	return 0;
}

int loopbuf_finit(LOOPBUF_S * pstLoopbuf) {
	if (pstLoopbuf->status) {
		pthread_mutex_destroy(&pstLoopbuf->lock);
		pstLoopbuf->status = 0;
	}

	return 0;
}

int loopbuf_put_data(LOOPBUF_S * pstLoopbuf, unsigned char * pbuf, int len) {
	int left_bytes = 0;
	int wr_bytes = 0;
	unsigned char * wr_buf = NULL;
	
	if (NULL == pbuf) {
		trace("para error\n");
		return -1;
	}
	
	if (pstLoopbuf->left_num < len) {
		trace("there is not enough buf\n");
		return -1;
	}

	left_bytes = len;
	wr_buf = pbuf;
	while(left_bytes > 0) {
		wr_bytes = pstLoopbuf->total_len - pstLoopbuf->tail;
		if (wr_bytes > left_bytes) {
			wr_bytes = left_bytes;
		}
		memcpy(&pstLoopbuf->pbuf[pstLoopbuf->tail], wr_buf, wr_bytes);
		pstLoopbuf->tail += wr_bytes;
		if (pstLoopbuf->tail >= pstLoopbuf->total_len) {
			pstLoopbuf->tail -= pstLoopbuf->total_len;
		}
		wr_buf += wr_bytes;
		left_bytes -= wr_bytes;
	}

	pstLoopbuf->num += len;
	pstLoopbuf->left_num = pstLoopbuf->total_len - pstLoopbuf->num;
	return 0;
}

int loopbuf_get_data(LOOPBUF_S * pstLoopbuf, unsigned char * pbuf, int len) {
	int left_bytes = 0;
	int rd_bytes = 0;
	unsigned char * rd_buf = NULL;
	
/*	if (NULL == pbuf) {*/
/*		trace("para error\n");*/
/*		return -1;*/
/*	}*/
	
	if (pstLoopbuf->num < len) {
		trace("there is no enough data\n");
		return -1;
	}

	left_bytes = len;
	rd_buf = pbuf;
	while(left_bytes > 0) {
		rd_bytes = pstLoopbuf->total_len - pstLoopbuf->head;
		if (rd_bytes > left_bytes) {
			rd_bytes = left_bytes;
		}
		if (pbuf) memcpy(rd_buf, &pstLoopbuf->pbuf[pstLoopbuf->head], rd_bytes);
		pstLoopbuf->head += rd_bytes;
		if (pstLoopbuf->head >= pstLoopbuf->total_len) {
			pstLoopbuf->head -= pstLoopbuf->total_len;
		}
		rd_buf += rd_bytes;
		left_bytes -= rd_bytes;
	}

	pstLoopbuf->num -= len;
	pstLoopbuf->left_num = pstLoopbuf->total_len - pstLoopbuf->num;

	return 0;
}

int loopbuf_data_cnt(LOOPBUF_S * pstLoopbuf) {
	return pstLoopbuf->num;
}

void loopbuf_lock(LOOPBUF_S * pstLoopbuf) {
	pthread_mutex_lock(&pstLoopbuf->lock);
}

void loopbuf_unlock(LOOPBUF_S * pstLoopbuf) {
	pthread_mutex_unlock(&pstLoopbuf->lock);
}

int pool_init(POOL_S * pool, int num, int blk_size) {
	NODE_S * pnode;
	unsigned char * pbuf;
	int len;
	int i;
	
	if (blk_size <= 0) {
		trace("para error\n");
		return -1;
	}

	memset(pool, 0, sizeof(POOL_S));

	pool->blk_size = blk_size;

	blk_size += sizeof(NODE_S);
	len = num*blk_size;

	list_init(&pool->work_list, 0, 0, 0, 0);
	list_init(&pool->free_list, 0, 0, 0, 0);

	pbuf = malloc(len);
	if (NULL == pbuf) {
		trace("malloc failed\n");
		return -1;
	}

	pool->pbuf = pbuf;
	pool->total_len = len;

	for (i = 0; i < num; i++) {
		pnode = (NODE_S *)pbuf;
		list_add_tail(&pool->free_list, pnode);
		pbuf += blk_size;
	}

	return 0;	
}

int pool_finit(POOL_S * pool) {
	free(pool->pbuf);
	memset(pool, 0, sizeof(POOL_S));

	return 0;
}

int pool_write_data(POOL_S * pool, unsigned char * pbuf, int len) {
	NODE_S * pnode;
	LIST_S * free_list = &pool->free_list;
	LIST_S * work_list = &pool->work_list;
	
	if (len > pool->blk_size) {
		trace("data is too large\n");
		return -1;
	}

	list_lock(free_list);
	if (list_size(free_list) <= 0) {
		trace("pool is full\n");
		list_unlock(free_list);
		return -1;
	} else {
		pnode = list_remove_head(free_list);
	}
	list_unlock(free_list);

	memcpy(pnode->data, pbuf, len);
	pnode->len = len;

	list_lock(work_list);
	list_add_tail(work_list, pnode);
	list_unlock(work_list);

	return 0;
}

int pool_read_data(POOL_S * pool, unsigned char * pbuf, int max_len) {
	NODE_S * pnode;
	LIST_S * free_list = &pool->free_list;
	LIST_S * work_list = &pool->work_list;
	int len;

	list_lock(work_list);
	if (list_size(work_list) <= 0) {
		trace("pool is empty\n");
		list_unlock(work_list);
		return -1;
	} else {
		pnode = list_remove_head(work_list);
	}
	list_unlock(work_list);

	len = pnode->len;
	if (len > max_len) {
		len = max_len;
	}

	memcpy(pbuf, pnode->data, len);

	list_lock(free_list);
	list_add_tail(free_list, pnode);
	list_unlock(free_list);

	return 0;
}

int pool_size(POOL_S * pool) {
	LIST_S * work_list = &pool->work_list;
	int num;
	
	list_lock(work_list);
	num = list_size(work_list);
	list_unlock(work_list);

	return num;
}

int pool_operate_it(POOL_S * pool, LIST_CALLBACK cb) {
	LIST_S * work_list = &pool->work_list;

	list_lock(work_list);
	list_operate_it(work_list, cb);
	list_unlock(work_list);

	return 0;
}

int pool_erase_it(POOL_S * pool, void * pbuf, LIST_CALLBACK cb) {
	NODE_S * pnode;
	LIST_S * free_list = &pool->free_list;
	LIST_S * work_list = &pool->work_list;
	
	list_lock(work_list);
	pnode = list_remove_it(work_list, pbuf, cb);
	list_unlock(work_list);

	if (NULL == pnode) {
		return -1;
	}

	list_lock(free_list);
	list_add_tail(free_list, pnode);
	list_unlock(free_list);

	return 0;
}

