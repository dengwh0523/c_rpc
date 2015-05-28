#include "debug.h"
#include "rmi.h"
#include "socket.h"
#include "list.h"
#include "mem_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

void * rmi_listen_thread(void * arg);
void * rmi_server_thread(void * arg);

int rmi_init(struct rmi * rmi) {
	if (NULL == rmi) {
		trace("rmi is null\n");
		return -1;
	}
	memset(rmi, 0, sizeof(struct rmi));
	rmi->lock = lock_create();
	return 0;
}

int rmi_finit(struct rmi * rmi) {
	if (NULL == rmi) {
		return 0;
	}

	if (rmi->lock) {
		lock_destroy(rmi->lock);
		rmi->lock = NULL;
	}

	return 0;
}

void rmi_lock(struct rmi * rmi) {
	lock_lock(rmi->lock);
}

void rmi_unlock(struct rmi * rmi) {
	lock_unlock(rmi->lock);
}

int rmi_set_timeout(struct rmi * rmi, int timeout/* unit: 1ms*/) {
	rmi->timeout = timeout;
	return 0;
}

int rmi_set_mem_pool_size(struct rmi * rmi, int pool_size) {
	rmi->mem_pool_size = pool_size;
	return 0;
}

/*int rmi_set_recv_buf_size(struct rmi * rmi, int buf_size) {*/
/*	rmi->loopbuf_len = buf_size;*/
/*	return 0;*/
/*}*/

int rmi_set_max_connect_num(struct rmi * rmi, int num) {
	rmi->max_connect_num = num;
	return 0;
}

static struct struct_pair_entry * get_struct_entry(char * struct_name) {
	int i;
	for (i = 0; i < struct_num; i++) {
		if (0 == strcmp(struct_name, struct_pair[i].name)) {
			return &struct_pair[i];
		}
	}
	
	return NULL;
}

int func_id_cmp(void * data, void * id) {
	unsigned int dst_id = (unsigned int)id;
	unsigned int src_id = ((struct func_entry *)data)->func_id;
	if (dst_id == src_id) {
		return 0;
	}

	return (dst_id>src_id) ? 1 : -1;
}

struct func_entry * get_func_entry(int id) {
	return FAST_FOR_EACH(func_table, func_num, struct func_entry, id, func_id_cmp);
}

static int serialize_rmi_header(struct rmi_header * hdr) {
	hdr->magic = htonl(hdr->magic);
	hdr->plen = htonl(hdr->plen);
	hdr->seq = htonl(hdr->seq);
	hdr->funcid = htonl(hdr->funcid);
	hdr->crc = htonl(hdr->crc);

	return 0;
}

static int deserialize_rmi_header(struct rmi_header * hdr) {
	hdr->magic = ntohl(hdr->magic);
	hdr->plen = ntohl(hdr->plen);
	hdr->seq = ntohl(hdr->seq);
	hdr->funcid = ntohl(hdr->funcid);
	hdr->crc = ntohl(hdr->crc);

	return 0;
}

// set unsigned char, short, int, long long
int set_varint(const unsigned char * pdata, const int len, unsigned char * pbuf) {
	int r_len = 0;
	unsigned long long value;
	switch(len) {
		case sizeof(char) : 
			value = *(unsigned char *)pdata;
			break;
		case sizeof(short) :
			value = *(unsigned short *)pdata;
			break;
		case sizeof(int) :
			value = *(unsigned int *)pdata;
			break;
		case sizeof(long long) :
			value = *(unsigned long long *)pdata;
			break;
		default	:
			trace("para error\n");
			return -1;
	}
	
	while(1) {
		if (value>>7) {
			pbuf[r_len++] = (1 << 7) | (value & 0x7f);
			value >>= 7;
		} else {
			pbuf[r_len++] = (unsigned char)value;
			break;
		}
	}
	return r_len;
}

// get unsigned char, short, int, long long
int get_varint(unsigned char * pdata, const int len, const unsigned char * pbuf) {
	int r_len = 0;
	unsigned long long value = 0;
	unsigned int tmp;
	while(1) {
		tmp = pbuf[r_len];
		value |= (tmp & 0x7f)<<(r_len*7);
		r_len++;
		if (0 == (tmp&0x80)) {
			break;
		}
	}
	if (pdata) {
		switch(len) {
			case sizeof(char) : 
				*(unsigned char *)pdata = (unsigned char)value;
				break;
			case sizeof(short) :
				*(unsigned short *)pdata = (unsigned short)value;
				break;
			case sizeof(int) :
				*(unsigned int *)pdata = (unsigned int)value;
				break;
			case sizeof(long long) :
				*(unsigned long long *)pdata = (unsigned long long)value;
				break;
			default	:
				trace("para error\n");
				return -1;
		}
	}
	return r_len;
}

