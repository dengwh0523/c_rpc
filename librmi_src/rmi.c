#include "debug.h"
#include "rmi.h"
#include "socket.h"
#include "list.h"
#include "mem_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UDP_BURST_LEN	(32*1024)
#define BROADCAST_ADDR	"255.255.255.255"

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

int rmi_set_recv_buf_size(struct rmi * rmi, int buf_size) {
	rmi->recv_buf_size = buf_size;
	return 0;
}

int rmi_set_send_buf_size(struct rmi * rmi, int buf_size) {
	rmi->send_buf_size = buf_size;
	return 0;
}

int rmi_set_max_connect_num(struct rmi * rmi, int num) {
	rmi->max_connect_num = num;
	return 0;
}

int rmi_set_keepalive_time(struct rmi * rmi, int time) {
	rmi->keepalive_time = time;
	return 0;
}

void rmi_set_default_para(struct rmi * rmi) {
	if (rmi->max_connect_num <= 0) {
		rmi->max_connect_num = RMI_MAX_CONNECT;
	}

	if (rmi->timeout <= 0) {
		rmi->timeout = RMI_DEFAULT_TIMEOUT;
	}

	if (rmi->mem_pool_size <= 0) {
		rmi->mem_pool_size = RMI_DEFAULT_POOL_SIZE;
	}

	if (rmi->recv_buf_size <= 0) {
		rmi->recv_buf_size = RMI_DEFAULT_RECV_BUF_SIZE;
	}

	if (rmi->send_buf_size <= 0) {
		rmi->send_buf_size = RMI_DEFAULT_RECV_BUF_SIZE;
	}

	if (rmi->keepalive_time <= 0) {
		rmi->keepalive_time = RMI_DEFAULT_KEEPALIVE_TIME;
	}
}

int rmi_set_socket_type(struct rmi * rmi, int socket_type) {
	rmi->socket_type = socket_type;
	return 0;
}

int rmi_set_broadcast(struct rmi * rmi) {
	rmi->broadcast = 1;
	return 0;
}

static int struct_name_cmp(void * data, void * id) {
	return strcmp(id, ((struct struct_pair_entry *)data)->name);
}

static struct struct_pair_entry * get_struct_entry(struct rmi * rmi, char * struct_name) {
	return FOR_EACH_WITH_NUM(rmi->struct_pair, rmi->struct_num, struct struct_pair_entry, struct_name, struct_name_cmp);
}

static int func_id_cmp(void * data, void * id) {
	unsigned int dst_id = (unsigned int)id;
	unsigned int src_id = ((struct func_entry *)data)->func_id;
	if (dst_id == src_id) {
		return 0;
	}

	return (dst_id>src_id) ? 1 : -1;
}

