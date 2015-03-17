
D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%{
#include <stdio.h>
#include "c_grammer.tab.h" 

void get_token_name();
void get_token_value();
int set_input_file(char * infile);
%}


%x incl
%x def
%x comment

%%
"/*"				{ BEGIN(comment); }
<comment>[^*]*      {/* eat anything that's not a '*' */}
<comment>"*"+[^*/]* {/* eat up '*'s not followed by '/'s */}
<comment>"*"+"/"    { BEGIN(INITIAL); }

"//"(.)*[^\n]		{ }

(#include)(.)*\n    {}
                                              
(#define)(.)*\n    	{}

"#ifdef"(.)*\n		{ }
"#else"(.)*\n		{ }
"#endif"(.)*\n		{ }
"#ifndef"(.)*\n		{ }
	
"IN"				{get_token_name(); return(DIR);}
"OUT"				{get_token_name(); return(DIR);}

"MARK"				{get_token_name(); return(MARK);}

"struct rmi * rmi"(,)?	{}

"auto"				{ get_token_name(); return(AUTO); }
"break"				{ get_token_name(); return(BREAK); }
"case"				{ get_token_name(); return(CASE); }
"char"				{ get_token_name(); return(CHAR); }
"const"				{ get_token_name(); return(CONST); }
"continue"			{ get_token_name(); return(CONTINUE); }
"default"			{ get_token_name(); return(DEFAULT); }
"do"				{ get_token_name(); return(DO); }
"double"			{ get_token_name(); return(DOUBLE); }
"else"				{ get_token_name(); return(ELSE); }
"enum"				{ get_token_name(); return(ENUM); }
"extern"			{ get_token_name(); return(EXTERN); }
"float"				{ get_token_name(); return(FLOAT); }
"for"				{ get_token_name(); return(FOR); }
"goto"				{ get_token_name(); return(GOTO); }
"if"				{ get_token_name(); return(IF); }
"int"				{ get_token_name(); return(INT); }
"long"				{ get_token_name(); return(LONG); }
"register"			{ get_token_name(); return(REGISTER); }
"return"			{ get_token_name(); return(RETURN); }
"short"				{ get_token_name(); return(SHORT); }
"signed"			{ get_token_name(); return(SIGNED); }
"sizeof"			{ get_token_name(); return(SIZEOF); }
"static"			{ get_token_name(); return(STATIC); }
"struct"			{ get_token_name(); return(STRUCT); }
"switch"			{ get_token_name(); return(SWITCH); }
"typedef"			{ get_token_name(); return(TYPEDEF); }
"union"				{ get_token_name(); return(UNION); }
"unsigned"			{ get_token_name(); return(UNSIGNED); }
"void"				{ get_token_name(); return(VOID); }
"volatile"			{ get_token_name(); return(VOLATILE); }
"while"				{ get_token_name(); return(WHILE); }
					
{L}({L}|{D})*		{ get_token_name(); return(check_type()); }

0[xX]{H}+{IS}?		{ get_token_value(); return(CONSTANT); }
0{D}+{IS}?			{ get_token_value(); return(CONSTANT); }
{D}+{IS}?			{ get_token_value(); return(CONSTANT); }
L?'(\\.|[^\\'])+'	{ get_token_value(); return(CONSTANT); }
					
{D}+{E}{FS}?		{ get_token_value(); return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?	{ get_token_value(); return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?	{ get_token_value(); return(CONSTANT); }

L?\"(\\.|[^\\"])*\"	{ get_token_name(); return(STRING_LITERAL); }

"..."				{ get_token_name(); return(ELLIPSIS); }
">>="				{ get_token_name(); return(RIGHT_ASSIGN); }
"<<="				{ get_token_name(); return(LEFT_ASSIGN); }
"+="				{ get_token_name(); return(ADD_ASSIGN); }
"-="				{ get_token_name(); return(SUB_ASSIGN); }
"*="				{ get_token_name(); return(MUL_ASSIGN); }
"/="				{ get_token_name(); return(DIV_ASSIGN); }
"%="				{ get_token_name(); return(MOD_ASSIGN); }
"&="				{ get_token_name(); return(AND_ASSIGN); }
"^="				{ get_token_name(); return(XOR_ASSIGN); }
"|="				{ get_token_name(); return(OR_ASSIGN); }
">>"				{ get_token_name(); return(RIGHT_OP); }
"<<"				{ get_token_name(); return(LEFT_OP); }
"++"				{ get_token_name(); return(INC_OP); }
"--"				{ get_token_name(); return(DEC_OP); }
"->"				{ get_token_name(); return(PTR_OP); }
"&&"				{ get_token_name(); return(AND_OP); }
"||"				{ get_token_name(); return(OR_OP); }
"<="				{ get_token_name(); return(LE_OP); }
">="				{ get_token_name(); return(GE_OP); }
"=="				{ get_token_name(); return(EQ_OP); }
"!="				{ get_token_name(); return(NE_OP); }
";"					{ get_token_value(); return(';'); }
"{"					{ get_token_value(); return('{'); }
"}"					{ get_token_value(); return('}'); }
","					{ get_token_value(); return(','); }
":"					{ get_token_value(); return(':'); }
"="					{ get_token_value(); return('='); }
"("					{ get_token_value(); return('('); }
")"					{ get_token_value(); return(')'); }
"["					{ get_token_value(); return('['); }
"]"					{ get_token_value(); return(']'); }
"."					{ get_token_value(); return('.'); }
"&"					{ get_token_value(); return('&'); }
"!"					{ get_token_value(); return('!'); }
"~"					{ get_token_value(); return('~'); }
"-"					{ get_token_value(); return('-'); }
"+"					{ get_token_value(); return('+'); }
"*"					{ get_token_value(); return('*'); }
"/"					{ get_token_value(); return('/'); }
"%"					{ get_token_value(); return('%'); }
"<"					{ get_token_value(); return('<'); }
">"					{ get_token_value(); return('>'); }
"^"					{ get_token_value(); return('^'); }
"|"					{ get_token_value(); return('|'); }
"?"					{ get_token_value(); return('?'); }
"\\\n"				{ }
	
[ \t\v\n\f]			{ }
.					{ printf("error\n"); exit(0); }

%%

yywrap()
{
	return(1);
}

void get_token_name() {
	yylval.name = strdup(yytext);
}

void get_token_value() {
	yylval.val = atoi(yytext);
}

int check_type()
{
/*
* pseudo code --- this is what it should check
*
*	if (yytext == type_name)
*		return(TYPE_NAME);
*
*	return(IDENTIFIER);
*/

/*
*	it actually will only return IDENTIFIER
*/

	return(IDENTIFIER);
}

int set_input_file(char * infile) {
	yyin = fopen( infile, "r" );

	if ( ! yyin ) {
		return -1;
	}

	yy_switch_to_buffer(yy_create_buffer( yyin, YY_BUF_SIZE ));
	
	return 0;
}

/*
From : http://www.lysator.liu.se/c/ANSI-C-grammar-l.html
*/