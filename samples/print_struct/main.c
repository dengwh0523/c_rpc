#include "test.h"
#include "rmi.h"
#include "test_rmi.h"

int main(int argc, char * argv[]) {
	struct rmi server_rmi;
	
	struct aaa staaa;
	struct bbb stbbb;
	int index = 0;

	RMI_INIT_SERVER(&server_rmi, test);
	// rmi_server_start(&server_rmi, 1000);
	
#if 0
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
#endif

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
	stbbb.a_array[0] = staaa;
	
	rmi_print_struct_info(&server_rmi, "struct aaa", "staaa", (unsigned char *)&staaa, &index);
	
	index = 0;
	rmi_print_struct_info(&server_rmi, "struct bbb", "stbbb", (unsigned char *)&stbbb, &index);

	getchar();

	rmi_server_close(&server_rmi);

	return 0;	
}