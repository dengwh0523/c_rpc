#ifndef __SWITCH_CTRL_H__
#define __SWITCH_CTRL_H__

#include "rmi.h"

#define TEST_MACRO	10

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int enable MARK(0, 0);
	int link MARK(1, 0);
	int speed MARK(2, 0);
	int duplex MARK(3, 0);
	int fc MARK(4, 0);
} SWITCH_PORT_INFO_S;

int switch_get_port_status(struct rmi * rmi, _IN unsigned int port, _OUT SWITCH_PORT_INFO_S * pstPortInfo);

#ifdef __cplusplus
}
#endif

#endif

