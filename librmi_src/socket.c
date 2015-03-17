#include <stdio.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <string.h>
#include <sys/epoll.h>

#include "socket.h"
#include "debug.h"

typedef struct {
	int err_no;
	char *description;
} FD_ERROR_S;

const FD_ERROR_S fd_error_table[] = {
	{FD_SUCCESS, "fd success"},
	{FD_ERROR, "fd error"},
	{FD_CLOSED, "fd closed"},
	{FD_TIMEOUT, "fd timeout"},
};

char * get_fd_error_str(int err_no) {
	int array_len = STR_ARRAY_LEN(fd_error_table);
	int i;
	for(i = 0; i < array_len; i++) {
		if (fd_error_table[i].err_no == err_no) {
			return fd_error_table[i].description;
		}
	}
	return NULL;
}

int ip_to_net(char * ip) {
	return inet_addr(ip);
}

char * net_to_ip(int addr) {	
	struct in_addr in;
	
	in.s_addr = addr;
	return (char *)inet_ntoa(in);
}

int create_epoll(int maxfd) {
	int fd = -1;
	fd = epoll_create(maxfd);
	if (fd <= 0) {
		trace("epoll_create failed\n");
		return -1;
	}

	return fd;
}

int close_fd(int fd) {
	if (fd > 0) {
		close(fd);
	}
	return 0;
}

int epoll_envent_add(int epollfd, int fd, int state, int mode, void * ptr) {
	struct epoll_event ev;	
	
	if (FD_RD == state) {
		state = EPOLLIN/*|EPOLLET*/;
	} else if (FD_WR == state) {
		state = EPOLLOUT/*|EPOLLET*/;
	} else {
		state = EPOLLIN | EPOLLOUT;
	}
	if (mode) {
		state |= EPOLLET;
	}
	ev.events = state;
	if (ptr) {
		ev.data.ptr = ptr;
	} else {
		ev.data.fd = fd;
	}
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);

	return 0;
}

int epoll_envent_delete(int epollfd, int fd) {
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);

	return 0;
}

int epoll_envent_modify(int epollfd, int fd, int state, int mode, void * ptr) {
	struct epoll_event ev;
	
	if (FD_RD == state) {
		state = EPOLLIN/*|EPOLLET*/;
	} else if (FD_WR == state) {
		state = EPOLLOUT/*|EPOLLET*/;
	} else {
		state = EPOLLIN | EPOLLOUT;
	}
	if (mode) {
		state |= EPOLLET;
	}
	ev.events = state;
	if (ptr) {
		ev.data.ptr = ptr;
	} else {
		ev.data.fd = fd;
	}
	epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);

	return 0;
}

int epoll_wait_data(int epollfd, int * fd_array, const int len, int timeout) {
	struct epoll_event events[len];
	int n;
	int i;

	n = epoll_wait(epollfd, events, len, timeout);
	if (n <= 0) {
		return -1;
	}
	for (i = 0; i < n; i++) {
		fd_array[i] = events[i].data.fd;
	}
	return n;
}

int epoll_read_data(int fd, unsigned char * buf, int buf_size) {
	int rd_len;
	rd_len = recv(fd, buf, buf_size, 0);
	if (rd_len < 0) {
		if (EAGAIN == errno) {
			return 0;
		} else {
			return -1;
		}
	} else if (0 == rd_len) {
		return -1;
	} else {
		return rd_len;
	}
}

int create_tcp_server_socket(unsigned short port) {
	int ret = 0;
	int fd = -1;
	struct sockaddr_in bind_addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		trace("socket error\n");
		return -1;
	}
	{
		int reuse = 1;	
		if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,(char*)&reuse, sizeof(int)) != 0) {		
			trace("setsocketopt error\n");
			ret = -1;
			goto exit;
		}
	}
	
	bind_addr.sin_family = AF_INET;
	bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind_addr.sin_port = htons(port);
	
	if (bind(fd, (struct sockaddr *)&bind_addr, sizeof(bind_addr)) != 0) {
		trace("bind error: %s\n", strerror(errno));
		ret = -1;
		goto exit;
	}
	
	if (listen(fd, 16) != 0) {
		trace("listen error\n");
		ret = -1;
		goto exit;
	}
exit:
	if (ret < 0) {
		if (fd > 0) {
			close_fd(fd);
		}
		fd = -1;
	}
	return fd;
}

int create_tcp_client_socket(char * dst_ip, unsigned short port) {
	int fd = -1;
	struct sockaddr_in dest_addr;
	
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_addr.s_addr = ip_to_net(dst_ip);
	dest_addr.sin_port = htons(port);
	
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		trace("sokcet error\n");
		return -1;
	}
