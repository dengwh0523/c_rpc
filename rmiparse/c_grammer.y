%union {
	int val;
	char * name;
}

%{
#include <stdio.h>
#include <string.h>
#include "rmiparse.h"

static char g_type[128]; // 用来存储当前正解析到的函数返回类型或参数类型
static char g_name[128]; // 用来存储当前正解析到的函数名字或参数名字
static int array_len;	// 数组长度
static struct parameter s_para; // 用来存储当前正解析到的参数信息
static struct struct_info s_struct; // 用来存储当前正解析到的结构体信息
static struct func_info s_func; // 用来存储当前正解析到的函数信息

int is_func = 0;

void init(void);
%}

%token <val> CONSTANT
%token <name> IDENTIFIER STRING_LITERAL SIZEOF
%token <name> PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token <name> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token <name> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token <name> XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token <name> TYPEDEF EXTERN STATIC AUTO REGISTER
%token <name> CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token <name> STRUCT UNION ENUM ELLIPSIS

%token <name> CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token <name> MARK DIR

%type  <name> direct_declarator declaration_specifiers struct_or_union
%type  <name> specifier_qualifier_list declarator type_specifier

%start translation_unit
%%


primary_expression
	: IDENTIFIER
	| CONSTANT
	{
		array_len = $1;
	}
	| STRING_LITERAL
	| '(' expression ')'
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	;

argument_expression_list
	: assignment_expression
	| argument_expression_list ',' assignment_expression
	;

unary_expression
	: postfix_expression
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| unary_operator cast_expression
	| SIZEOF unary_expression
	| SIZEOF '(' type_name ')'
	;

unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

cast_expression
	: unary_expression
	| '(' type_name ')' cast_expression
	;

multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression
	| multiplicative_expression '/' cast_expression
	| multiplicative_expression '%' cast_expression
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LE_OP shift_expression
	| relational_expression GE_OP shift_expression
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
	: conditional_expression
	| unary_expression assignment_operator assignment_expression
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expression
	: assignment_expression
	| expression ',' assignment_expression
	;

constant_expression
	: conditional_expression
	;

declaration
	: declaration_specifiers ';'
	{
		//printf("111\n");
	}
	| declaration_specifiers init_declarator_list ';'
	{
		//printf("222\n");
		if (is_func) {
			strcat(s_func.ret_type, $1);
			s_func.func_id = crc32(s_func.func_name, strlen(s_func.func_name));
			list_write_data(&g_func_list, (unsigned char *)&s_func, sizeof(s_func), 0);
			
			memset(&s_func, 0, sizeof(s_func));
			list_init(&s_func.para_list, BUF_SIZE, LIST_MAX_TIME, LIST_MAX_PACKET, LIST_MAX_USER);
		}
		is_func = 0;
	}
	;

declaration_specifiers
	: storage_class_specifier
	{
	}
	| storage_class_specifier declaration_specifiers
	{
	}
	| type_specifier
	{
	}
	| type_specifier declaration_specifiers
	{
		char last_type[sizeof(g_type)] = {0};
		strcpy(last_type, g_type);
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, $$);
		strcat(g_type, " ");
		strcat(g_type, last_type);

		if ($$) free($$);
		$$ = strdup(g_type);
	}
	| type_qualifier
	{
	}
	| type_qualifier declaration_specifiers
	{
	}
	;

init_declarator_list
	: init_declarator
	| init_declarator_list ',' init_declarator
	;

init_declarator
	: declarator
	| declarator '=' initializer
	;

storage_class_specifier
	: TYPEDEF
	| EXTERN
	| STATIC
	| AUTO
	| REGISTER
	;

