#include <signal.h>

#include "test.h"
#include "test_rmi.h"
#include "find_device.h"
#include "find_rmi.h"
#include "socket.h"

#define CONNECT_NUM	1

char * server_ip;
unsigned short server_port;

int test_cnt = 0;
int connect_cnt = 0;

int set_dev_info(struct rmi * rmi, DEV_INFO_S * pdev) {
	printf("dev_ip: %s\n", net_to_ip(pdev->dev_ip));

	return 0;
}

void * test_proc() {
	struct rmi client_rmi, * rmi;
	int i;
	int test_times = 1;
	int n;
	
	rmi = &client_rmi;
	RMI_INIT_CLIENT(rmi, test);	
	rmi_set_socket_type(rmi, RMI_SOCKET_UDP);
	rmi_set_broadcast(rmi);
	rmi_set_timeout(rmi, 500);
	if (0 != rmi_client_start(rmi, server_ip, server_port)) {
		trace("rmi_client_start failed\n");
		return NULL;
	}
	
	find_device(rmi);

	if (0 != rmi_client_close(rmi)) {
		trace("rmi_client_close failed\n");
		return NULL;
	}
	printf("test pass!!!\n");
	test_cnt++;

	return NULL;
}

int main(int argc, char * argv[]) {
	unsigned short port;
	char * host;
	struct rmi * rmi;
	int i;
	int cnt;
	pthread_t pid;
	struct sigaction act;

	struct rmi find_rmi;
	
	if (argc != 3) {
		printf("usage: %s host port\n", argv[0]);
		return -1;
	}

	server_ip = argv[1];
	server_port = atoi(argv[2]);
	
	act.sa_sigaction = SIG_IGN;
    act.sa_flags = SA_NOMASK;
    sigaction(SIGPIPE, &act, NULL);

	RMI_INIT_SERVER(&find_rmi, find);
	rmi_set_socket_type(&find_rmi, RMI_SOCKET_UDP);
	rmi_set_broadcast(&find_rmi);
/*	rmi_set_keepalive_time(&find_rmi, 0xffffff);*/
	rmi_server_start(&find_rmi, 8888);

	for(i = 0; i < CONNECT_NUM; i++) {
		int ret = pthread_create(&pid, NULL, test_proc, NULL);
	}

	getchar();
	printf("successs cnt: %d\n", test_cnt);
	
	rmi_server_close(&find_rmi);

	return 0;
}

