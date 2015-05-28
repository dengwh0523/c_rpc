#include "rmiparse.h"
#include "debug.h"
#include "crc32.h"

#define RMI_FIELD_VAR			0
#define RMI_FIELD_FIX			1
#define RMI_FIELD_LEN			2
#define RMI_FIELD_SIGNED		3

#define DEFAULT_STUB_H	"stub.h"
#define DEFAULT_STUB_C	"stub.c"
#define DEFAULT_PROXIER_C	"proxier.c"

#define MAKE_FIELD_TYPE(field_type) {field_type, #field_type}

LIST_S g_struct_list;
LIST_S g_func_list;
extern int yyparse(); // 实现在x.tab.c 文件中
extern void init(void);
extern int set_input_file(char *infile);

char * pointer_str[] = {"", " *"};
char * pointer2_str[] = {"", "&"};

// file
char * infile = NULL;
char * stub_h = DEFAULT_STUB_H;
char * stub_c = DEFAULT_STUB_C;
char * proxier_c = DEFAULT_PROXIER_C;

FILE * stub_h_fp = NULL;
FILE * stub_c_fp = NULL;
FILE * proxier_c_fp = NULL;

struct field_type_s {
	int id;
	char * name;
};

struct field_type_s filed_type_map[] = {
	MAKE_FIELD_TYPE(RMI_FIELD_VAR),
	MAKE_FIELD_TYPE(RMI_FIELD_FIX),
	MAKE_FIELD_TYPE(RMI_FIELD_LEN),
	MAKE_FIELD_TYPE(RMI_FIELD_SIGNED),
};

char * get_field_type(const int id) {
	int n = STR_ARRAY_LEN(filed_type_map);
	int i;
	for (i = 0; i < n; i++) {
		if (id == filed_type_map[i].id) {
			return filed_type_map[i].name;
		}
	}

	return NULL;
}

int rpc_init() {	
	memset(&g_struct_list, 0, sizeof(g_struct_list));
	memset(&g_func_list, 0, sizeof(g_func_list));
	
	if (0 != list_init(&g_struct_list, BUF_SIZE, LIST_MAX_TIME, LIST_MAX_PACKET, LIST_MAX_USER)) {
		trace("list_init failed\n");
		return -1;
	}
	
	if (0 != list_init(&g_func_list, BUF_SIZE, LIST_MAX_TIME, LIST_MAX_PACKET, LIST_MAX_USER)) {
		trace("list_init failed\n");
		return -1;
	}
	
	init();

/*	stub_h_fp = fopen(stub_h, "wb");*/
/*	if (NULL == stub_h_fp) {*/
/*		trace("open file[%s] failed\n", stub_h);*/
/*		return -1;*/
/*	}*/
	stub_c_fp = fopen(stub_c, "wb");
	if (NULL == stub_c_fp) {
		trace("open file[%s] failed\n", stub_c);
		return -1;
	}
	proxier_c_fp = fopen(proxier_c, "wb");
	if (NULL == proxier_c_fp) {
		trace("open file[%s] failed\n", proxier_c);
		return -1;
	}

	return 0;
}

int rpc_display() {
	int struct_num;
	int func_num;
	int i, j;
	
	struct_num = list_size(&g_struct_list);
	printf("!!!!!!!!!!!! total struct num is %d !!!!!!!!!!\n", struct_num);

	for (i = 0; i < struct_num; i++) {
		struct struct_info * struct_info;
		int para_num;		
		struct_info = list_at(&g_struct_list, i);
		para_num = list_size(&struct_info->para_list);
		printf("%s name: %s, member num: %d\n", struct_info->type, struct_info->name, para_num);
		for (j = 0; j < para_num; j++) {
			struct parameter * para;
			para = list_at(&struct_info->para_list, j);
			printf("\t%s%s:%s:%d\n", para->type, pointer_str[para->pointer], para->name, para->len);
		}
	}

	func_num = list_size(&g_func_list);
	printf("!!!!!!!!!!!! total function num is %d !!!!!!!!!!\n", func_num);

	for (i = 0; i < func_num; i++) {
		struct func_info * func_info;
		int para_num;		
		func_info = list_at(&g_func_list, i);
		para_num = list_size(&func_info->para_list);
		printf("return type: %s%s; func name: %s; para num: %d\n", \
			func_info->ret_type, pointer_str[func_info->pointer], func_info->func_name, para_num);
		for (j = 0; j < para_num; j++) {
			struct parameter * para;
			para = list_at(&func_info->para_list, j);
			printf("\t%s%s:%s:%d\n", para->type, pointer_str[para->pointer], para->name, para->len);
		}
	}

	return 0;
}

