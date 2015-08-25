#ifndef _WIN32
#include <signal.h>
#include <pthread.h>
#else
#include <Winsock2.h>
#endif

#include <stdlib.h>

#include "test.h"
#include "test_rmi.h"
#include "find_device.h"
#include "find_rmi.h"
#include "socket.h"

#define NET_TIMEOUT	100*1000	// unit: 1us

void find_device(struct rmi * rmi) {
	DEV_INFO_S stDevinfo;
	struct rmi find_rmi;

	/*stDevinfo.dev_ip = 0x8501a8c0;*/
	stDevinfo.dev_ip = our_ip_addr();

	RMI_INIT_CLIENT(&find_rmi, find);
	rmi_set_broadcast(&find_rmi);
	rmi_client_start(&find_rmi, "255.255.255.255", 8888);
	set_dev_info(&find_rmi, &stDevinfo);
	rmi_client_close(&find_rmi);

	return;
}

int main(int argc, char * argv[]) {
	unsigned short port;

	struct rmi server_rmi;

#ifndef _WIN32
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

	port = atoi(argv[1]);

	RMI_INIT_SERVER(&server_rmi, test);
	rmi_set_socket_type(&server_rmi, RMI_SOCKET_UDP);
	rmi_set_broadcast(&server_rmi);
/*	rmi_set_keepalive_time(&server_rmi, 0xffffff);*/
	rmi_server_start(&server_rmi, port);

	getchar();

	rmi_server_close(&server_rmi);

	return 0;	
}

