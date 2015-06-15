#ifndef __FIND_DEVICE_H__
#define __FIND_DEVICE_H__

#include "rmi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	char dev_type[32] MARK(0, 1);
	unsigned short port MARK(1, 0);
	int dev_ip MARK(2, 0);
	int dev_mask MARK(3, 0);
	int dev_gw MARK(4, 0);
	char dev_mac[6] MARK(5, 0);
} DEV_INFO_S;

int set_dev_info(struct rmi * rmi, _IN DEV_INFO_S * pdev);

#ifdef __cplusplus
}
#endif

#endif