type_specifier
	: VOID	
	{	
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, $1);
	}
	| CHAR	
	{
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, $1);
	}
	| SHORT 
	{
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, $1);
	}
	| INT
	{
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, $1);
	}
	| LONG
	{
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, $1);
	}
	| FLOAT
	{
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, $1);
	}
	| DOUBLE
	{
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, $1);
	}
	| SIGNED
	{
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, $1);
	}
	| UNSIGNED
	{
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, $1);
	}
	| struct_or_union_specifier
	{
		if ($$) free($$);
		$$ = strdup(g_type);
	}
	| enum_specifier
	{
	}
	| TYPE_NAME
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}' 
	{
		strcpy(s_struct.type, $1);
		strcat(s_struct.type, " ");
		strcat(s_struct.type, $2);
		strcpy(s_struct.name, $2);
		
		list_write_data(&g_struct_list, (unsigned char *)&s_struct, sizeof(s_struct), 0);
		
		memset(&s_struct, 0, sizeof(s_struct));
		list_init(&s_struct.para_list, BUF_SIZE, LIST_MAX_TIME, LIST_MAX_PACKET, LIST_MAX_USER);
	}
	| struct_or_union '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER
	{
		strcpy(g_type, $1);
		strcat(g_type, " ");
		strcat(g_type, $2);
	}
	;

struct_or_union
	: STRUCT
	| UNION
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'
	{
		//printf("%s\n", $1);
		list_write_data(&s_struct.para_list, (unsigned char *)&s_para, sizeof(s_para), 0);
		memset(&s_para, 0, sizeof(s_para));
	}
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	{
		char last_type[sizeof(g_type)] = {0};
		strcpy(last_type, g_type);
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, $$);
		strcat(g_type, " ");
		strcat(g_type, last_type);
	}
	| type_specifier
	{
	}
	| type_qualifier specifier_qualifier_list
	{
	}
	| type_qualifier
	{
	}
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: declarator
	{
		//memset(&s_para, 0, sizeof(s_para));
		strcpy(s_para.name, g_name);
		strcpy(s_para.type, g_type);
		s_para.len = array_len;
		
		memset(g_name, 0, sizeof(g_name));
		memset(g_type, 0, sizeof(g_type));
		array_len = 1;
	}
	| declarator MARK '(' CONSTANT ',' CONSTANT ')'
	{
		//memset(&s_para, 0, sizeof(s_para));
		strcpy(s_para.name, g_name);
		strcpy(s_para.type, g_type);
		s_para.len = array_len;
		s_para.string = $6;
		s_para.field_num = $4;
		s_para.field_type = gen_field_type(&s_para);
		
		memset(g_name, 0, sizeof(g_name));
		memset(g_type, 0, sizeof(g_type));
		array_len = 1;
	}
	| ':' constant_expression
	| declarator ':' constant_expression
	;

enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM IDENTIFIER
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator
	: IDENTIFIER
	| IDENTIFIER '=' constant_expression
	;

type_qualifier
	: CONST
	| VOLATILE
	;

declarator
	: pointer direct_declarator
	{
		if (is_func) {
			s_func.pointer = 1;
		} else {
			s_para.pointer = 1;
		}
	}
	| direct_declarator
	;

direct_declarator
	: IDENTIFIER
	{
		strcpy(g_name, $1);
	}
	| '(' declarator ')'
	{
	}
	| direct_declarator '[' constant_expression ']'
	| direct_declarator '[' ']'
	| direct_declarator '(' parameter_type_list ')'
	{
		strcat(s_func.func_name, $1);
		is_func = 1;
	}
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '(' ')'
	{
		strcat(s_func.func_name, $1);
		is_func = 1;
	}
	;

pointer
	: '*'
	| '*' type_qualifier_list
	| '*' pointer
	| '*' type_qualifier_list pointer
	;

type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;