// set signed 
int set_varint_signed(const unsigned char * pdata, const int len, unsigned char * pbuf) {
	int r_len = 0;
	long long value;
	switch(len) {
		case sizeof(char) : 
			value = *(signed char *)pdata;
			break;
		case sizeof(short) :
			value = *(short *)pdata;
			break;
		case sizeof(int) :
			value = *(int *)pdata;
			break;
		case sizeof(long long) :
			value = *(long long *)pdata;
			break;
		default	:
			trace("para error\n");
			return -1;
	}
	value = (value << 1) ^ (value >> (sizeof(long long)-1));
	return set_varint((unsigned char *)&value, len, pbuf);
}

// get signed char, short, int, long long
int get_varint_signed(unsigned char * pdata, const int len, const unsigned char * pbuf) {
	int r_len = 0;
	unsigned long long value_unsigned = 0;
	long long value;

	r_len = get_varint((unsigned char *)&value_unsigned, len, pbuf);
	if (value_unsigned & 0x01) {
		value = 0 - (value_unsigned>>1);
	} else {
		value = value_unsigned>>1;
	}
	
	if (pdata) {
		switch(len) {
			case sizeof(char) : 
				*(signed char *)pdata = (signed char)value;
				break;
			case sizeof(short) :
				*(short *)pdata = (short)value;
				break;
			case sizeof(int) :
				*(int *)pdata = (int)value;
				break;
			case sizeof(long long) :
				*(long long *)pdata = (long long)value;
				break;
			default	:
				trace("para error\n");
				return -1;
		}
	}
	return r_len;
}

// set float, double
int set_fix(const unsigned char * pdata, const int len, unsigned char * pbuf) {
	memcpy(pbuf, pdata, len);
	return len;
}

// get float, double
int get_fix(unsigned char * pdata, const int len, const unsigned char * pbuf) {
	if (pdata) memcpy(pdata, pbuf, len);
	return len;
}

// set string, struct
int set_len(const unsigned char * pdata, const int len, unsigned char * pbuf) {
	int r_len = 0;
	r_len += set_varint((unsigned char *)&len, sizeof(len), pbuf+r_len);
	r_len += set_fix(pdata, len, pbuf+r_len);
	return r_len;
}

// get string
int get_len(unsigned char * pdata, const int max_len, const unsigned char * pbuf) {
	int r_len = 0;
	int len = 0;
	r_len += get_varint((unsigned char *)&len, sizeof(len), pbuf+r_len);
	pbuf += r_len;
	r_len += len;
	if (len > max_len) {
		len = max_len;
	}
	get_fix(pdata, len, pbuf);
	return r_len;
}

int serialize(const unsigned char * pdata, unsigned char * pbuf, const struct struct_entry * entry_in) {
	int i, j;
	struct struct_pair_entry * pair_entry;
	const struct struct_entry * entry;
	int member_num;
	int len = 0;
	
	if (entry_in->type_name && (pair_entry = get_struct_entry(entry_in->type_name))) {
		entry = pair_entry->entry;
		member_num = pair_entry->member_num;
	} else {
		entry = entry_in;
		member_num = 1;
	}

	for (i = 0; i < member_num; i++) {
		const unsigned char * base = pdata+entry[i].offset;
		int key = (entry[i].field_num << 3) | entry[i].field_type;
		for (j = 0; j < entry[i].num; j++) {	// array
			// set key
			len += set_varint((unsigned char *)&key, sizeof(key), pbuf+len);

			// set value
			if (entry[i].type_name) {
				unsigned char * buf;
				int str_len;
				buf = malloc(entry[i].type_len*2);
				if (NULL == buf) {
					trace("malloc error\n");
					return -1;
				}
				str_len = serialize(base, buf, &entry[i]);
				len += set_len(buf, str_len, pbuf+len);
				free(buf);
			} else {
				switch(entry[i].field_type) {
					case RMI_FIELD_VAR :
						len += set_varint(base, entry[i].type_len, pbuf+len);
						break;
					case RMI_FIELD_FIX :
						len += set_len(base, entry[i].type_len, pbuf+len);
						break;
					case RMI_FIELD_LEN:
						len += set_len(base, strlen(base), pbuf+len);
						break;
					case RMI_FIELD_SIGNED:
						len += set_varint_signed(base, entry[i].type_len, pbuf+len);
						break;
					default :
						trace("field type[%d] is not support now\n", entry[i].field_type);
						return -1;
				}
			}
			base += entry[i].type_len;
		}
	}

	return len;
}

