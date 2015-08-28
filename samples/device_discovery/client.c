#ifndef _WIN32
#include <signal.h>
#else
#include <windows.h>
#endif

#include <stdlib.h>

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

void set_dev_info(struct rmi * rmi, DEV_INFO_S * pdev) {
	printf("dev_ip: 0x%08x %s\n", pdev->dev_ip, net_to_ip(my_ntohl(pdev->dev_ip)));

	return;
}

void * test_proc() {
	struct rmi client_rmi, * rmi;
	int i;
	int test_times = 1;
	int n;
	
	rmi = &client_rmi;
	RMI_INIT_CLIENT(rmi, test);	
	rmi_set_broadcast(rmi);
	if (0 != rmi_client_start(rmi, "255.255.255.255", server_port)) {
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

	struct rmi find_rmi;

#ifndef _WIN32
	pthread_t pid;
	struct sigaction act;

	act.sa_sigaction = SIG_IGN;
	act.sa_flags = SA_NOMASK;
	sigaction(SIGPIPE, &act, NULL);
#else
	if (0 != socket_init()) {
		trace("socket init failed\n");
		return -1;
	}
#endif
	
	if (argc != 2) {
		printf("usage: %s port\n", argv[0]);
		return -1;
	}

	/*server_ip = argv[1];*/
	server_port = atoi(argv[1]);

	RMI_INIT_SERVER(&find_rmi, find);
	rmi_set_broadcast(&find_rmi);
/*	rmi_set_keepalive_time(&find_rmi, 0xffffff);*/
	rmi_set_recv_buf_size(&find_rmi, 1024*1024);
	rmi_server_start(&find_rmi, 8888);

	for(i = 0; i < CONNECT_NUM; i++) {
		/*int ret = pthread_create(&pid, NULL, test_proc, NULL);*/

		int j;
		for (j = 0; j < 3; j++) {
			if (thread_run(test_proc, NULL)) {
				break;
			} else {
				msleep(10);
			}
		}
		if (3 == j) {
			trace("start test_proc thread failed\n");
		}
	}

	getchar();
	printf("successs cnt: %d\n", test_cnt);
	
	rmi_server_close(&find_rmi);

	return 0;
}

