#include <signal.h>
#include <pthread.h>

#include "test.h"
#include "test_rmi.h"
#include "find_device.h"

#define NET_TIMEOUT	100*1000	// unit: 1us

int find_device(struct rmi * rmi) {
	DEV_INFO_S stDevinfo;
	struct rmi find_rmi;

	stDevinfo.dev_ip = 0x8501a8c0;

	RMI_INIT_CLIENT(&find_rmi, find);
	rmi_set_socket_type(&find_rmi, RMI_SOCKET_UDP);
	rmi_set_broadcast(&find_rmi);
	rmi_client_start(&find_rmi, "255.255.255.255", 8888);
	set_dev_info(&find_rmi, &stDevinfo);
	rmi_client_close(&find_rmi);

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
	
	act.sa_sigaction = SIG_IGN;
    act.sa_flags = SA_NOMASK;
    sigaction(SIGPIPE, &act, NULL);

	RMI_INIT_SERVER(&server_rmi, test);
	rmi_set_socket_type(&server_rmi, RMI_SOCKET_UDP);
	rmi_set_broadcast(&server_rmi);
/*	rmi_set_keepalive_time(&server_rmi, 0xffffff);*/
	rmi_server_start(&server_rmi, port);

	getchar();

	rmi_server_close(&server_rmi);

	return 0;	
}