int rpc_finit() {
	int struct_num;
	int func_num;
	int i, j;
	
	struct_num = list_size(&g_struct_list);

	for (i = 0; i < struct_num; i++) {
		struct struct_info * struct_info;
		struct_info = list_at(&g_struct_list, i);
		list_finit(&struct_info->para_list);
	}
	list_finit(&g_struct_list);

	func_num = list_size(&g_func_list);

	for (i = 0; i < func_num; i++) {
		struct func_info * func_info;
		func_info = list_at(&g_func_list, i);
		list_finit(&func_info->para_list);
	}
	list_finit(&g_func_list);

	return 0;
}

int output(FILE * fp, char * str) {
	fprintf(fp, "%s", str);
	return 0;
}

int output_struct_info(FILE * fp, LIST_S * struct_list) {
	int struct_num;
	int i, j;

	struct_num = list_size(struct_list);
	for (i = 0; i < struct_num; i++) {
		struct struct_info * struct_info;
		int para_num;		
		struct_info = list_at(struct_list, i);
		output(fp, struct_info->type);
		output(fp, " ");
/*		output(fp, struct_info->name);*/
/*		output(fp, " ");*/
		output(fp, "{\n");
		
		para_num = list_size(&struct_info->para_list);
		for (j = 0; j < para_num; j++) {
			struct parameter * para;
			para = list_at(&struct_info->para_list, j);
			output(fp, "\t");
			output(fp, para->type);
			output(fp, pointer_str[para->pointer]);
			output(fp, " ");
			output(fp, para->name);
			if (para->len > 1) {
				char buf[16] = {0};
				output(fp, "[");
				sprintf(buf, "%d", para->len);
				output(fp, buf);
				output(fp, "]");
			}
			output(fp, ";\n");
		}
		output(fp, "};\n\n");
	}

	return 0;
}

int output_func_info(FILE * fp, LIST_S * func_list) {
	int func_num;
	int i, j;

	func_num = list_size(func_list);
	for (i = 0; i < func_num; i++) {
		struct func_info * func_info;
		int para_num;		
		func_info = list_at(func_list, i);
		output(fp, func_info->ret_type);
		output(fp, pointer_str[func_info->pointer]);
		output(fp, " ");
		output(fp, func_info->func_name);
		output(fp, "(");
		
		para_num = list_size(&func_info->para_list);
		// added para
		output(fp, "struct rmi * rmi");
		if (para_num > 0) {
			output(fp, ", ");
		}
		for (j = 0; j < para_num; j++) {
			struct parameter * para;
			para = list_at(&func_info->para_list, j);
			output(fp, para->type);
			output(fp, pointer_str[para->pointer]);
			if (strlen(para->name)) {
				output(fp, " ");
				output(fp, para->name);
			}
			if (para->len > 1) {
				char buf[16] = {0};
				output(fp, "[");
				sprintf(buf, "%d", para->len);
				output(fp, buf);
				output(fp, "]");
			}
			if (j != para_num-1) {
				output(fp, ", ");
			}
		}
		output(fp, ");\n");
	}

	return 0;
}

