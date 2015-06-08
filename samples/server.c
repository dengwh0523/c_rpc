#include <signal.h>
#include <pthread.h>

#include "test.h"
#include "rmi.h"

#define NET_TIMEOUT	100*1000	// unit: 1us

#define MAX_NUM 2

/*struct aaa gs_para1[MAX_NUM];*/
/*struct bbb gs_para2[MAX_NUM];*/

pthread_mutex_t g_lock[MAX_NUM];

#if 0
int set_para(struct rmi * rmi, _IN int index, _IN struct aaa * para1) {
	if (index >= MAX_NUM) {
		trace("para error\n");
		return 1;
	}
	pthread_mutex_lock(&g_lock[index]);
	memcpy(&gs_para1[index], para1, sizeof(struct aaa));
	pthread_mutex_unlock(&g_lock[index]);
	return 0;
}

int set_para2(struct rmi * rmi, _IN int index, _IN struct aaa * para1, _IN struct bbb * para2) {
	if (index >= MAX_NUM) {
		trace("para error\n");
		return 1;
	}
	pthread_mutex_lock(&g_lock[index]);
	memcpy(&gs_para1[index], para1, sizeof(struct aaa));
	memcpy(&gs_para2[index], para2, sizeof(struct bbb));
	pthread_mutex_unlock(&g_lock[index]);
	return 0;
}

int get_para(struct rmi * rmi, _IN int index, _OUT struct aaa *para1) {
	if (index >= MAX_NUM) {
		trace("para error\n");
		return 1;
	}
	pthread_mutex_lock(&g_lock[index]);
	memcpy(para1, &gs_para1[index], sizeof(struct aaa));
	pthread_mutex_unlock(&g_lock[index]);
	return 0;
}

int get_para2(struct rmi * rmi, _OUT struct aaa *para1, _OUT struct bbb *para2, _IN int index) {
	if (index >= MAX_NUM) {
		trace("para error\n");
		return 1;
	}
	pthread_mutex_lock(&g_lock[index]);
	memcpy(para1, &gs_para1[index], sizeof(struct aaa));
	memcpy(para2, &gs_para2[index], sizeof(struct bbb));
	pthread_mutex_unlock(&g_lock[index]);
	return 0;
}

int get_version(struct rmi * rmi) {
	return 0;
}

int test1(struct rmi * rmi, int para0, char * ddd) {
	return 0;
}

enable: 1
link: 00000001
speed: 00010001
duplex: 00010003
fc: 00000003
#endif
/*int switch_get_port_status(struct rmi * rmi, unsigned int port, SWITCH_PORT_INFO_S * pstPortInfo) {*/
/*	pstPortInfo->enable = 0x1;*/
/*	pstPortInfo->link = 0x1;*/
/*	pstPortInfo->speed = 0x10001;*/
/*	pstPortInfo->duplex = 0x10003;*/
/*	pstPortInfo->fc = 0x3;*/
/**/
/*	return 0;*/
/*}*/

void set_test_data(TEST_S * pt) {
	int i;
	pt->aaa = 0xaa55;
	for (i = 0; i < TEST_MACRO; i++) {
		pt->bbb[i] = i;
	}
}

int switch_test_get2(struct rmi * rmi, TEST3_S * ptest) {
	TEST_S * pt;
	int i;
	pt = &ptest->stTest2.stTest2;	
	set_test_data(pt);

	for (i = 0; i < TEST_MACRO; i++) {
		pt = &ptest->stTest2.stTest1[i];	
		set_test_data(pt);
	}

/*	int i;*/
/*	char *buf = (char *)ptest;*/
/*	for (i = 0; i < sizeof(TEST3_S); i++) {*/
/*		*(buf+i) = i;*/
/*	}*/

	return 0;
}

int main(int argc, char * argv[]) {
	unsigned short port;
	int fd = -1;
	int i;
	struct sigaction act;

	struct rmi server_rmi;
	
	if (argc != 2) {
		printf("usage: %s port\n", argv[0]);
		return -1;
	}

	port = atoi(argv[1]);

	for (i = 0; i < MAX_NUM; i++) {
		pthread_mutex_init(&g_lock[i], NULL);
	}
	
	act.sa_sigaction = SIG_IGN;
    act.sa_flags = SA_NOMASK;
    sigaction(SIGPIPE, &act, NULL);

	RMI_INIT_SERVER(&server_rmi, test);
	rmi_set_socket_type(&server_rmi, RMI_SOCKET_UDP);
	rmi_set_recv_buf_size(&server_rmi, 2*1024*1024);
	rmi_set_send_buf_size(&server_rmi, 2*1024*1024);
	rmi_server_start(&server_rmi, port);

	getchar();

	rmi_server_close(&server_rmi);

	return 0;	
}