#if 1
	{
		int sopt = 1;
		setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *)&sopt, sizeof(int));
	}
#endif
	
	if (connect(fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) != 0) {
		trace("connet error\n");
		close_fd(fd);
		return -1;
	}

	return fd;
}

int tcp_accept(int fd) {
	struct sockaddr_in addr;
	int addr_len = sizeof(struct sockaddr);
	int client_fd;
	
	client_fd = accept(fd, (struct sockaddr *)&addr, &addr_len);
	if (client_fd < 0) {
		 trace("%s\n", strerror(errno));
	}

	return client_fd;
}

int create_udp_server_socket(unsigned short port) {
	int ret = 0;
	int fd = -1;
	struct sockaddr_in bind_addr;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		trace("socket error\n");
		return -1;
	}
	
	if (set_fd_reuse(fd) != 0) {	
		trace("set_fd_reuse error\n");
		ret = -1;
		goto exit;
	}
	
	bind_addr.sin_family = AF_INET;
	bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind_addr.sin_port = htons(port);
	
	if (bind(fd, (struct sockaddr *)&bind_addr, sizeof(bind_addr)) != 0) {
		trace("bind error: %s\n", strerror(errno));
		ret = -1;
		goto exit;
	}
	
exit:
	if (ret < 0) {
		if (fd > 0) {
			close_fd(fd);
		}
		fd = -1;
	}
	return fd;
}

int create_udp_client_socket(char * dst_ip, unsigned short port) {
	int fd;
	struct sockaddr_in dest_addr;
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		trace("socket error\n");
		return -1;
	}

	if (dst_ip) {
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_addr.s_addr = ip_to_net(dst_ip);
		dest_addr.sin_port = htons(port);
		
		if (connect(fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) != 0) {
			trace("connet error\n");
			close_fd(fd);
			return -1;
		}
	}
	
	return fd;
}

int udp_set_dst(int fd, char * dst_ip, unsigned short port) {
	struct sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_addr.s_addr = ip_to_net(dst_ip);
	dest_addr.sin_port = htons(port);
	
	if (connect(fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) != 0) {
		trace("connet error\n");
		close_fd(fd);
		return -1;
	}
	return 0;
}

int udp_send(int fd, unsigned char * pbuf, int len, char * dst_ip, unsigned short port) {
	struct sockaddr_in dst_addr;
	
	memset(&dst_addr, 0, sizeof(dst_addr));
	dst_addr.sin_family = AF_INET;	 
	dst_addr.sin_addr.s_addr = inet_addr(dst_ip);
	dst_addr.sin_port = htons(port);

	sendto(fd, pbuf, len, 0, (struct sockaddr *)&dst_addr, sizeof(dst_addr));

	return 0;
}

int udp_recv(int fd, unsigned char * pbuf, int len, char * src_ip, unsigned short * port ) {
	struct sockaddr_in addr;
	int addr_len = sizeof(addr);

	len = recvfrom(fd, pbuf, len, 0, (struct sockaddr *)&addr, &addr_len);

	if (len <= 0) {
		return -1;
	}

	if (port) {
		*port = ntohs(addr.sin_port);
	}
	if (src_ip) {
		strcpy(src_ip, net_to_ip(addr.sin_addr.s_addr));
	}

	return len;
}

int get_peer_info(int fd, char * src_ip, unsigned short * src_port) {
	struct sockaddr_in addr;
	int addrlen = sizeof(addr);
	if (getpeername(fd, (struct sockaddr *)&addr, &addrlen) != 0) {
        trace("getpeer_information: getpeername() error: %s\n", strerror(errno));
        return -1;
     } else {
     	if (src_ip) {
        	strcpy(src_ip, net_to_ip(addr.sin_addr.s_addr));
     	}
		if (src_port) {
			*src_port = ntohs(addr.sin_port);
		}
     }
	 return 0;
}

int get_host_info(int fd, char * src_ip, unsigned short * src_port) {
	struct sockaddr_in addr;
	int addrlen = sizeof(addr);
	if (getsockname(fd, (struct sockaddr *)&addr, &addrlen) != 0) {
        trace("getpeer_information: getsockname() error: %s\n", strerror(errno));
        return -1;
     } else {
     	if (src_ip) {
        	strcpy(src_ip, net_to_ip(addr.sin_addr.s_addr));
     	}
		if (src_port) {
			*src_port = ntohs(addr.sin_port);
		}
     }

	 return 0;
}