int output_header(FILE * fp, int flag) {
	output(fp, "#include \"");
	output(fp, infile);
	output(fp, "\"\n");
	output(fp, "#include \"rmi.h\"\n");
	if (flag) {
		output(fp, "#include \"mem_pool.h\"\n");
	}
	output(fp, "\n");

	return 0;
}

int output_cplus_start(FILE * fp) {
	output(fp, "#ifdef __cplusplus\n"
			   "extern \"C\" {\n"
			   "#endif\n\n");
}

int output_cplus_end(FILE * fp) {
	output(fp, "#ifdef __cplusplus\n"
			   "}\n"
			   "#endif\n\n");
}

int output_struct_member(FILE * fp, LIST_S * struct_list) {
	int struct_num;
	int i, j;

	output(fp, "// struct member\n");

	struct_num = list_size(struct_list);
	for (i = 0; i < struct_num; i++) {
		struct struct_info * struct_info;
		int para_num;		
		struct_info = list_at(struct_list, i);
		output(fp, "struct struct_entry struct_");
		output(fp, struct_info->name);
		output(fp, "[] = {\n");
		
		para_num = list_size(&struct_info->para_list);
		for (j = 0; j < para_num; j++) {
			struct parameter * para;
			char buf[16] = {0};
			int array_len;
			int is_struct = 0;
			int is_str = 0;
			para = list_at(&struct_info->para_list, j);
			output(fp, "\t");
			output(fp, "{");
			// field num
			sprintf(buf, "%d", para->field_num);
			output(fp, buf);
			output(fp, ", ");
			// field type
			output(fp, get_field_type(para->field_type));
			output(fp, ", ");
			// type name 
			if (strstr(para->type, "struct")) {
				output(fp, "\"");
				output(fp, para->type);
				output(fp, "\"");
				is_struct = 1;
			} else {
				output(fp, "NULL");
			}
			output(fp, ", ");
			// array num
			if (!is_struct && RMI_FIELD_LEN == para->field_type) {
				array_len = 1;
				is_str = 1;
			} else {
				array_len = para->len;
			}
/*			printf("array_len: %d, name: %s, is_str: %d\n", array_len, para->name, is_str);*/
			if (1 == array_len) {
				output(fp, "1, ");
			} else {
				output(fp, "SIZE(");
				output(fp, struct_info->type);
				output(fp, ", ");
				output(fp, para->name);
				output(fp, ", ");
				output(fp, "sizeof(");
				output(fp, para->type);
				output(fp, ")");
				output(fp, "), ");
			}
			// size
			if (is_str) {
				output(fp, "SIZE(");
				output(fp, struct_info->type);
				output(fp, ", ");
				output(fp, para->name);
				output(fp, ", 1), ");
			} else {
				output(fp, "sizeof(");
				output(fp, para->type);
				output(fp, "), ");
			}
			// offset
			output(fp, "OFFSET(");
			output(fp, struct_info->type);
			output(fp, ", ");
/*			output(fp, struct_info->name);*/
/*			output(fp, ", ");*/
			output(fp, para->name);
			output(fp, ")"),
			output(fp, "},\n");
		}
		output(fp, "};\n\n");
	}

	return 0;
}

int output_struct_pair(FILE * fp, LIST_S * struct_list) {
	int struct_num;
	int i, j;

	output(fp, "// struct pair\n");
	output(fp, "struct struct_pair_entry struct_pair[] = {\n");

	struct_num = list_size(struct_list);
	for (i = 0; i < struct_num; i++) {
		struct struct_info * struct_info;
		struct_info = list_at(struct_list, i);		
		output(fp, "\t{");
		output(fp, "\"");
		output(fp, struct_info->type);
		output(fp, "\"");
		output(fp, ", ");
		output(fp, "STR_ARRAY_LEN(struct_");
		output(fp, struct_info->name);
		output(fp, "), ");
		output(fp, "struct_");		
		output(fp, struct_info->name);
		output(fp, "},\n");
	}
	output(fp, "\t{NULL, 0, NULL},\n");
	output(fp, "};\n\n");

	return 0;
}

