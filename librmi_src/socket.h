#ifndef __SOCKET_H__
#define __SOCKET_H__

#ifdef __cplusplus
extern "C" {
#endif

#define FD_SUCCESS	(0)
#define FD_ERROR	(-1)
#define FD_TIMEOUT	(-2)
#define FD_CLOSED   (-3)

#define FD_RD	1
#define FD_WR	2
#define FD_RDWR	3

// tool fuction
char * get_fd_error_str(int err_no);
int ip_to_net(char * ip);
char * net_to_ip(int addr);

// create socket
int close_fd(int fd);
#ifdef _WIN32
int socket_init();
#endif

#ifndef _WIN32
int create_epoll(int maxfd);
int epoll_envent_add(int epollfd, int fd, int state, int mode, void * ptr);
int epoll_envent_delete(int epollfd, int fd);
int epoll_envent_modify(int epollfd, int fd, int state, int mode, void * ptr);
int epoll_wait_data(int epollfd, int * fd_array, const int len, int timeout);
int epoll_read_data(int fd, unsigned char * buf, int buf_size);
#endif

int create_tcp_server_socket(unsigned short port);
int create_tcp_client_socket(char * dst_ip, unsigned short port);
int tcp_accept(int fd);
int create_udp_server_socket(unsigned short port);
int create_udp_client_socket(char * dst_ip, unsigned short port);

// set fd option
int set_fd_reuse(int fd);
int set_fd_nonblock(int fd);
int set_fd_recvbuf(int fd, int recv_buf);
int set_fd_sendbuf(int fd, int send_buf);
int set_fd_broadcast(int fd);

// fd read write fuction
int read_fd_timeout(int fd, unsigned char * pbuf, int len, int timeout/*ms*/);
int write_fd_timeout(int fd, unsigned char * pbuf, int len, int timeout/*ms*/);
int udp_send(int fd, unsigned char * pbuf, int len, char * dst_ip, unsigned short port);
int udp_recv(int fd, unsigned char * pbuf, int len, char * src_ip, unsigned short * port);
int nonblock_read(int fd, unsigned char * pbuf, int len, int timeout/*ms*/);
int nonblock_write(int fd, unsigned char * pbuf, int len, int timeout/*ms*/);
int block_read(int fd, unsigned char * pbuf, int len);
int block_write(int fd, unsigned char * pbuf, int len);

int get_host_info(int fd, char * src_ip, unsigned short * src_port);
int get_peer_info(int fd, char * src_ip, unsigned short * src_port);
int udp_set_dst(int fd, char * dst_ip, unsigned short port);


#ifdef __cplusplus
}
#endif

#endif

