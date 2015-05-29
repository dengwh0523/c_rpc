#include <signal.h>
#include <pthread.h>

#include "test.h"
#include "rmi.h"

#define NET_TIMEOUT	100*1000	// unit: 1us

#define MAX_NUM 2

struct aaa gs_para1[MAX_NUM];
struct bbb gs_para2[MAX_NUM];

pthread_mutex_t g_lock[MAX_NUM];

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

	rmi_init(&server_rmi);
	rmi_server_start(&server_rmi, port);

	getchar();

	rmi_server_close(&server_rmi);

	return 0;	
}