int deserialize(unsigned char * pdata, const unsigned char * pbuf, const int enc_len, const struct struct_entry * entry_in) {
	int i;
	struct struct_pair_entry * pair_entry;
	const struct struct_entry * entry;
	int member_num;
	int len = 0;
	int key;
	int field_num;
	int field_type;
	int left_bytes = enc_len;
	int last_field_num = -1;
	int field_cnt = 0;
	int offset = 0;
	
	if (entry_in->type_name && (pair_entry = get_struct_entry(entry_in->type_name))) {
		entry = pair_entry->entry;
		member_num = pair_entry->member_num;
	} else {
		entry = entry_in;
		member_num = 1;
	}

	while(left_bytes > 0) {
		unsigned char * base;

		// get key
		len += get_varint((unsigned char *)&key, sizeof(key), pbuf+len);
		field_num = key >> 3;
		field_type = key & 0x7;
		
		for (i = 0; i < member_num; i++) {
			if (entry[i].field_num == field_num) {
				break;
			}
		}

		if (field_num != last_field_num) {
			field_cnt = 0;
			offset = 0;
		} else {
			field_cnt++;
			if (i != member_num) {
				offset += entry[i].type_len;
			}
		}
		last_field_num = field_num;

		if (i == member_num) {
			trace("not exist filed: %d\n", field_num);
			base = NULL;
		} else {
			base = pdata+entry[i].offset+offset;
		}

		if (field_cnt >= entry[i].num) {
			base = NULL;
		}

		// get value
		switch(field_type) {
			case RMI_FIELD_VAR :
				len += get_varint(base, entry[i].type_len, pbuf+len);
				break;
			case RMI_FIELD_FIX :
				if (base) memset(base, 0, entry[i].type_len);
				len += get_len(base, entry[i].type_len, pbuf+len);
				break;
			case RMI_FIELD_LEN:
				if (base) {						
					if (entry[i].type_name) {
						int str_len;
						len += get_varint((unsigned char *)&str_len, sizeof(str_len), pbuf+len);
						len += deserialize(base, pbuf+len, str_len, &entry[i]);
					} else {
						memset(base, 0, entry[i].type_len);
						len += get_len(base, entry[i].type_len-1, pbuf+len);
					}
				} else {
					len += get_len(base, entry[i].type_len-1, pbuf+len);
				}
				break;
			case RMI_FIELD_SIGNED:
				len += get_varint_signed(base, entry[i].type_len, pbuf+len);
				break;
			default :
				trace("field type[%d] is not support now\n", entry[i].field_type);
				return -1;
		}
		left_bytes = enc_len - len;
	}

	return len;
}

int func_serialize(const unsigned char * pdata, unsigned char * pbuf, const struct struct_entry * entry_in) {
	int len;
	len = serialize(pdata, pbuf+4, entry_in);
	*(int *)pbuf = htonl(len);
	len += 4;
	return len;
}

int func_deserialize(unsigned char * pdata, const unsigned char * pbuf, const struct struct_entry * entry_in) {
	int len;
	len = ntohl(*(int *)pbuf);
	len = deserialize(pdata, pbuf+4, len, entry_in);
	len += 4;
	return len;
}

static int verify_header(struct rmi_header * hdr) {
	if ((RMI_MAGIC == hdr->magic) && (SUPPORT_RMI_VER == hdr->ver) 
		&& (hdr->mtype >= RMI_MTYPE_MIN) && (hdr->mtype <= RMI_MTYPE_MAX)) {
		return 0;
	} else {
		return -1;
	}
}

int find_response(struct rmi_header * hdr, struct rmi_header * r_hdr)
{
	if (0 != verify_header(hdr)) {
		return -1;
	}

	if (RMI_RESPONSE == r_hdr->mtype) {
		if ((hdr->seq == r_hdr->seq) && (0 == r_hdr->stat) 
/*			&&(hdr->objid == r_hdr->objid) && (hdr->itfid == r_hdr->itfid) */
			&& (hdr->funcid == r_hdr->funcid))
			return 0;
	}

	return -1;
}