parameter_type_list
	: parameter_list
	| parameter_list ',' ELLIPSIS
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: DIR declaration_specifiers declarator	
	{
		//printf("is_func: %d, pointer: %d\n", is_func, s_para.pointer);
		//memset(&s_para, 0, sizeof(s_para));
		strcpy(s_para.name, g_name);
		strcpy(s_para.type, g_type);
		s_para.len = array_len;
		if (0 == memcmp($1, "IN", strlen("IN"))) {
			s_para.dir = PARA_IN;
		} else if (0 == memcmp($1, "OUT", strlen("OUT"))) {
			s_para.dir = PARA_OUT;
		}
		if (PARA_OUT == s_para.dir && 1 != s_para.pointer) {
			return -1;
		}
		s_para.field_type = gen_field_type(&s_para);
		
		list_write_data(&s_func.para_list, (unsigned char *)&s_para, sizeof(s_para), 0);
		memset(&s_para, 0, sizeof(s_para));
		
		memset(g_name, 0, sizeof(g_name));
		memset(g_type, 0, sizeof(g_type));
		array_len = 1;
	}
	| declaration_specifiers abstract_declarator
	| DIR declaration_specifiers
	{
		//memset(&s_para, 0, sizeof(s_para));
		//strcpy(s_para.name, g_name);
		strcpy(s_para.type, g_type);
		s_para.len = array_len;
		sprintf(s_para.name, "para%d", list_size(&s_func.para_list));
		if (0 == memcmp($1, "IN", strlen("IN"))) {
			s_para.dir = PARA_IN;
		} else if (0 == memcmp($1, "OUT", strlen("OUT"))) {
			s_para.dir = PARA_OUT;
		}
		if (PARA_OUT == s_para.dir && 1 != s_para.pointer) {
			return -1;
		}
		
		list_write_data(&s_func.para_list, (unsigned char *)&s_para, sizeof(s_para), 0);
		memset(&s_para, 0, sizeof(s_para));
		
		memset(g_name, 0, sizeof(g_name));
		memset(g_type, 0, sizeof(g_type));
		array_len = 1;
	}
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

type_name
	: specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator
	;

abstract_declarator
	: pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' constant_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' constant_expression ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	;

initializer
	: assignment_expression
	| '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	;

initializer_list
	: initializer
	| initializer_list ',' initializer
	;

statement
	: labeled_statement
	| compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

labeled_statement
	: IDENTIFIER ':' statement
	| CASE constant_expression ':' statement
	| DEFAULT ':' statement
	;

compound_statement
	: '{' '}'
	| '{' statement_list '}'
	| '{' declaration_list '}'
	| '{' declaration_list statement_list '}'
	;

declaration_list
	: declaration
	| declaration_list declaration
	;

statement_list
	: statement
	| statement_list statement
	;

expression_statement
	: ';'
	| expression ';'
	;

selection_statement
	: IF '(' expression ')' statement
	| IF '(' expression ')' statement ELSE statement
	| SWITCH '(' expression ')' statement
	;

iteration_statement
	: WHILE '(' expression ')' statement
	| DO statement WHILE '(' expression ')' ';'
	| FOR '(' expression_statement expression_statement ')' statement
	| FOR '(' expression_statement expression_statement expression ')' statement
	;

jump_statement
	: GOTO IDENTIFIER ';'
	| CONTINUE ';'
	| BREAK ';'
	| RETURN ';'
	| RETURN expression ';'
	;

translation_unit
	: external_declaration
	| translation_unit external_declaration
	;

external_declaration
	: function_definition
	| declaration
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement
	| declaration_specifiers declarator compound_statement
	| declarator declaration_list compound_statement
	| declarator compound_statement
	;

%%
#include <stdio.h>

extern char yytext[];

yyerror(s)
char *s;
{
	fflush(stdout);
	//printf("\n%*s\n%*s\n", column, "^", column, s);
	printf("\n%s\n", s);
}

/*
main() {
	// yydebug = 1;
	//while(0 != yylex());
	yyparse();
}
*/
void init() {
	memset(g_name, 0, sizeof(g_name));
	memset(g_type, 0, sizeof(g_type));
	memset(&s_para, 0, sizeof(s_para));
	memset(&s_struct, 0, sizeof(s_struct));
	list_init(&s_struct.para_list, BUF_SIZE, LIST_MAX_TIME, LIST_MAX_PACKET, LIST_MAX_USER);
	memset(&s_func, 0, sizeof(s_func));
	list_init(&s_func.para_list, BUF_SIZE, LIST_MAX_TIME, LIST_MAX_PACKET, LIST_MAX_USER);
	array_len = 1;
}