int output_func_para(FILE * fp, LIST_S * func_list) {
	int func_num;
	int i, j;

	output(fp, "// function interface parametes\n");

	func_num = list_size(func_list);
	for (i = 0; i < func_num; i++) {
		struct func_info * func_info;
		int para_num;		
		func_info = list_at(func_list, i);
		output(fp, "struct struct_entry ");
		output(fp, func_info->func_name);
		output(fp, "_para[] = {\n");
		
		para_num = list_size(&func_info->para_list);
		for (j = 0; j < para_num; j++) {
			struct parameter * para;
			para = list_at(&func_info->para_list, j);
			output(fp, "\t{0, ");
			output(fp, get_field_type(para->field_type));
			output(fp, ", ");
			if (strstr(para->type, "struct")) {
				output(fp, "\"");
				output(fp, para->type);
				output(fp, "\"");
			} else {
				output(fp, "NULL");
			}
			output(fp, ", ");
			output(fp, "1, ");
			output(fp, "sizeof(");
			output(fp, para->type);
			output(fp, "), ");
			output(fp, "0},\n");
		}
		output(fp, "};\n\n");
	}

	return 0;
}

int output_return_para(FILE * fp) {	
	output(fp, "// funtion return parameter\n");
	output(fp, "struct struct_entry return_para[] = {\n");
	output(fp, "	{0, RMI_FIELD_SIGNED, NULL, 1, sizeof(int), 0},\n");
	output(fp, "};\n\n");

	return 0;
}

int output_func_interface(FILE * fp, LIST_S * func_list) {
	int func_num;
	int i, j;

	output(fp, "// invoke function interfaces\n");

	func_num = list_size(func_list);
	for (i = 0; i < func_num; i++) {
		struct func_info * func_info;
		char buf[16] = {0};
		func_info = list_at(func_list, i);
/*		output(fp, func_info->ret_type);*/
/*		output(fp, pointer_str[func_info->pointer]);*/
		output(fp, "int invoke_func_");
		sprintf(buf, "%08x", func_info->func_id);
		output(fp, buf);
		output(fp, "(struct rmi* rmi, struct func_entry * func, const unsigned char * pbuf, const int len, unsigned char ** ret_buf, int * ret_len);\n");
	}

	output(fp, "\n");

	return 0;
}

int output_func_map(FILE * fp, LIST_S * func_list, int flag) {
	int func_num;
	int i, j;

	output(fp, "// function map\n");
	output(fp, "struct func_entry func_table[] = {\n");
	
	func_num = list_size(func_list);
	for (i = 0; i < func_num; i++) {
		struct func_info * func_info;
		char buf[16] = {0};
		func_info = list_at(func_list, i);
		output(fp, "\t{0x");
		sprintf(buf, "%08x", func_info->func_id);
		output(fp, buf);
		output(fp, ", ");
		if (flag) {
			output(fp, "invoke_func_");
			output(fp, buf);
		} else {
			output(fp, "NULL");
		}
		output(fp, ", ");
		output(fp, "STR_ARRAY_LEN(");
		output(fp, func_info->func_name);
		output(fp, "_para), ");
		output(fp, func_info->func_name);
		output(fp, "_para},\n");
	}

	output(fp, "};\n\n");

	output(fp, "const int func_num = STR_ARRAY_LEN(func_table);\n");
	output(fp, "const int struct_num = STR_ARRAY_LEN(struct_pair);\n\n");

	return 0;
}

