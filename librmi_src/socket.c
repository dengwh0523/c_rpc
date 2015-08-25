#ifndef _WIN32
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <sys/epoll.h>
#else
#include<winsock2.h>
#include <Ws2tcpip.h>
#include <io.h>
#define close	closesocket
#endif

#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "socket.h"
#include "debug.h"

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

#ifdef __cplusplus
extern "C" {
#endif

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

int close_fd(int fd) {
	if (fd > 0) {
		close(fd);
	}
	return 0;
}

#ifdef _WIN32
int socket_init() {
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(1, 1);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
		return -1;

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		WSASetLastError (WSAVERNOTSUPPORTED);
		return -1;
	}

	return 0;
}
#endif

#ifndef _WIN32
int create_epoll(int maxfd) {
	int fd = -1;
	fd = epoll_create(maxfd);
	if (fd <= 0) {
		trace("epoll_create failed\n");
		return -1;
	}

	return fd;
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
#endif

int create_tcp_server_socket(unsigned short port) {
	int ret = 0;
	int fd = -1;
	struct sockaddr_in bind_addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
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
	
	if (listen(fd, 1024) != 0) {
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
	
	set_fd_broadcast(fd);
	
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
	set_fd_broadcast(fd);

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

	return sendto(fd, pbuf, len, 0, (struct sockaddr *)&dst_addr, sizeof(dst_addr));
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
#ifndef _WIN32
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0) {
	   trace("fcntl socket error!\n");
	   return -1;
	}

	if (-1 == fcntl(fd, F_SETFL, flags | O_NONBLOCK)) {
	   trace("fcntl socket error!\n");
	   return -1;
	}
#endif

	return 0;
}

int set_fd_reuse(int fd) {
	int reuse = 1;	
	return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,(char*)&reuse, sizeof(int));
}

int set_fd_recvbuf(int fd, int recv_buf) {
	return setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char *)&recv_buf, sizeof(recv_buf));
}

int set_fd_sendbuf(int fd, int send_buf) {
	return setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char *)&send_buf, sizeof(send_buf));
}

int set_fd_broadcast(int fd) {
	int bbroadcast = 1;	
	return setsockopt(fd, SOL_SOCKET, SO_BROADCAST,(char*)&bbroadcast,sizeof(int));
}

int read_fd_timeout(int fd, unsigned char * pbuf, int len, int timeout/* unit: ms */) {
	int rd_len;
	
	fd_set fdsr;
	struct timeval tv;
	int sel_ret = -1;
		
	FD_ZERO(&fdsr);
	FD_SET(fd, &fdsr);

	tv.tv_sec = timeout/1000;
	tv.tv_usec = timeout%1000;
#ifndef _WIN32
	tv.tv_usec *= 1000;
#endif

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
	#ifndef _WIN32
		rd_len = read(fd, pbuf, len);
	#else
		rd_len = recv(fd, pbuf, len, 0);
	#endif
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
	tv.tv_usec = timeout%1000;
#ifndef _WIN32
	tv.tv_usec *= 1000;
#endif

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

#ifndef _WIN32
	wr_len = write(fd, pbuf, len);
#else
	wr_len = send(fd, pbuf, len, 0);
#endif

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
	#ifndef _WIN32
		ret = read(fd, pbuf+rd_len, len-rd_len);
	#else
		ret = recv(fd, pbuf+rd_len, len-rd_len, 0);
	#endif
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
	#ifndef _WIN32
		ret = write(fd, pbuf+wr_len, len-wr_len);
	#else
		ret = send(fd, pbuf+wr_len, len-wr_len, 0);
	#endif
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

void clear_socket_buf(int fd) {
	unsigned char data[1024];
	while (read_fd_timeout(fd, data, sizeof(data), 0) > 0);
}

int socket_join_group(int fd, char *mult_ip) {
	struct ip_mreq imr;
	imr.imr_multiaddr.s_addr = ip_to_net(mult_ip);
	imr.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		(const char*)&imr, sizeof (struct ip_mreq)) < 0) {
			trace("setsockopt IP_ADD_MEMBERSHIP failed: %s\n", strerror(errno));
			return -1;
	}

	return 0;
}

int socket_leave_group(int fd, char *mult_ip) {
	struct ip_mreq imr;
	imr.imr_multiaddr.s_addr = ip_to_net(mult_ip);
	imr.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP,
		(const char*)&imr, sizeof (struct ip_mreq)) < 0) {
			trace("setsockopt IP_DROP_MEMBERSHIP failed: %s\n", strerror(errno));
			return -1;
	}

	return 0;
}

int our_ip_addr() {
	int fd;
	/*char * mult_ip = "228.67.43.91";*/
	char * mult_ip = "255.255.255.255";
	unsigned short port = 15947;
	char test_string[1024] = "hostIdTest";
	int test_string_len = strlen(test_string);
	char recv_buf[1024] = {0};
	int recv_len;
	char src_ip[16] = {0};
	int ret = 0;

	fd = create_udp_server_socket(port);
	if (fd <= 0) {
		trace("create_udp_server_socket failed\n");
		ret = -1;
		goto exit;
	}

	do {
// 		ret = socket_join_group(fd, mult_ip);
// 		if (ret < 0) {
// 			trace("socket_join_group failed\n");
// 			break;
// 		}

		srand((unsigned int)&recv_len);

		sprintf(test_string+test_string_len, "%d", rand());
		test_string_len = strlen(test_string);

		ret = udp_send(fd, test_string, test_string_len, mult_ip, port);
		if (ret < 0) {
			trace("udp_send failed\n");
			break;
		}

		while(1) {
			ret = read_fd_timeout(fd, NULL, 0, 5000);
			if (ret < 0) {
				trace("read_fd_timeout failed\n");
				break;
			}
			recv_len = udp_recv(fd, recv_buf, sizeof(recv_buf), src_ip, NULL);

			if (recv_len == test_string_len && 0 == strncmp(test_string, recv_buf, recv_len)) {
				break;
			}
		}
	} while(0);

// 	ret = socket_leave_group(fd, mult_ip);
// 	if (ret < 0) {
// 		trace("socket_leave_group failed\n");
// 		goto exit;
// 	}

exit:
	if (fd > 0) {
		close_fd(fd);
	}
	if (ret < 0) {
		ret = 0;
	} else {
		ret = ip_to_net(src_ip);
	}
	return ret;
}

#ifdef __cplusplus
}
#endif