void gen_header(struct rmi_header * hdr, int id, int len, int seq) {
	hdr->magic = RMI_MAGIC;
	hdr->ver = 1;
	hdr->mtype = RMI_TWO_WAY_REQ;
	hdr->seq = seq;
	hdr->funcid = id;
	hdr->plen = len;
}

int invoke(struct rmi * rmi, int id, unsigned char * pbuf, int len, unsigned char ** r_buf, int * r_len) {
	int r_ret = 0;
	struct rmi_header hdr, hdr_orig;
	struct rmi_header r_hdr;

	if (rmi->fd <= 0) {
		int fd = -1;
	
		fd = create_tcp_client_socket(rmi->server_ip, rmi->server_port);
		if (fd < 0) {
			trace("create_tcp_client_socket failed; host[%s], port[%d]\n", host, port);
			return -1;
		}

		set_fd_nonblock(fd);
		
		rmi->fd = fd;
	}

	*r_buf = NULL;
	
	gen_header(&hdr, id, len, rmi->seq++);
	hdr_orig = hdr;
	serialize_rmi_header(&hdr);

	memcpy(pbuf, &hdr, sizeof(hdr));

#if 0
	// 发送消息头
	r_ret = nonblock_write(rmi->fd, (unsigned char *)&hdr, sizeof(hdr), rmi->timeout);
/*	r_ret = block_write(rmi->fd, (unsigned char *)&hdr, sizeof(hdr));*/
	if (r_ret < 0) {
		//trace("nonblock_write failed; err: %s\n", get_fd_error_str(r_ret));
		goto exit;
	}
#endif

	// 发送消息体
	r_ret = nonblock_write(rmi->fd, pbuf, len+sizeof(struct rmi_header), rmi->timeout);
/*	r_ret = block_write(rmi->fd, pbuf, len);*/
	if (r_ret < 0) {
		//trace("nonblock_write failed; err: %s\n", get_fd_error_str(r_ret));
		goto socket_error;
	}

	// 获取返回值
	r_ret = nonblock_read(rmi->fd, (unsigned char *)&r_hdr, sizeof(r_hdr), rmi->timeout);
/*	r_ret = block_read(rmi->fd, (unsigned char *)&r_hdr, sizeof(r_hdr));*/
	if (r_ret < 0) {
		//trace("nonblock_read failed; err: %s\n", get_fd_error_str(r_ret));
		goto socket_error;
	}
	deserialize_rmi_header(&r_hdr);

	mem_reset_pool(rmi->mem_pool);
	if (r_hdr.plen > 0) {
		*r_len = r_hdr.plen;
		*r_buf = mem_palloc(rmi->mem_pool, *r_len);
		if (NULL == *r_buf) {
			mem_reset_pool(rmi->mem_pool);
			trace("malloc error\n");
			r_ret = -1;
			goto exit;
		}
		r_ret = nonblock_read(rmi->fd, *r_buf, *r_len, rmi->timeout);
	/*	r_ret = block_read(rmi->fd, *r_buf, *r_len);*/
		if (r_ret < 0) {
			//trace("nonblock_read failed; err: %s\n", get_fd_error_str(r_ret));
			goto socket_error;
		}
	}

	if (0 != find_response(&hdr_orig, &r_hdr)) {
		trace("recv error\n");
		r_ret = -1;
		goto exit;
	}

	return 0;

socket_error:
	close_fd(rmi->fd);
	rmi->fd = -1;
	
exit:
	return r_ret;
}

static int response(struct rmi * rmi, struct rmi_header * hdr, int error_code) {
	int ret = 0;
	
	hdr->mtype = RMI_RESPONSE;
	hdr->stat = error_code;

	serialize_rmi_header(hdr);

/*	ret = nonblock_write(rmi->fd, (unsigned char *)hdr, sizeof(struct rmi_header), rmi->timeout);*/
/*	if (ret < 0) {*/
/*		trace("nonblock_write failed; err: %s\n", get_fd_error_str(ret));*/
/*		return -1;*/
/*	}*/

	return 0;
}

int rmi_cmp_fd(void * src, void * dst) {
	struct rmi * rmi_src = (struct rmi *)src;
	struct rmi * rmi_dst = *(struct rmi **)dst;

	if (rmi_src == rmi_dst && rmi_src->fd == rmi_dst->fd) {
		return 0;
	}

	return -1;
}

int rmi_operate_close(void * src, void * dst) {
	struct rmi * rmi_dst = *(struct rmi **)dst;
	rmi_dst->thread_start = 0;

	return 0;
}

