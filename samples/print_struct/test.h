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
	unsigned long long g MARK(6, 0);
};
struct bbb {
	struct aaa stAaa MARK(0, 0);
	unsigned int a[10] MARK(1, 0);
	struct aaa a_array[3] MARK(2, 0);
};

#endif