int set_fd_nonblock(int fd) {	
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0) {
	   trace("fcntl socket error!\n");
	   return -1;
	}

	if (-1 == fcntl(fd, F_SETFL, flags | O_NONBLOCK)) {
	   trace("fcntl socket error!\n");
	   return -1;
	}

	return 0;
}

int set_fd_reuse(int fd) {
	int reuse = 1;	
	return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,(char*)&reuse, sizeof(int));
}

int read_fd_timeout(int fd, unsigned char * pbuf, int len, int timeout/* unit: ms */) {
	int rd_len;
	
	fd_set fdsr;
	struct timeval tv;
	int sel_ret = -1;
		
	FD_ZERO(&fdsr);
	FD_SET(fd, &fdsr);

	tv.tv_sec = timeout/1000;
	tv.tv_usec = timeout%1000 * 1000;

	sel_ret = select(fd + 1, &fdsr, NULL, NULL, &tv);

	if( sel_ret < 0 )
	{
		trace("select error !");
		return -1;
	}
	else if( sel_ret == 0 )
	{
/*		trace("recv timeout\n");*/
/*		return -1;*/
		return FD_TIMEOUT;
	}

	if (pbuf) {
		rd_len = read(fd, pbuf, len);
	} else {
		return 0;
	}

	if( rd_len <= 0 )
	{
/*		trace("fd closed\n");*/
/*		return -1;*/
		return FD_CLOSED;
	}

	return rd_len;
}

int write_fd_timeout(int fd, unsigned char * pbuf, int len, int timeout/*ms*/) {
	int wr_len;
	
	fd_set fdsw;
	struct timeval tv;
	int sel_ret = -1;
		
	FD_ZERO(&fdsw);
	FD_SET(fd, &fdsw);

	tv.tv_sec = timeout/1000;
	tv.tv_usec = timeout%1000 * 1000;

	sel_ret = select(fd + 1, NULL, &fdsw, NULL, &tv);

	if( sel_ret < 0 )
	{
		trace("select error !");
		return -1;
	}
	else if( sel_ret == 0 )
	{
/*		trace("send timeout\n");*/
/*		return -1;*/
		return FD_TIMEOUT;
	}

	wr_len = write(fd, pbuf, len);

	if( wr_len <= 0 )
	{
/*		trace("fd closed\n");*/
/*		return -1;*/
		return FD_CLOSED;
	}

	return wr_len;
}

int nonblock_read(int fd, unsigned char * pbuf, int len, int timeout) {
	int rd_len;
	int ret;
	
	rd_len = 0;
	while(rd_len != len) {
		ret = read_fd_timeout(fd, pbuf+rd_len, len-rd_len, timeout);
		if (ret <= 0) {
/*			trace("recv error\n");*/
/*			return -1;*/
			return ret;
		}
		rd_len += ret;
		ret = 0;
	}

	return 0;
}

int nonblock_write(int fd, unsigned char * pbuf, int len, int timeout) {
	int wr_len;
	int ret;
	
	wr_len = 0;
	while(wr_len != len) {
		ret = write_fd_timeout(fd, pbuf+wr_len, len-wr_len, timeout);
		if (ret <= 0) {
/*			trace("send error\n");*/
/*			return -1;*/
			return ret;
		}
		wr_len += ret;
		ret = 0;
	}

	return 0;
}

int block_read(int fd, unsigned char * pbuf, int len) {
	int rd_len = 0;
	int ret = 0;
	
	while(rd_len != len) {
		ret = read(fd, pbuf+rd_len, len-rd_len);
		if (ret < 0) {
			if (EAGAIN == errno || EINTR == errno) {
				continue;
			} else {
				//trace("recv error\n");
				ret = FD_ERROR;
				break;
			}
		} else if (ret == 0) {
			//trace("socket closed\n");
			ret = FD_CLOSED;
			break;
		} else {
			rd_len += ret;
		}
		ret = 0;
	}
	
	if (ret < 0) {
		perror("read");
	} 

	return ret;
}

int block_write(int fd, unsigned char * pbuf, int len) {
	int wr_len = 0;
	int ret = 0;
	
	while(wr_len != len) {
		ret = write(fd, pbuf+wr_len, len-wr_len);
		if (ret < 0) {
			if (EAGAIN == errno || EINTR == errno) {
				continue;
			} else {
				//trace("send error\n");
				ret = FD_ERROR;
				break;
			}
		} else if (ret == 0) {
			//trace("socket closed\n");
			ret = FD_CLOSED;
			break;
		} else {
			wr_len += ret;
		}
		ret = 0;
	}
	
	if (ret < 0) {
		perror("write");
	}
	
	return ret;
}