void * rmi_listen_thread(void * arg) {
	int ret = 0;
	struct rmi * rmi = (struct rmi *)arg;	
	struct rmi * client_rmi = NULL;
	
	int client_fd = -1;
	
	while(rmi->thread_start) {
		if (0 != read_fd_timeout(rmi->fd, NULL, 0, rmi->timeout)) {
			continue;
		}
		client_fd = tcp_accept(rmi->fd);
		if (client_fd <= 0) {
			trace("tcp_accept error, fd: %d\n", rmi->fd);
			continue;						
		}
		if (pool_size(rmi->user_data) >= rmi->max_connect_num) {
			trace("connect is full\n");
			close_fd(client_fd);
			continue;
		}
		client_rmi = malloc(sizeof(struct rmi));
		if (NULL == client_rmi) {
			trace("malloc failed\n");
			close_fd(client_fd);
			continue;
		}

		client_rmi->mem_pool = mem_create_pool(rmi->mem_pool_size);
		if (NULL == client_rmi->mem_pool) {
			trace("mem_create_pool failed\n");
			close_fd(client_fd);
			continue;
		}
		client_rmi->mem_pool_size = rmi->mem_pool_size;

		set_fd_nonblock(client_fd);
		
		client_rmi->fd = client_fd;
		client_rmi->timeout = rmi->timeout;
		client_rmi->user_data = rmi;
		client_rmi->thread_start = 1;
/*		pthread_create(&client_rmi->pid, NULL, rmi_server_thread, (void *)client_rmi);*/
		thread_run(rmi_server_thread, (void *)client_rmi);
		pool_write_data(rmi->user_data, (unsigned char *)&client_rmi, sizeof(struct rmi *));
		
		//printf("!!!! connect num: %d !!!!!!!!!!\n", pool_size(rmi->user_data));
	}

	rmi->thread_start = 0;
	close_fd(rmi->fd);
	rmi->fd = -1;
	
	return NULL;
}

void * rmi_server_thread(void * arg) {
	struct rmi * rmi = (struct rmi *)arg;
	struct rmi * server_rmi = rmi->user_data;
	int ret = 0;
	struct rmi_header hdr;
	unsigned char * pdata;
	unsigned char * pbuf;
	int len;
	struct func_entry * func_entry;
	
/*	pthread_detach(pthread_self());*/

	rmi->user_data = NULL;

	while(rmi->thread_start) {
		int error_code = 0;
		
		if (0 != read_fd_timeout(rmi->fd, NULL, 0, rmi->timeout)) {
			continue;
		}
		mem_reset_pool(rmi->mem_pool);
		ret = nonblock_read(rmi->fd, (unsigned char *)&hdr, sizeof(hdr), rmi->timeout);
		if (ret < 0) {
			//trace("nonblock_read failed; err: %s\n", get_fd_error_str(ret));
			break;
		}
		deserialize_rmi_header(&hdr);
		if (0 != verify_header(&hdr)) {
			trace("verify_header failed\n");
			break;
		}
		pdata = mem_palloc(rmi->mem_pool, hdr.plen);
		if (NULL == pdata) {
			trace("malloc error\n");
			break;
		}
		ret = nonblock_read(rmi->fd, pdata, hdr.plen, rmi->timeout);
		if (ret < 0) {
			//trace("nonblock_read failed; err: %s\n", get_fd_error_str(ret));
			break;
		}

		do {			
			if ((func_entry = get_func_entry(hdr.funcid)) == NULL) {
				trace("req interface is not support now!\n");
				len = 0;
				error_code = 2;
				pbuf = (unsigned char *)&hdr;
				break;
			}	

			if (0 != func_entry->invoke_func(rmi, func_entry, pdata, hdr.plen, &pbuf, &len)) {
				trace("func[%d] failed!\n", hdr.funcid);
				len = 0;
				error_code = 3;
				pbuf = (unsigned char *)&hdr;
				break;
			}
		} while(0);

		hdr.plen = len;
		response(rmi, &hdr, error_code);
		if (len) {
			memcpy(pbuf, &hdr, sizeof(hdr));
		}

		ret = nonblock_write(rmi->fd, pbuf, len+sizeof(hdr), rmi->timeout);
		if (ret < 0) {
			//trace("nonblock_write failed; err: %s\n", get_fd_error_str(ret));
			break;
		}
	}

	rmi->thread_start = 0;
	pool_erase_it(server_rmi->user_data, rmi, rmi_cmp_fd);

	if (rmi->fd > 0) {
		close_fd(rmi->fd);
		rmi->fd = -1;
	}
	if (rmi->mem_pool) {
		mem_destroy_pool(rmi->mem_pool);
		rmi->mem_pool = NULL;
	}
	if (rmi->user_data) {
		free(rmi->user_data);
		rmi->user_data = NULL;
	}
	//printf("!!!! connect num2: %d !!!!!!!!!!\n", pool_size(server_rmi->user_data));

	free(rmi);

	return NULL;
}

