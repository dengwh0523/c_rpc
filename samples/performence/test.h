#ifndef __RPC_H__
#define __RPC_H__

#include "rmi.h"

#define TEST_MACRO	100

struct aaa {
	int a MARK(0, 0);
	char b MARK(1, 0);
	float c MARK(2, 0);
	double d MARK(3, 0);
	long double e MARK(4, 0);
	char f[TEST_MACRO] MARK(5, 1);
};
struct bbb {
	struct aaa stAaa MARK(0, 0);
	unsigned int a[10] MARK(1, 0);
	struct aaa a_array[20] MARK(2, 0);
};
int set_para(struct rmi * rmi, _IN int index, _IN struct aaa * para1); 
int set_para2(struct rmi * rmi, _IN int index, _IN struct aaa * para1, _IN struct bbb * para2); 
int get_para(struct rmi * rmi, _IN int index, _OUT struct aaa *para1);
int get_para2(struct rmi * rmi, _IN int index, _OUT struct aaa *para1, _OUT struct bbb * para2);

int get_version(struct rmi * rmi);
int test1(struct rmi * rmi, _IN int, _OUT char * ddd);

#endif