int output_func_definition(FILE * fp, LIST_S * func_list) {
	int func_num;
	int i, j;

	output(fp, "// invoke function definition\n");
	
	func_num = list_size(func_list);
	for (i = 0; i < func_num; i++) {
		struct func_info * func_info;
		int para_num;
		char buf[16] = {0};
		func_info = list_at(func_list, i);
		output(fp, "int invoke_func_");
		sprintf(buf, "%08x", func_info->func_id);
		output(fp, buf);
		output(fp, "(struct rmi* rmi, struct func_entry * func, const unsigned char * pbuf, const int len, unsigned char ** ret_buf, int * ret_len) {\n");

		output(fp, "\tint i;\n");
		output(fp, "\tint parse_len = 0;\n");
		output(fp, "\tint para_num = 0;\n");
		output(fp, "\tunsigned char *buf;\n\n");
		output(fp, "\t// 参数列表\n");
		output(fp, "\tint r_ret;\n");

		para_num = list_size(&func_info->para_list);
		for (j = 0; j < para_num; j++) {
			struct parameter * para;
			para = list_at(&func_info->para_list, j);
			output(fp, "\t");
			output(fp, para->type);
			output(fp, " ");
			output(fp, "r_");
			output(fp, para->name);
			output(fp, ";\n");
		}

		output(fp, "\n\t// 反序列化入参数\n");
		for (j = 0; j < para_num; j++) {
			struct parameter * para;
			para = list_at(&func_info->para_list, j);
			if (PARA_IN != para->dir) {
				continue;
			}
			output(fp, "\tparse_len += func_deserialize((unsigned char *)&r_");
			output(fp, para->name);
			output(fp, ", pbuf+parse_len, &func->para[para_num++]);\n");
		}
				
		output(fp, "\n");
		output(fp, "\tif (len != parse_len) {\n");
		output(fp, "\t	trace(\"parse error\\n\");\n");
		output(fp, "\t	return -1;\n");
		output(fp, "\t}\n");
		output(fp, "\tparse_len = 0;\n");

		output(fp, "\n\t// 调用函数\n");
		output(fp, "\tr_ret = ");
		output(fp, func_info->func_name);
		output(fp, "(");
		output(fp, "rmi");
		for (j = 0; j < para_num; j++) {
			struct parameter * para;
			para = list_at(&func_info->para_list, j);
			output(fp, ", ");
			output(fp, pointer2_str[para->pointer]);
			output(fp,"r_");
			output(fp, para->name);
		}
		output(fp, ");\n");
		
		output(fp, "\n\t// 序列化出参数\n");
		output(fp, "\t*ret_len = sizeof(struct rmi_header)+(sizeof(r_ret)");
		for (j = 0; j < para_num; j++) {
			struct parameter * para;
			para = list_at(&func_info->para_list, j);
			if (PARA_OUT != para->dir) {
				continue;
			}
			output(fp, "+sizeof(r_");
			output(fp, para->name);
			output(fp, ")");
		}
		output(fp, ")*2;\n");
		output(fp, "\t*ret_buf = mem_palloc(rmi->mem_pool, *ret_len);\n");
		output(fp, "\tif (NULL == *ret_buf) {\n");
		output(fp, "\t	trace(\"malloc failed\\n\");\n");
		output(fp, "\t	return -1;\n");
		output(fp, "\t}\n");
		output(fp, "\tbuf = *ret_buf + sizeof(struct rmi_header);\n");
		output(fp, "\tparse_len += func_serialize((unsigned char *)&r_ret, buf+parse_len, &return_para[0]);\n");

		for (j = 0; j < para_num; j++) {
			struct parameter * para;
			para = list_at(&func_info->para_list, j);
			if (PARA_OUT != para->dir) {
				continue;
			}
			output(fp, "\tparse_len += func_serialize((unsigned char *)&r_");
			output(fp, para->name);
			output(fp, ", buf+parse_len, &func->para[para_num++]);\n");
		}
		output(fp, "\n\t*ret_len = parse_len;\n\n");
		output(fp, "\treturn 0;\n");
		
		output(fp, "}\n\n");
	}


	return 0;
}

