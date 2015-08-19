#ifndef _WIN32
#include <signal.h>
#else
#include <Winsock2.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include "test.h"
#include "rmi.h"
#include "test_rmi.h"
#include "socket.h"

#define NET_TIMEOUT	100*1000	// unit: 1us

#define MAX_NUM 2

struct aaa gs_para1[MAX_NUM];
struct bbb gs_para2[MAX_NUM];

lock  * g_lock[MAX_NUM];

int set_para(struct rmi * rmi, _IN int index, _IN struct aaa * para1) {
	if (index >= MAX_NUM) {
		trace("para error\n");
		return 1;
	}
	lock_lock(g_lock[index]);
	memcpy(&gs_para1[index], para1, sizeof(struct aaa));
	lock_unlock(g_lock[index]);
	return 0;
}

int set_para2(struct rmi * rmi, _IN int index, _IN struct aaa * para1, _IN struct bbb * para2) {
	if (index >= MAX_NUM) {
		trace("para error\n");
		return 1;
	}
	lock_lock(g_lock[index]);
	memcpy(&gs_para1[index], para1, sizeof(struct aaa));
	memcpy(&gs_para2[index], para2, sizeof(struct bbb));
	lock_unlock(g_lock[index]);
	return 0;
}

int get_para(struct rmi * rmi, _IN int index, _OUT struct aaa *para1) {
	if (index >= MAX_NUM) {
		trace("para error\n");
		return 1;
	}
	lock_lock(g_lock[index]);
	memcpy(para1, &gs_para1[index], sizeof(struct aaa));
	lock_unlock(g_lock[index]);
	return 0;
}

int get_para2(struct rmi * rmi, _IN int index, _OUT struct aaa *para1, _OUT struct bbb *para2) {
	if (index >= MAX_NUM) {
		trace("para error\n");
		return 1;
	}
	lock_lock(g_lock[index]);
	memcpy(para1, &gs_para1[index], sizeof(struct aaa));
	memcpy(para2, &gs_para2[index], sizeof(struct bbb));
	lock_unlock(g_lock[index]);
	return 0;
}

int get_version(struct rmi * rmi) {
	return 0;
}

int test1(struct rmi * rmi, int para0, char * ddd) {
	return 0;
}

int main(int argc, char * argv[]) {
	unsigned short port;
	int fd = -1;
	int i;
#ifndef _WIN32
	struct sigaction act;

	act.sa_sigaction = SIG_IGN;
	act.sa_flags = SA_NOMASK;
	sigaction(SIGPIPE, &act, NULL);
#endif

	struct rmi server_rmi;

#ifdef _WIN32
	if (0 != socket_init()) {
		trace("socket init failed\n");
		return -1;
	}
#endif
	
	if (argc != 2) {
		printf("usage: %s port\n", argv[0]);
		return -1;
	}

	port = atoi(argv[1]);

	for (i = 0; i < MAX_NUM; i++) {
		g_lock[i] = lock_create();
	}

	RMI_INIT_SERVER(&server_rmi, test);
	rmi_server_start(&server_rmi, port);

	getchar();

	rmi_server_close(&server_rmi);
	for (i = 0; i < MAX_NUM; i++) {
		lock_destroy(g_lock[i]);
	}

	return 0;	
}