struct func_entry * get_func_entry(struct rmi * rmi, int id) {
	return FAST_FOR_EACH_WITH_NUM(rmi->func_table, rmi->func_num, struct func_entry, id, func_id_cmp);
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
	value = (value << 1) ^ (value >> (sizeof(long long)*8-1));
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

int serialize(struct rmi * rmi, const unsigned char * pdata, unsigned char * pbuf, const struct struct_entry * entry_in) {
	int i, j;
	struct struct_pair_entry * pair_entry;
	const struct struct_entry * entry;
	int member_num;
	int len = 0;
	
	if (entry_in->type_name && (pair_entry = get_struct_entry(rmi, entry_in->type_name))) {
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
				str_len = serialize(rmi, base, buf, &entry[i]);
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

int deserialize(struct rmi * rmi, unsigned char * pdata, const unsigned char * pbuf, const int enc_len, const struct struct_entry * entry_in) {
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
	
	if (entry_in->type_name && (pair_entry = get_struct_entry(rmi, entry_in->type_name))) {
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
						len += deserialize(rmi, base, pbuf+len, str_len, &entry[i]);
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

int func_serialize(struct rmi * rmi, const unsigned char * pdata, unsigned char * pbuf, const struct struct_entry * entry_in) {
	int len;
	len = serialize(rmi, pdata, pbuf+4, entry_in);
	*(int *)pbuf = htonl(len);
	len += 4;
	return len;
}

int func_deserialize(struct rmi * rmi, unsigned char * pdata, const unsigned char * pbuf, const struct struct_entry * entry_in) {
	int len;
	len = ntohl(*(int *)pbuf);
	len = deserialize(rmi, pdata, pbuf+4, len, entry_in);
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

int rmi_recv(struct rmi * rmi, unsigned char ** r_buf, int * r_len) {
	int ret = 0;
	struct rmi_header hdr;
	int len;
	unsigned char * pdata = NULL;
	
	do {
		mem_reset_pool(rmi->mem_pool);
		ret = nonblock_read(rmi->fd, (unsigned char *)&hdr, sizeof(hdr), rmi->timeout);
		if (ret < 0) {
			//trace("nonblock_read failed; err: %s\n", get_fd_error_str(ret));
			break;
		}
		deserialize_rmi_header(&hdr);
		if (0 != verify_header(&hdr)) {
			trace("verify_header failed\n");
			ret = -1;
			break;
		}
		len = hdr.plen + sizeof(hdr);
		pdata = mem_palloc(rmi->mem_pool, len);
		if (NULL == pdata) {
			trace("malloc error\n");
			break;
		}
		*r_buf = pdata;
		*r_len = hdr.plen;

		pdata += sizeof(hdr);
		len -= sizeof(hdr);
		while(len) {
			int rd_len;
			if (len > UDP_BURST_LEN) {
				rd_len = UDP_BURST_LEN;
			} else {
				rd_len = len;
			}			
			ret = nonblock_read(rmi->fd, pdata, rd_len, rmi->timeout);
			if (ret < 0) {
				//trace("nonblock_read failed; err: %s\n", get_fd_error_str(ret));
				break;
			}
			pdata += rd_len;
			len -= rd_len;
		}

		memcpy(*r_buf, &hdr, sizeof(hdr));
	} while(0);
	
	return ret;
}

int rmi_send(struct rmi * rmi, unsigned char * pbuf, int len) {
	int ret = 0;
	if (RMI_SOCKET_TCP == rmi->socket_type) {
		ret = nonblock_write(rmi->fd, pbuf, len, rmi->timeout);
		if (ret < 0) {
			//trace("nonblock_write failed; err: %s\n", get_fd_error_str(ret));
			return ret;
		}		
 	} else {
 		if (rmi->broadcast) {
			ret = udp_send(rmi->fd, pbuf, sizeof(struct rmi_header), BROADCAST_ADDR, rmi->peer_port);
 		} else {
			ret = nonblock_write(rmi->fd, pbuf, sizeof(struct rmi_header), rmi->timeout);
 		}
		if (ret < 0) {
			//trace("nonblock_write failed; err: %s\n", get_fd_error_str(ret));
			return ret;
		}
		len -= sizeof(struct rmi_header);
		pbuf += sizeof(struct rmi_header);
		while(len) {
			int wr_len;
			if (len > UDP_BURST_LEN) {
				wr_len = UDP_BURST_LEN;
			} else {
				wr_len = len;
			} 		
			if (rmi->broadcast) {
				ret = udp_send(rmi->fd, pbuf, wr_len, BROADCAST_ADDR, rmi->peer_port);
			} else {
				ret = nonblock_write(rmi->fd, pbuf, wr_len, rmi->timeout);
			}
			if (ret < 0) {
				//trace("nonblock_write failed; err: %s\n", get_fd_error_str(ret));
				return ret;
			}
			pbuf += wr_len;
			len -= wr_len;
		}
	}

	return 0;
}

int udp_accept(struct rmi * rmi) {
	int ret = 0;	
	struct rmi_header hdr;
	
	char src_ip[16] = {0};
	unsigned short port;

	int client_fd;
	char * host;
	
	ret = udp_recv(rmi->fd, (unsigned char *)&hdr, sizeof(hdr), src_ip, &port);
	if (sizeof(hdr) != ret) {
		return -1;
	}
	serialize_rmi_header(&hdr);
	
/*	if (0 != rmi->peer_ip[0]) {*/
/*		memcpy(src_ip, rmi->peer_ip, strlen(rmi->peer_ip));*/
/*		src_ip[strlen(rmi->peer_ip)] = 0;*/
/*	}*/
/**/
/*	if (rmi->peer_port > 0) {*/
/*		port = rmi->peer_port;*/
/*	}*/
	strcpy(rmi->peer_ip, src_ip);
	rmi->peer_port = port;
	
	if (rmi->broadcast) {
		host = NULL;
	} else {
		host = src_ip;
	}
	client_fd = create_udp_client_socket(host, port);
	if (client_fd <= 0) {
		trace("create_udp_client_socket\n");
		return -1;;						
	}

	response(NULL, &hdr, 0);

	if (rmi->broadcast) {
		ret = udp_send(client_fd, (unsigned char *)&hdr, sizeof(hdr), BROADCAST_ADDR, port);
		if (ret <= 0) {
			trace("udp_send failed\n");
			return -1;
		}
	} else {
		ret = nonblock_write(client_fd, (unsigned char *)&hdr, sizeof(hdr), rmi->timeout);
		if (ret < 0) {
			trace("nonblock_write failed\n");
			close_fd(client_fd);
			return -1;
		}
	}

	ret = nonblock_read(client_fd, (unsigned char *)&hdr, sizeof(hdr), rmi->timeout);
	if (ret < 0) {
		trace("nonblock_read failed\n");
		close_fd(client_fd);
		return -1;
	}

	set_fd_sendbuf(client_fd, rmi->send_buf_size);
	set_fd_recvbuf(client_fd, rmi->recv_buf_size);

	return client_fd;
}

int udp_connect(struct rmi * rmi, char * host, unsigned short port) {
	int fd = -1;
	int ret = 0;
	
	char dst_ip[16] = {0};
	unsigned short dst_port;
	struct rmi_header hdr, hdr_orig, r_hdr;
	int len;
	
	fd = create_udp_client_socket(NULL, port);
	if (fd < 0) {
		trace("create_udp_client_socket failed; host[%s], port[%d]\n", host, port);
		return -1;
	}

	// sync to server
	gen_header(&hdr, 0, 0, 0);
	hdr_orig = hdr;
	serialize_rmi_header(&hdr);
	
	ret = udp_send(fd, (unsigned char *)&hdr, sizeof(hdr), host, port);
	if (ret < 0) {
		trace("udp_send failed\n");
		goto failed;
	}

	if (0 != read_fd_timeout(fd, NULL, 0, rmi->timeout)) {
		goto failed;
	}
	
	len = udp_recv(fd, (unsigned char *)&r_hdr, sizeof(r_hdr), dst_ip, &dst_port);
	if (len != sizeof(r_hdr)) {
		trace("recv error\n");
		goto failed;
	}
	serialize_rmi_header(&r_hdr);
	
	if (0 != find_response(&hdr_orig, &r_hdr)) {
		trace("recv error\n");
		goto failed;
	}	
	
/*	if (0 != rmi->peer_ip[0]) {*/
/*		memcpy(dst_ip, rmi->peer_ip, strlen(rmi->peer_ip));*/
/*		dst_ip[strlen(rmi->peer_ip)] = 0;*/
/*	}*/
/**/
/*	if (rmi->peer_port > 0) {*/
/*		dst_port = rmi->peer_port;*/
/*	}*/
	strcpy(rmi->peer_ip, dst_ip);
	rmi->peer_port = dst_port;

	if (rmi->broadcast) {
		ret = udp_send(fd, (unsigned char *)&hdr, sizeof(hdr), BROADCAST_ADDR, dst_port);
		if (ret <= 0) {
			trace("udp_send failed\n");
			goto failed;
		}
	} else {
		udp_set_dst(fd, dst_ip, dst_port);
		
		ret = nonblock_write(fd, (unsigned char *)&hdr, sizeof(hdr), rmi->timeout);
		if (ret < 0) {
			trace("nonblock_write failed\n");
			goto failed;
		}
	}
	rmi->fd = fd;	

	set_fd_sendbuf(fd, rmi->send_buf_size);
	set_fd_recvbuf(fd, rmi->recv_buf_size);

	return 0;

failed:
	close_fd(fd);
	rmi->fd = -1;
	return -1;
}

int rmi_create_sock(struct rmi * rmi, unsigned short port) {	
	switch(rmi->socket_type) {
	case RMI_SOCKET_UDP : 
		rmi->fd = create_udp_server_socket(port);
		if (rmi->fd < 0) {
			trace("create_udp_server_socket failed; port[%d]\n", port);
			goto failed;
		}
		break;
	case RMI_SOCKET_TCP :
		rmi->fd = create_tcp_server_socket(port);
		if (rmi->fd < 0) {
			trace("create_tcp_server_socket failed; port[%d]\n", port);
			goto failed;
		}
		break;
	default :
		trace("socket type[%d] not support now!\n", rmi->socket_type);
		goto failed;
	}

	set_fd_nonblock(rmi->fd);

	return 0;

failed:
	return -1;
}

int rmi_accept(struct rmi * rmi) {
	int client_fd;
	if (RMI_SOCKET_TCP == rmi->socket_type) {
		client_fd = tcp_accept(rmi->fd);
		if (client_fd <= 0) {
			trace("tcp_accept error, fd: %d\n", rmi->fd);
			return -1;						
		}
	} else {
		client_fd = udp_accept(rmi);
		if (client_fd <= 0) {
			trace("create_udp_client_socket\n");
			return -1;						
		}
	}

	set_fd_nonblock(client_fd);

	return client_fd;
}

int rmi_conncet(struct rmi * rmi, char * host, unsigned short port) {
	int fd = -1;

	switch(rmi->socket_type) {
	case RMI_SOCKET_UDP : 
		if (0 != udp_connect(rmi, host, port)) {
			trace("udp_connect failed\n");
			goto failed;
		}
		break;
	case RMI_SOCKET_TCP :	
		fd = create_tcp_client_socket(host, port);
		if (fd < 0) {
			trace("create_tcp_client_socket failed; host[%s], port[%d]\n", host, port);
			return -1;
		}	
		rmi->fd = fd;
		break;
	default :
		trace("socket type[%d] not support now!\n", rmi->socket_type);
		goto failed;
	}

	set_fd_nonblock(rmi->fd);

	return 0;

failed:
	return -1;
}

int rmi_disconncet(struct rmi * rmi) {
	struct rmi_header hdr;
	
	switch(rmi->socket_type) {
	case RMI_SOCKET_UDP : 
		gen_header(&hdr, 0, 0, 0);
		hdr.mtype = RMI_CLOSE;
		serialize_rmi_header(&hdr);
		rmi_send(rmi, (unsigned char *)&hdr, sizeof(hdr));
		msleep(1);
		rmi_send(rmi, (unsigned char *)&hdr, sizeof(hdr));
		msleep(1);
		rmi_send(rmi, (unsigned char *)&hdr, sizeof(hdr));
		break;
	case RMI_SOCKET_TCP :	
		break;
	default :
		trace("socket type[%d] not support now!\n", rmi->socket_type);
		return -1;
	}
	
	if (rmi->fd > 0) {
		close_fd(rmi->fd);
		rmi->fd = -1;
	}

	return 0;
}

int invoke(struct rmi * rmi, int id, unsigned char * pbuf, int len, unsigned char ** r_buf, int * r_len) {
	int r_ret = 0;
	struct rmi_header hdr, hdr_orig;

	if (rmi->fd <= 0) {
		r_ret = rmi_conncet(rmi, rmi->server_ip, rmi->server_port);
		if (r_ret < 0) {
			trace("rmi_conncet failed; host[%s], port[%d]\n", rmi->server_ip, rmi->server_port);
			return -1;
		}
	}

	*r_buf = NULL;
	
	gen_header(&hdr, id, len, rmi->seq++);
	hdr_orig = hdr;
	serialize_rmi_header(&hdr);

	memcpy(pbuf, &hdr, sizeof(hdr));

	// 发送消息
	r_ret = rmi_send(rmi, pbuf, len+sizeof(struct rmi_header));
	if (r_ret < 0) {
		//trace("nonblock_write failed; err: %s\n", get_fd_error_str(ret));
		goto socket_error;
	}
	

	// 获取返回值
	r_ret = rmi_recv(rmi, r_buf, r_len);
	if (r_ret < 0) {
		//trace("rmi_recv failed; err: %s\n", get_fd_error_str(ret));
		goto socket_error;
	}

	if (0 != find_response(&hdr_orig, (struct rmi_header *)(*r_buf))) {
		trace("recv error\n");
		r_ret = -1;
		goto exit;
	}

	*r_buf += sizeof(struct rmi_header);

	return 0;

socket_error:
	rmi_disconncet(rmi);
	
exit:
	return r_ret;
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
		client_fd = rmi_accept(rmi);
		if (client_fd < 0) {
			trace("rmi_accept failed\n");
			continue;
		}
		if (pool_size(rmi->user_data) >= rmi->max_connect_num) {
			trace("connect is full\n");
			close_fd(client_fd);
			continue;
		}
		client_rmi = calloc(1, sizeof(struct rmi));
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

/*		set_fd_nonblock(client_fd);*/
		
		client_rmi->fd = client_fd;
		client_rmi->timeout = rmi->timeout;
		client_rmi->user_data = rmi;
		client_rmi->thread_start = 1;

		// copy some server rmi info to client rmi
		client_rmi->socket_type = rmi->socket_type;
		client_rmi->recv_buf_size = rmi->recv_buf_size;
		client_rmi->send_buf_size = rmi->send_buf_size;
		client_rmi->keepalive_time = rmi->keepalive_time;
		client_rmi->broadcast = rmi->broadcast;
		client_rmi->peer_port = rmi->peer_port;
		strcpy(client_rmi->peer_ip, rmi->peer_ip);
		
		client_rmi->struct_num = rmi->struct_num;
		client_rmi->struct_pair = rmi->struct_pair;
		client_rmi->func_num = rmi->func_num;
		client_rmi->func_table = rmi->func_table;
		client_rmi->return_para = rmi->return_para;
	
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
	struct rmi_header * p_hdr;
	unsigned char * pdata;
	unsigned char * pbuf;
	int len;
	struct func_entry * func_entry;
	
/*	pthread_detach(pthread_self());*/

	rmi->user_data = NULL;

	while(rmi->thread_start) {
		int error_code = 0;
		
		if (0 != read_fd_timeout(rmi->fd, NULL, 0, rmi->keepalive_time)) {
			printf("keepalive timeout\n");
			break;
		}

		ret = rmi_recv(rmi, &pdata, &len);	
		if (ret < 0) {
			//trace("rmi_recv failed; err: %s\n", get_fd_error_str(ret));
			break;
		}

		p_hdr = (struct rmi_header *)pdata;
		pdata += sizeof(struct rmi_header);

		// exit from udp
		if (RMI_CLOSE == p_hdr->mtype) {
			break;
		}

		do {			
			if ((func_entry = get_func_entry(rmi, p_hdr->funcid)) == NULL) {
				trace("req interface is not support now!\n");
				len = 0;
				error_code = 2;
				pbuf = (unsigned char *)p_hdr;
				break;
			}	

			if (0 != func_entry->invoke_func(rmi, func_entry, pdata, p_hdr->plen, &pbuf, &len)) {
				trace("func[%d] failed!\n", p_hdr->funcid);
				len = 0;
				error_code = 3;
				pbuf = (unsigned char *)p_hdr;
				break;
			}
		} while(0);

		p_hdr->plen = len;
		response(rmi, p_hdr, error_code);
		if (len) {
			memcpy(pbuf, p_hdr, sizeof(struct rmi_header));
		}
		
		ret = rmi_send(rmi, pbuf, len+sizeof(struct rmi_header));
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
/*	printf("!!!! connect num2: %d !!!!!!!!!!\n", pool_size(server_rmi->user_data));*/

	free(rmi);

	return NULL;
}

int rmi_server_start(struct rmi * rmi, unsigned short port) {
	rmi_set_default_para(rmi);

	if (0 != rmi_create_sock(rmi, port)) {
		trace("rmi_create_sock failed\n");
		goto failed;
	}

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
	
	rmi_set_default_para(rmi);

	rmi->mem_pool = mem_create_pool(rmi->mem_pool_size);
	if (NULL == rmi->mem_pool) {
		trace("mem_create_pool failed\n");
		return -1;
	}

	if (0 != rmi_conncet(rmi, host, port)) {
		trace("rmi_conncet failed\n");
		goto failed;
	}
	
	memcpy(rmi->server_ip, host, strlen(host));
	rmi->server_port = port;

	return 0;

failed:
	if (rmi->fd > 0) {
		close_fd(rmi->fd);
	}
	
	if (rmi->user_data) {
		free(rmi->user_data);
		rmi->user_data = NULL;
	}

	if (rmi->mem_pool) {
		mem_destroy_pool(rmi->mem_pool);
		rmi->mem_pool = NULL;
	}
	return -1;
}

int rmi_client_close(struct rmi * rmi) {
	rmi_disconncet(rmi);
	
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