int output_proxier_func(FILE * fp, LIST_S * func_list) {
	int func_num;
	int i, j;
	char buf[16] = {0};

	output(fp, "// function definition\n");

	func_num = list_size(func_list);
	for (i = 0; i < func_num; i++) {
		struct func_info * func_info;
		int para_num;
		func_info = list_at(func_list, i);

		// function interface
		output(fp, func_info->ret_type);
		output(fp, pointer_str[func_info->pointer]);
		output(fp, " ");
		output(fp, func_info->func_name);
		output(fp, "(struct rmi * rmi");		
		
		para_num = list_size(&func_info->para_list);
		for (j = 0; j < para_num; j++) {
			struct parameter * para;
			para = list_at(&func_info->para_list, j);
			output(fp, ", ");
			output(fp, para->type);
			output(fp, pointer_str[para->pointer]);
			output(fp, " ");
			output(fp, para->name);
		}
		output(fp, ") {\n");

		// parameter
		output(fp, "\tunsigned char buf[sizeof(struct rmi_header)+(sizeof(int)");
		for (j = 0; j < para_num; j++) {
			struct parameter * para;
			para = list_at(&func_info->para_list, j);
			if (PARA_IN != para->dir) {
				continue;
			}
			output(fp, "+sizeof(");
			output(fp, para->type);
			output(fp, ")");
		}
		output(fp, ")*2] = {0};\n");
		output(fp, "\tunsigned char * pbuf = buf+sizeof(struct rmi_header);\n");

		output(fp, "\tunsigned char * pdata;\n");
		output(fp, "\tint len = 0;\n");
		output(fp, "\tint parse_len = 0;\n");
		output(fp, "\tstruct func_entry * func_entry;\n");
		output(fp, "\tint para_num = 0;\n");

		output(fp, "\tint func_id = ");
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "0x%08x", func_info->func_id);
		output(fp, buf);
		output(fp, ";\n");

		output(fp, "\n\t// 参数列表\n");
		output(fp, "\tint r_ret;\n\n");

		output(fp, "\tfunc_entry = get_func_entry(func_id);\n");
		output(fp, "\tif (NULL == func_entry) {\n");
		output(fp, "\t	trace(\"no such function\\n\");\n");
		output(fp, "\t	return -1;\n");
		output(fp, "\t}\n");

		output(fp, "\n\t// 序列化入参数\n");
		for (j = 0; j < para_num; j++) {
			struct parameter * para;
			para = list_at(&func_info->para_list, j);
			if (PARA_IN != para->dir) {
				continue;
			}
			output(fp, "\tlen += func_serialize((unsigned char *)");
			output(fp, pointer2_str[para->pointer^0x1]);
			output(fp, para->name);
			output(fp, ", pbuf+len, &func_entry->para[para_num++]);\n");			
		}

		output(fp, "\n\t// 远程调用\n");
		output(fp, "\tif (0 != invoke(rmi, func_id, buf, len, &pdata, &len)) {\n");
		output(fp, "\t	trace(\"invoke failed\\n\");\n");
		output(fp, "\t	return -1;\n");
		output(fp, "\t}\n");
		
		output(fp, "\n\t// 反序列化出参数\n");
		output(fp, "\tparse_len += func_deserialize((unsigned char *)&r_ret, pdata+parse_len, &return_para[0]);\n");
		for (j = 0; j < para_num; j++) {
			struct parameter * para;
			para = list_at(&func_info->para_list, j);
			if (PARA_OUT != para->dir) {
				continue;
			}
			output(fp, "\tparse_len += func_deserialize((unsigned char *)");
			output(fp, pointer2_str[para->pointer^0x1]);
			output(fp, para->name);
			output(fp, ", pdata+parse_len, &func_entry->para[para_num++]);\n");
		}
/*		output(fp, "\n\tfree(pdata);\n\n");*/
		output(fp, "\n");
	
		output(fp, "\tif (len != parse_len) {\n");
		output(fp, "\t	trace(\"parse error\\n\");\n");
		output(fp, "\t	return -1;\n");
		output(fp, "\t}\n");

		output(fp, "\n\treturn r_ret;\n");
		output(fp, "}\n\n");
	}

	return 0;
}
	