int rmi_server_start(struct rmi * rmi, unsigned short port) {
	if (rmi->max_connect_num <= 0) {
		rmi->max_connect_num = RMI_MAX_CONNECT;
	}

	if (rmi->timeout <= 0) {
		rmi->timeout = RMI_DEFAULT_TIMEOUT;
	}

	if (rmi->mem_pool_size <= 0) {
		rmi->mem_pool_size = RMI_DEFAULT_POOL_SIZE;
	}
	
	rmi->fd = create_tcp_server_socket(port);
	if (rmi->fd < 0) {
		trace("create_tcp_server_socket failed; port[%d]\n", port);
		goto failed;
	}

	set_fd_nonblock(rmi->fd);

	rmi->user_data = malloc(sizeof(POOL_S));
	if (NULL == rmi->user_data) {
		trace("malloc failed\n");
		goto failed;
	}
	if (0 != pool_init(rmi->user_data, rmi->max_connect_num, sizeof(struct rmi *))) {
		trace("pool_init failed\n");
		goto failed;
	}

	rmi->thread_start = 1;
/*	pthread_create(&rmi->pid, NULL, rmi_listen_thread, (void *)rmi);*/
	rmi->thread_pool = tpool_create(1);
	msleep(1);
	tpool_start(rmi->thread_pool, rmi_listen_thread, (void *)rmi);

	return 0;

failed:
	if (rmi->fd > 0) {
		close_fd(rmi->fd);
	}
	if (rmi->user_data) {
		pool_finit(rmi->user_data);
		free(rmi->user_data);
		rmi->user_data = NULL;
	}
	return -1;
}

int rmi_server_close(struct rmi * rmi) {
	if (NULL == rmi) {
		trace("para error\n");
		return -1;
	}

	rmi->thread_start = 0;
/*	pthread_join(rmi->pid, 0);*/
	tpool_destroy(rmi->thread_pool);
	rmi->thread_pool = NULL;

	pool_operate_it(rmi->user_data, rmi_operate_close);

	while(pool_size(rmi->user_data)) {
		msleep(0);
	}	

	if (rmi->user_data) {
		pool_finit(rmi->user_data);
		free(rmi->user_data);
		rmi->user_data = NULL;
	}
	if (rmi->mem_pool) {
		mem_destroy_pool(rmi->mem_pool);
		rmi->mem_pool = NULL;
	}

	rmi_finit(rmi);

	return 0;
}

int rmi_client_start(struct rmi * rmi, char * host, unsigned short port) {
	int fd = -1;
	
	fd = create_tcp_client_socket(host, port);
	if (fd < 0) {
		trace("create_tcp_client_socket failed; host[%s], port[%d]\n", host, port);
		return -1;
	}

	set_fd_nonblock(fd);
	
	rmi->fd = fd;

	if (rmi->timeout <= 0) {
		rmi->timeout = RMI_DEFAULT_TIMEOUT;
	}

	if (rmi->mem_pool_size <= 0) {
		rmi->mem_pool_size = RMI_DEFAULT_POOL_SIZE;
	}

	rmi->mem_pool = mem_create_pool(rmi->mem_pool_size);
	if (NULL == rmi->mem_pool) {
		trace("mem_create_pool failed\n");
		return -1;
	}

	memcpy(rmi->server_ip, host, strlen(host));
	rmi->server_port = port;

	return 0;
}

int rmi_client_close(struct rmi * rmi) {
	if (rmi->fd > 0) {
		close_fd(rmi->fd);
		rmi->fd = -1;
	}
	if (rmi->user_data) {
		free(rmi->user_data);
		rmi->user_data = NULL;
	}

	if (rmi->mem_pool) {
		mem_destroy_pool(rmi->mem_pool);
		rmi->mem_pool = NULL;
	}

	rmi_finit(rmi);
	
	return 0;
}

#ifdef __cplusplus
}
#endif