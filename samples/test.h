#ifndef __SWITCH_CTRL_H__
#define __SWITCH_CTRL_H__

#include "rmi.h"

#define TEST_MACRO	30

#ifdef __cplusplus
extern "C" {
#endif

/*typedef struct {*/
/*	int enable MARK(0, 0);*/
/*	int link MARK(1, 0);*/
/*	int speed MARK(2, 0);*/
/*	int duplex MARK(3, 0);*/
/*	int fc MARK(4, 0);*/
/*} SWITCH_PORT_INFO_S;*/
/**/
/*int switch_get_port_status(struct rmi * rmi, _IN unsigned int port, _OUT SWITCH_PORT_INFO_S * pstPortInfo);*/


struct test {
	int aaa MARK(0, 0);
	int bbb[TEST_MACRO] MARK(1, 0);
};

typedef struct test TEST_S;

typedef struct {
	TEST_S stTest1[TEST_MACRO] MARK(0, 0);
	struct test stTest2 MARK(1, 0);
} TEST2_S;

typedef struct test3 {
	TEST2_S stTest2 MARK(0, 0);
	TEST2_S stTest3[TEST_MACRO] MARK(1, 0);
} TEST3_S;

int switch_test_get2(struct rmi * rmi, _OUT TEST3_S * ptest);


#ifdef __cplusplus
}
#endif

#endif