int generate_stub_h(LIST_S * struct_list, LIST_S * func_list, FILE * fp) {
	output(fp, "#include \"rmi.h\"\n\n");
	output(fp, "#ifndef __STUB_H__\n");
	output(fp, "#define __STUB_H__\n\n");

	output_struct_info(fp, struct_list);
	output_func_info(fp, func_list);

	output(fp, "\n#endif\n");

	fclose(fp);
	return 0;
}

int generate_stub_c(LIST_S * struct_list, LIST_S * func_list, FILE * fp) {
	output_header(fp, 1);
	output_cplus_start(fp);
	output_struct_member(fp, struct_list);
	output_struct_pair(fp, struct_list);
	output_func_para(fp, func_list);
	output_return_para(fp);
	output_func_interface(fp, func_list);
	output_func_map(fp, func_list, 1);
	
	output_func_definition(fp, func_list);
	output_cplus_end(fp);

	fclose(fp);
	return 0;
}

int generate_proxier_c(LIST_S * struct_list, LIST_S * func_list, FILE * fp) {
	output_header(fp, 0);
	output_cplus_start(fp);
	output_struct_member(fp, struct_list);
	output_struct_pair(fp, struct_list);
	output_func_para(fp, func_list);
	output_return_para(fp);
/*	output_func_interface(fp, func_list);*/
	output_func_map(fp, func_list, 0);
	
	output_proxier_func(fp, func_list);
	output_cplus_end(fp);

	fclose(fp);
	return 0;
}

static unsigned int min_func_id;
static NODE_S * min_node;

int get_min_id(void * src, void * dst) {
	struct func_info * func_info = (struct func_info *)dst;
	if (func_info->func_id < min_func_id) {
		min_func_id = func_info->func_id;
		min_node = to_list_node(func_info);
	}

	return 0;
}

void func_sort(LIST_S * func_list) {
	int func_num;
	int i, j;

	LIST_S stList;
	struct func_info * func_info;

	memset(&stList, 0, sizeof(stList));
	list_init(&stList, 0, 0, 0, 0);

	do {
		min_func_id = 0xffffffff;
		func_num = list_size(func_list);
		if (func_num <= 0) {
			break;
		}
		
		list_operate_it(func_list, get_min_id);
		if (min_node) {
			list_write_data(&stList, min_node->data, sizeof(struct func_info), 0);
			list_delete_node(func_list, min_node);
		}
	} while(1);

	list_finit(func_list);

	*func_list = stList;
}

int main(int argc, char * argv[]) {
	int struct_num;
	int func_num;
	int i, j;
	int ret;

	if (argc > 1) {
		infile = argv[1];
		if (0 != set_input_file(infile)) {
			trace("set_input_file %s failed\n", infile);
			return -1;
		}
	}

	if (0 != rpc_init()) {
		trace("rpc_init failed\n");
		return -1;
	}

	ret = yyparse();
	if (0 != ret) {
		trace("sytax error\n");
		return -1;
	}

	func_sort(&g_func_list);

	// generate_stub_h(&g_struct_list, &g_func_list, stub_h_fp);
	generate_stub_c(&g_struct_list, &g_func_list, stub_c_fp);
	generate_proxier_c(&g_struct_list, &g_func_list, proxier_c_fp);

	//rpc_display();

	rpc_finit();

	return 0;
}

int gen_field_type(struct parameter * para) {
	// string
	if (para->string && \
		0 == memcmp(para->type, "char", strlen("char")) && \
		para->len < 0) {		
		return RMI_FIELD_LEN;	
	}

	// struct
	if (strstr(para->type, "struct")) {	
		return RMI_FIELD_LEN;
	}

	// float
	if (strstr(para->type, "float") || \
		strstr(para->type, "double")) {
		return RMI_FIELD_FIX;
	}

	// int
	if (strstr(para->type, "unsigned")) {
		return RMI_FIELD_VAR;
	} else {
		return RMI_FIELD_SIGNED;
	}	
}

