#ifndef __RPC_H__
#define __RPC_H__

#include "rmi.h"

struct aaa {
	int a MARK(0, 0);
	signed char b MARK(1, 0);
	float c MARK(2, 0);
	double d MARK(3, 0);
	long double e MARK(4, 0);
	char f[100] MARK(5, 0);
};
struct bbb {
	struct aaa stAaa MARK(0, 0);
	int a[10] MARK(1, 0);
	struct aaa a_array[20] MARK(2, 0);
};
int set_para(struct rmi * rmi, IN int index, IN struct aaa * para1); 
int set_para2(struct rmi * rmi, IN int index, IN struct aaa * para1, IN struct bbb * para2); 
int get_para(struct rmi * rmi, IN int index, OUT struct aaa *para1);
int get_para2(struct rmi * rmi, IN int index, OUT struct aaa *para1, OUT struct bbb *para2);

#endif

