#include "test.h"
#include "rmi.h"
#include "test_rmi.h"

int main(int argc, char * argv[]) {
	struct rmi server_rmi;
	
	struct aaa staaa;
	struct bbb stbbb;

	RMI_INIT_SERVER(&server_rmi, test);

	memset(&staaa, 0, sizeof(staaa));
	staaa.a = -10;
	staaa.b = 100;
	staaa.c = 0.14;
	staaa.d = 3.1415926;
	staaa.e = 345678.3456789;
	staaa.g = 0x1234567890;
	strcpy(staaa.f, "test print");
	
	
	memset(&stbbb, 0, sizeof(stbbb));
	stbbb.stAaa = staaa;
	stbbb.a[0] = 0xffffffff;
	stbbb.a_array[1] = staaa;
	
	RMI_PRINT_STRUCT_INFO(&server_rmi, struct aaa, staaa);
	RMI_PRINT_STRUCT_INFO(&server_rmi, struct bbb, stbbb);

	getchar();

	rmi_finit(&server_rmi);

	return 0;	
}