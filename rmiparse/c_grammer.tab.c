/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */


/* Line 268 of yacc.c  */
#line 71 "c_grammer.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     CONSTANT = 258,
     IDENTIFIER = 259,
     STRING_LITERAL = 260,
     SIZEOF = 261,
     NEWTYPE = 262,
     PTR_OP = 263,
     INC_OP = 264,
     DEC_OP = 265,
     LEFT_OP = 266,
     RIGHT_OP = 267,
     LE_OP = 268,
     GE_OP = 269,
     EQ_OP = 270,
     NE_OP = 271,
     AND_OP = 272,
     OR_OP = 273,
     MUL_ASSIGN = 274,
     DIV_ASSIGN = 275,
     MOD_ASSIGN = 276,
     ADD_ASSIGN = 277,
     SUB_ASSIGN = 278,
     LEFT_ASSIGN = 279,
     RIGHT_ASSIGN = 280,
     AND_ASSIGN = 281,
     XOR_ASSIGN = 282,
     OR_ASSIGN = 283,
     TYPE_NAME = 284,
     TYPEDEF = 285,
     EXTERN = 286,
     STATIC = 287,
     AUTO = 288,
     REGISTER = 289,
     CHAR = 290,
     SHORT = 291,
     INT = 292,
     LONG = 293,
     SIGNED = 294,
     UNSIGNED = 295,
     FLOAT = 296,
     DOUBLE = 297,
     CONST = 298,
     VOLATILE = 299,
     VOID = 300,
     STRUCT = 301,
     UNION = 302,
     ENUM = 303,
     ELLIPSIS = 304,
     CASE = 305,
     DEFAULT = 306,
     IF = 307,
     ELSE = 308,
     SWITCH = 309,
     WHILE = 310,
     DO = 311,
     FOR = 312,
     GOTO = 313,
     CONTINUE = 314,
     BREAK = 315,
     RETURN = 316,
     MARK = 317,
     DIR = 318
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 1 "c_grammer.y"

	int val;
	char * name;



/* Line 293 of yacc.c  */
#line 177 "c_grammer.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 343 of yacc.c  */
#line 6 "c_grammer.y"

#include <stdio.h>
#include <string.h>
#include "rmiparse.h"

#define trace(fmt, ...) \
do{\
	char __buf1[64], __buf2[1024];\
    snprintf(__buf1, sizeof(__buf1), "line[%d]: ", yylineno);\
    snprintf(__buf2, sizeof(__buf2), fmt, ##__VA_ARGS__);\
    printf("%s%s", __buf1, __buf2);\
} while(0)

static char g_type[128]; // 用来存储当前正解析到的函数返回类型或参数类型
static char g_name[128]; // 用来存储当前正解析到的函数名字或参数名字
static int array_len;	// 数组长度
static struct parameter s_para; // 用来存储当前正解析到的参数信息
static struct struct_info s_struct; // 用来存储当前正解析到的结构体信息
static struct func_info s_func; // 用来存储当前正解析到的函数信息
static struct newtype_info s_newtype;

int is_func = 0;
int is_typedef = 0;
int is_struct_def = 0;

int is_first_para = 1;

void init(void);
int write_func_info(char *);
void write_struct_info(void);
void write_newtype_info(void);
int write_func_para(char * name, char * dir);
int check_func_first_para();


/* Line 343 of yacc.c  */
#line 225 "c_grammer.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  62
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1352

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  88
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  64
/* YYNRULES -- Number of rules.  */
#define YYNRULES  217
/* YYNRULES -- Number of states.  */
#define YYNSTATES  361

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   318

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    75,     2,     2,     2,    77,    70,     2,
      64,    65,    71,    72,    69,    73,    68,    76,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    83,    85,
      78,    84,    79,    82,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    66,     2,    67,    80,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    86,    81,    87,    74,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    13,    15,    20,    24,
      29,    33,    37,    40,    43,    45,    49,    51,    54,    57,
      60,    63,    68,    70,    72,    74,    76,    78,    80,    82,
      87,    89,    93,    97,   101,   103,   107,   111,   113,   117,
     121,   123,   127,   131,   135,   139,   141,   145,   149,   151,
     155,   157,   161,   163,   167,   169,   173,   175,   179,   181,
     187,   189,   193,   195,   197,   199,   201,   203,   205,   207,
     209,   211,   213,   215,   217,   221,   223,   226,   230,   232,
     235,   237,   240,   242,   245,   247,   251,   253,   257,   259,
     261,   263,   265,   267,   269,   271,   273,   275,   277,   279,
     281,   283,   285,   287,   289,   291,   293,   299,   304,   307,
     309,   311,   313,   316,   320,   323,   325,   328,   330,   332,
     336,   338,   346,   349,   353,   358,   364,   367,   369,   373,
     375,   379,   381,   383,   386,   388,   390,   394,   399,   403,
     408,   413,   417,   419,   422,   425,   429,   431,   434,   436,
     440,   442,   446,   449,   451,   454,   458,   462,   465,   467,
     471,   473,   476,   478,   480,   483,   487,   490,   494,   498,
     503,   506,   510,   514,   519,   521,   525,   530,   532,   536,
     538,   540,   542,   544,   546,   548,   552,   557,   561,   564,
     568,   572,   577,   579,   582,   584,   587,   589,   592,   598,
     606,   612,   618,   626,   633,   641,   645,   648,   651,   654,
     658,   660,   663,   665,   667,   672,   676,   680
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     149,     0,    -1,     4,    -1,     3,    -1,     5,    -1,    64,
     108,    65,    -1,    89,    -1,    90,    66,   108,    67,    -1,
      90,    64,    65,    -1,    90,    64,    91,    65,    -1,    90,
      68,     4,    -1,    90,     8,     4,    -1,    90,     9,    -1,
      90,    10,    -1,   106,    -1,    91,    69,   106,    -1,    90,
      -1,     9,    92,    -1,    10,    92,    -1,    93,    94,    -1,
       6,    92,    -1,     6,    64,   135,    65,    -1,    70,    -1,
      71,    -1,    72,    -1,    73,    -1,    74,    -1,    75,    -1,
      92,    -1,    64,   135,    65,    94,    -1,    94,    -1,    95,
      71,    94,    -1,    95,    76,    94,    -1,    95,    77,    94,
      -1,    95,    -1,    96,    72,    95,    -1,    96,    73,    95,
      -1,    96,    -1,    97,    11,    96,    -1,    97,    12,    96,
      -1,    97,    -1,    98,    78,    97,    -1,    98,    79,    97,
      -1,    98,    13,    97,    -1,    98,    14,    97,    -1,    98,
      -1,    99,    15,    98,    -1,    99,    16,    98,    -1,    99,
      -1,   100,    70,    99,    -1,   100,    -1,   101,    80,   100,
      -1,   101,    -1,   102,    81,   101,    -1,   102,    -1,   103,
      17,   102,    -1,   103,    -1,   104,    18,   103,    -1,   104,
      -1,   104,    82,   108,    83,   105,    -1,   105,    -1,    92,
     107,   106,    -1,    84,    -1,    19,    -1,    20,    -1,    21,
      -1,    22,    -1,    23,    -1,    24,    -1,    25,    -1,    26,
      -1,    27,    -1,    28,    -1,   106,    -1,   108,    69,   106,
      -1,   105,    -1,   111,    85,    -1,   111,   112,    85,    -1,
     114,    -1,   114,   111,    -1,   115,    -1,   115,   111,    -1,
     126,    -1,   126,   111,    -1,   113,    -1,   112,    69,   113,
      -1,   127,    -1,   127,    84,   138,    -1,    30,    -1,    31,
      -1,    32,    -1,    33,    -1,    34,    -1,    45,    -1,    35,
      -1,    36,    -1,    37,    -1,    38,    -1,    41,    -1,    42,
      -1,    39,    -1,    40,    -1,   116,    -1,   123,    -1,    29,
      -1,     7,    -1,   117,     4,    86,   118,    87,    -1,   117,
      86,   118,    87,    -1,   117,     4,    -1,    46,    -1,    47,
      -1,   119,    -1,   118,   119,    -1,   120,   121,    85,    -1,
     115,   120,    -1,   115,    -1,   126,   120,    -1,   126,    -1,
     122,    -1,   121,    69,   122,    -1,   127,    -1,   127,    62,
      64,     3,    69,     3,    65,    -1,    83,   109,    -1,   127,
      83,   109,    -1,    48,    86,   124,    87,    -1,    48,     4,
      86,   124,    87,    -1,    48,     4,    -1,   125,    -1,   124,
      69,   125,    -1,     4,    -1,     4,    84,   109,    -1,    43,
      -1,    44,    -1,   129,   128,    -1,   128,    -1,     4,    -1,
      64,   127,    65,    -1,   128,    66,   109,    67,    -1,   128,
      66,    67,    -1,   128,    64,   131,    65,    -1,   128,    64,
     134,    65,    -1,   128,    64,    65,    -1,    71,    -1,    71,
     130,    -1,    71,   129,    -1,    71,   130,   129,    -1,   126,
      -1,   130,   126,    -1,   132,    -1,   132,    69,    49,    -1,
     133,    -1,   132,    69,   133,    -1,   111,   127,    -1,   111,
      -1,   111,   136,    -1,    63,   111,   127,    -1,    63,   111,
     136,    -1,    63,   111,    -1,     4,    -1,   134,    69,     4,
      -1,   120,    -1,   120,   136,    -1,   129,    -1,   137,    -1,
     129,   137,    -1,    64,   136,    65,    -1,    66,    67,    -1,
      66,   109,    67,    -1,   137,    66,    67,    -1,   137,    66,
     109,    67,    -1,    64,    65,    -1,    64,   131,    65,    -1,
     137,    64,    65,    -1,   137,    64,   131,    65,    -1,   106,
      -1,    86,   139,    87,    -1,    86,   139,    69,    87,    -1,
     138,    -1,   139,    69,   138,    -1,   141,    -1,   142,    -1,
     145,    -1,   146,    -1,   147,    -1,   148,    -1,     4,    83,
     140,    -1,    50,   109,    83,   140,    -1,    51,    83,   140,
      -1,    86,    87,    -1,    86,   144,    87,    -1,    86,   143,
      87,    -1,    86,   143,   144,    87,    -1,   110,    -1,   143,
     110,    -1,   140,    -1,   144,   140,    -1,    85,    -1,   108,
      85,    -1,    52,    64,   108,    65,   140,    -1,    52,    64,
     108,    65,   140,    53,   140,    -1,    54,    64,   108,    65,
     140,    -1,    55,    64,   108,    65,   140,    -1,    56,   140,
      55,    64,   108,    65,    85,    -1,    57,    64,   145,   145,
      65,   140,    -1,    57,    64,   145,   145,   108,    65,   140,
      -1,    58,     4,    85,    -1,    59,    85,    -1,    60,    85,
      -1,    61,    85,    -1,    61,   108,    85,    -1,   150,    -1,
     149,   150,    -1,   151,    -1,   110,    -1,   111,   127,   143,
     142,    -1,   111,   127,   142,    -1,   127,   143,   142,    -1,
     127,   142,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    64,    64,    65,    69,    70,    74,    75,    76,    77,
      78,    79,    80,    81,    85,    86,    90,    91,    92,    93,
      94,    95,    99,   100,   101,   102,   103,   104,   108,   109,
     113,   114,   115,   116,   120,   121,   122,   126,   127,   128,
     132,   133,   134,   135,   136,   140,   141,   142,   146,   147,
     151,   152,   156,   157,   161,   162,   166,   167,   171,   172,
     176,   177,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   195,   196,   200,   204,   209,   219,   222,
     226,   229,   241,   244,   250,   251,   255,   256,   260,   261,
     262,   263,   264,   268,   273,   278,   283,   288,   293,   298,
     303,   308,   313,   318,   321,   322,   330,   345,   350,   359,
     360,   364,   365,   369,   378,   387,   390,   393,   399,   400,
     404,   409,   423,   424,   428,   429,   430,   434,   435,   439,
     440,   444,   445,   449,   457,   461,   465,   468,   472,   473,
     478,   479,   489,   490,   491,   492,   496,   497,   502,   503,
     507,   508,   512,   518,   524,   530,   536,   544,   555,   556,
     560,   561,   565,   569,   570,   574,   575,   576,   577,   578,
     579,   580,   581,   582,   586,   587,   588,   592,   593,   597,
     598,   599,   600,   601,   602,   606,   607,   608,   612,   613,
     614,   615,   619,   620,   624,   625,   629,   630,   634,   635,
     636,   640,   641,   642,   643,   647,   648,   649,   650,   651,
     655,   656,   660,   661,   665,   666,   667,   668
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CONSTANT", "IDENTIFIER",
  "STRING_LITERAL", "SIZEOF", "NEWTYPE", "PTR_OP", "INC_OP", "DEC_OP",
  "LEFT_OP", "RIGHT_OP", "LE_OP", "GE_OP", "EQ_OP", "NE_OP", "AND_OP",
  "OR_OP", "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN", "ADD_ASSIGN",
  "SUB_ASSIGN", "LEFT_ASSIGN", "RIGHT_ASSIGN", "AND_ASSIGN", "XOR_ASSIGN",
  "OR_ASSIGN", "TYPE_NAME", "TYPEDEF", "EXTERN", "STATIC", "AUTO",
  "REGISTER", "CHAR", "SHORT", "INT", "LONG", "SIGNED", "UNSIGNED",
  "FLOAT", "DOUBLE", "CONST", "VOLATILE", "VOID", "STRUCT", "UNION",
  "ENUM", "ELLIPSIS", "CASE", "DEFAULT", "IF", "ELSE", "SWITCH", "WHILE",
  "DO", "FOR", "GOTO", "CONTINUE", "BREAK", "RETURN", "MARK", "DIR", "'('",
  "')'", "'['", "']'", "'.'", "','", "'&'", "'*'", "'+'", "'-'", "'~'",
  "'!'", "'/'", "'%'", "'<'", "'>'", "'^'", "'|'", "'?'", "':'", "'='",
  "';'", "'{'", "'}'", "$accept", "primary_expression",
  "postfix_expression", "argument_expression_list", "unary_expression",
  "unary_operator", "cast_expression", "multiplicative_expression",
  "additive_expression", "shift_expression", "relational_expression",
  "equality_expression", "and_expression", "exclusive_or_expression",
  "inclusive_or_expression", "logical_and_expression",
  "logical_or_expression", "conditional_expression",
  "assignment_expression", "assignment_operator", "expression",
  "constant_expression", "declaration", "declaration_specifiers",
  "init_declarator_list", "init_declarator", "storage_class_specifier",
  "type_specifier", "struct_or_union_specifier", "struct_or_union",
  "struct_declaration_list", "struct_declaration",
  "specifier_qualifier_list", "struct_declarator_list",
  "struct_declarator", "enum_specifier", "enumerator_list", "enumerator",
  "type_qualifier", "declarator", "direct_declarator", "pointer",
  "type_qualifier_list", "parameter_type_list", "parameter_list",
  "parameter_declaration", "identifier_list", "type_name",
  "abstract_declarator", "direct_abstract_declarator", "initializer",
  "initializer_list", "statement", "labeled_statement",
  "compound_statement", "declaration_list", "statement_list",
  "expression_statement", "selection_statement", "iteration_statement",
  "jump_statement", "translation_unit", "external_declaration",
  "function_definition", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,    40,    41,    91,    93,    46,    44,
      38,    42,    43,    45,   126,    33,    47,    37,    60,    62,
      94,   124,    63,    58,    61,    59,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    88,    89,    89,    89,    89,    90,    90,    90,    90,
      90,    90,    90,    90,    91,    91,    92,    92,    92,    92,
      92,    92,    93,    93,    93,    93,    93,    93,    94,    94,
      95,    95,    95,    95,    96,    96,    96,    97,    97,    97,
      98,    98,    98,    98,    98,    99,    99,    99,   100,   100,
     101,   101,   102,   102,   103,   103,   104,   104,   105,   105,
     106,   106,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   108,   108,   109,   110,   110,   111,   111,
     111,   111,   111,   111,   112,   112,   113,   113,   114,   114,
     114,   114,   114,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   116,   116,   116,   117,
     117,   118,   118,   119,   120,   120,   120,   120,   121,   121,
     122,   122,   122,   122,   123,   123,   123,   124,   124,   125,
     125,   126,   126,   127,   127,   128,   128,   128,   128,   128,
     128,   128,   129,   129,   129,   129,   130,   130,   131,   131,
     132,   132,   133,   133,   133,   133,   133,   133,   134,   134,
     135,   135,   136,   136,   136,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   138,   138,   138,   139,   139,   140,
     140,   140,   140,   140,   140,   141,   141,   141,   142,   142,
     142,   142,   143,   143,   144,   144,   145,   145,   146,   146,
     146,   147,   147,   147,   147,   148,   148,   148,   148,   148,
     149,   149,   150,   150,   151,   151,   151,   151
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     3,     1,     4,     3,     4,
       3,     3,     2,     2,     1,     3,     1,     2,     2,     2,
       2,     4,     1,     1,     1,     1,     1,     1,     1,     4,
       1,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     5,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     2,     3,     1,     2,
       1,     2,     1,     2,     1,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     5,     4,     2,     1,
       1,     1,     2,     3,     2,     1,     2,     1,     1,     3,
       1,     7,     2,     3,     4,     5,     2,     1,     3,     1,
       3,     1,     1,     2,     1,     1,     3,     4,     3,     4,
       4,     3,     1,     2,     2,     3,     1,     2,     1,     3,
       1,     3,     2,     1,     2,     3,     3,     2,     1,     3,
       1,     2,     1,     1,     2,     3,     2,     3,     3,     4,
       2,     3,     3,     4,     1,     3,     4,     1,     3,     1,
       1,     1,     1,     1,     1,     3,     4,     3,     2,     3,
       3,     4,     1,     2,     1,     2,     1,     2,     5,     7,
       5,     5,     7,     6,     7,     3,     2,     2,     2,     3,
       1,     2,     1,     1,     4,     3,     3,     2
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   135,   105,   104,    88,    89,    90,    91,    92,    94,
      95,    96,    97,   100,   101,    98,    99,   131,   132,    93,
     109,   110,     0,     0,   142,   213,     0,    78,    80,   102,
       0,   103,    82,     0,   134,     0,     0,   210,   212,   126,
       0,     0,   146,   144,   143,    76,     0,    84,    86,    79,
      81,   108,     0,    83,     0,   192,     0,   217,     0,     0,
       0,   133,     1,   211,     0,   129,     0,   127,   136,   147,
     145,     0,    77,     0,   215,     0,     0,   115,     0,   111,
       0,   117,     3,     2,     4,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,    23,    24,    25,    26,    27,   196,   188,     6,    16,
      28,     0,    30,    34,    37,    40,    45,    48,    50,    52,
      54,    56,    58,    60,    73,     0,   194,   179,   180,     0,
       0,   181,   182,   183,   184,    86,   193,   216,   158,     0,
     141,   153,     0,   148,   150,     0,     2,   138,    28,    75,
       0,     0,     0,     0,   124,    85,     0,   174,    87,   214,
       0,   114,   107,   112,     0,     0,   118,   120,   116,     0,
       0,    20,     0,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,   206,   207,   208,     0,     0,   160,     0,
       0,    12,    13,     0,     0,     0,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    62,     0,    19,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   197,
     190,     0,   189,   195,   157,     0,     0,   152,   162,   154,
     163,   139,     0,   140,     0,   137,   125,   130,   128,   177,
       0,   106,   122,     0,   113,     0,     0,   185,     0,     0,
     187,     0,     0,     0,     0,     0,   205,   209,     5,     0,
     162,   161,     0,    11,     8,     0,    14,     0,    10,    61,
      31,    32,    33,    35,    36,    38,    39,    43,    44,    41,
      42,    46,    47,    49,    51,    53,    55,    57,     0,    74,
     191,   155,   156,   170,     0,     0,   166,     0,   164,     0,
       0,   149,   151,   159,     0,   175,   119,     0,   123,    21,
     186,     0,     0,     0,     0,     0,    29,     9,     0,     7,
       0,   171,   165,   167,   172,     0,   168,     0,   176,   178,
       0,   198,   200,   201,     0,     0,     0,    15,    59,   173,
     169,     0,     0,     0,   203,     0,     0,   199,   202,   204,
     121
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,   108,   109,   275,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   207,
     125,   150,    55,    56,    46,    47,    27,    28,    29,    30,
      78,    79,    80,   165,   166,    31,    66,    67,    32,    33,
      34,    35,    44,   304,   143,   144,   145,   189,   305,   240,
     158,   250,   126,   127,   128,    58,   130,   131,   132,   133,
     134,    36,    37,    38
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -210
static const yytype_int16 yypact[] =
{
    1069,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,
    -210,  -210,     7,     6,    53,  -210,    41,  1284,  1284,  -210,
      14,  -210,  1284,   710,   125,    51,   979,  -210,  -210,   -82,
       9,   -32,  -210,  -210,    53,  -210,    59,  -210,   690,  -210,
    -210,   -43,  1304,  -210,   287,  -210,    41,  -210,   710,  1157,
     774,   125,  -210,  -210,     9,   -28,   -25,  -210,  -210,  -210,
    -210,     6,  -210,   562,  -210,   710,  1304,  1304,   415,  -210,
      46,  1304,  -210,    -9,  -210,   881,   893,   893,   932,     2,
      42,    88,    95,   544,   122,   110,   132,   176,   579,   762,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,   146,
     356,   932,  -210,   124,   157,   214,    70,   225,   137,   156,
     177,   246,    12,  -210,  -210,    66,  -210,  -210,  -210,   360,
     433,  -210,  -210,  -210,  -210,   180,  -210,  -210,  -210,  1284,
    -210,    61,   200,   199,  -210,   -60,  -210,  -210,  -210,  -210,
     205,    32,   932,     9,  -210,  -210,   562,  -210,  -210,  -210,
     631,  -210,  -210,  -210,   932,    72,  -210,    -5,  -210,   544,
     762,  -210,   932,  -210,  -210,   192,   544,   932,   932,   932,
     221,   618,   193,  -210,  -210,  -210,    73,    44,   126,   215,
     275,  -210,  -210,   636,   932,   277,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,   932,  -210,   932,
     932,   932,   932,   932,   932,   932,   932,   932,   932,   932,
     932,   932,   932,   932,   932,   932,   932,   932,   932,  -210,
    -210,   471,  -210,  -210,    61,  1024,   808,  -210,    16,  -210,
     140,  -210,  1241,  -210,   279,  -210,  -210,  -210,  -210,  -210,
      33,  -210,  -210,    46,  -210,   220,   932,  -210,   222,   544,
    -210,    68,   133,   144,   224,   618,  -210,  -210,  -210,  1112,
     173,  -210,   932,  -210,  -210,   151,  -210,   185,  -210,  -210,
    -210,  -210,  -210,   124,   124,   157,   157,   214,   214,   214,
     214,    70,    70,   225,   137,   156,   177,   246,    62,  -210,
    -210,  -210,  -210,  -210,   230,   233,  -210,   218,   140,  1199,
     847,  -210,  -210,  -210,   506,  -210,  -210,   283,  -210,  -210,
    -210,   544,   544,   544,   932,   859,  -210,  -210,   932,  -210,
     932,  -210,  -210,  -210,  -210,   234,  -210,   235,  -210,  -210,
     231,   236,  -210,  -210,   153,   544,   154,  -210,  -210,  -210,
    -210,   300,   544,   219,  -210,   544,   240,  -210,  -210,  -210,
    -210
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -210,  -210,  -210,  -210,   -48,  -210,   -88,    43,    45,    31,
      50,    84,    85,    83,    86,    87,  -210,   -57,   -13,  -210,
     -91,   -53,    17,     0,  -210,   239,  -210,   -30,  -210,  -210,
     238,   -52,   -23,  -210,    96,  -210,   248,   183,   -10,    -7,
     -33,    -3,  -210,   -58,  -210,    98,  -210,   182,  -116,  -209,
    -132,  -210,   -78,  -210,   163,   139,   226,  -175,  -210,  -210,
    -210,  -210,   314,  -210
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
      26,   142,    61,   149,    64,   243,   265,   186,   187,   244,
       1,    39,   148,    65,    42,   180,    41,    25,    51,    48,
       1,    43,    77,   208,   249,   239,   163,    49,    50,   308,
     226,   149,    53,    68,    69,   175,    26,   171,   173,   174,
     148,    70,    81,    76,   153,     1,    77,    77,    77,   135,
       1,    77,   233,    25,   161,     1,   152,   255,   168,   141,
     157,   308,   154,   148,   135,     1,    81,    81,    81,    77,
      23,    81,   271,   167,   169,   136,   188,    24,   256,   187,
     235,   187,   236,   216,   217,   176,   261,   262,   263,    81,
     325,   257,   136,    40,   227,   149,    17,    18,   260,   247,
      52,   153,   314,   277,   148,    23,   177,   149,   163,   268,
      23,   252,    24,   228,   182,    23,   148,    24,   302,   246,
     315,   280,   281,   282,    24,   235,    45,   236,    71,   164,
      77,   228,    24,   321,   237,   228,   298,   228,   238,   234,
      77,   253,   228,   157,    72,   330,   136,   188,   218,   219,
      81,   229,   178,   233,   190,   191,   192,   254,   267,   179,
      81,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   149,
     276,   320,   339,   307,   326,   270,   181,    75,   148,    59,
     269,    60,   236,   129,   279,   209,    57,    24,   322,   149,
     210,   211,   228,   318,   309,    61,   310,   222,   148,   323,
     193,    74,   194,   228,   195,   299,   327,   183,   353,   355,
     328,   137,   228,   228,   148,   214,   215,   301,    41,   212,
     213,   238,   238,   344,   346,   141,   223,   269,   159,   236,
     220,   221,   141,   341,   342,   343,   167,   287,   288,   289,
     290,   335,   329,   149,   228,   283,   284,   337,   224,   285,
     286,   184,   148,   225,    73,   241,   270,   354,   242,   141,
     291,   292,   245,   348,   357,   259,   264,   359,   266,   273,
     272,   278,   148,   313,   317,   333,   340,   319,   324,   352,
      82,    83,    84,    85,     2,   331,    86,    87,   332,   349,
     351,   157,   350,   356,   358,   360,   293,   295,   294,   141,
     155,   296,   151,   297,   160,   347,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,   248,    88,    89,    90,
     312,    91,    92,    93,    94,    95,    96,    97,    98,   316,
      63,    99,   258,     0,     0,   231,     0,   100,   101,   102,
     103,   104,   105,    82,    83,    84,    85,     2,     0,    86,
      87,     0,   106,    54,   107,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,     0,     0,     0,     0,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,     0,
      88,    89,    90,     0,    91,    92,    93,    94,    95,    96,
      97,    98,     2,     0,    99,     0,     0,     0,     0,     0,
     100,   101,   102,   103,   104,   105,    82,    83,    84,    85,
     206,     0,    86,    87,     3,   106,    54,   230,     0,     0,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,    83,    84,    85,     0,     0,
      86,    87,     0,    88,    89,    90,     0,    91,    92,    93,
      94,    95,    96,    97,    98,     0,     0,    99,     0,     0,
       0,     0,   162,   100,   101,   102,   103,   104,   105,    82,
     146,    84,    85,     0,     0,    86,    87,     0,   106,    54,
     232,    88,    89,    90,     0,    91,    92,    93,    94,    95,
      96,    97,    98,     0,     0,    99,     0,     0,     0,     0,
       0,   100,   101,   102,   103,   104,   105,    82,    83,    84,
      85,     0,     0,    86,    87,     0,   106,    54,   300,     0,
       0,     0,     0,     0,     0,    82,   146,    84,    85,     0,
      99,    86,    87,     0,     0,     0,   100,   101,   102,   103,
     104,   105,    82,   146,    84,    85,     0,     0,    86,    87,
       0,     0,   156,   338,    88,    89,    90,     0,    91,    92,
      93,    94,    95,    96,    97,    98,     0,     0,    99,     0,
       0,     0,     0,     0,   100,   101,   102,   103,   104,   105,
       0,    82,   146,    84,    85,     0,    99,    86,    87,   106,
      54,     0,   100,   101,   102,   103,   104,   105,     2,    82,
     146,    84,    85,    99,     0,    86,    87,     0,   156,   100,
     101,   102,   103,   104,   105,     0,     0,     0,     0,     0,
       3,     0,     0,     0,   185,     0,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
       0,     0,    99,     0,     0,     0,     0,     0,   100,   101,
     102,   103,   104,   105,     0,     0,     0,     2,     0,     0,
      99,   274,     0,   106,     0,     0,   100,   101,   102,   103,
     104,   105,     0,     0,     0,     0,     0,     2,   251,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,     0,
       0,     0,     0,     0,     0,    82,   146,    84,    85,     2,
       0,    86,    87,     0,    73,     0,    54,    82,   146,    84,
      85,     0,     0,    86,    87,     0,     0,     0,     0,     0,
       0,     3,     0,     0,     0,     0,    54,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    82,   146,    84,    85,     0,     0,    86,    87,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,     0,
       0,     0,   100,   101,   102,   103,   104,   105,    99,     0,
       0,   147,     0,     0,   100,   101,   102,   103,   104,   105,
      82,   146,    84,    85,     0,     0,    86,    87,     0,     0,
       0,     0,    82,   146,    84,    85,     0,     0,    86,    87,
       0,     0,    99,     0,     0,   306,     0,     0,   100,   101,
     102,   103,   104,   105,    82,   146,    84,    85,     0,     0,
      86,    87,     0,     0,     0,     0,    82,   146,    84,    85,
       0,     0,    86,    87,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   336,     0,     0,   100,   101,   102,
     103,   104,   105,    99,   345,     0,     0,     0,     0,   100,
     101,   102,   103,   104,   105,    82,   146,    84,    85,     0,
       0,    86,    87,     0,     0,   170,     0,     0,     0,     0,
       0,   100,   101,   102,   103,   104,   105,   172,     0,     0,
       0,     0,     0,   100,   101,   102,   103,   104,   105,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     1,     0,     0,     2,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,     0,
       0,     0,   100,   101,   102,   103,   104,   105,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,     1,     0,
       0,     2,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,     0,     0,     0,     0,     0,     0,
      24,     0,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,     1,     0,     0,     2,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   139,   235,   303,
     236,     0,     0,     0,     0,    24,     0,     0,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,     0,     2,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,     0,     0,     0,     0,     0,     0,
      24,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,   138,     0,     0,     2,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   139,   269,   303,   236,     0,
       0,     0,     0,    24,     0,     0,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,     2,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     139,     0,   140,     0,     0,     0,     0,     0,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,     2,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   139,     0,   334,     0,     0,     0,     0,     0,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
     311,     2,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   139,     0,     0,     0,     0,     0,
       0,     2,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,     3,     0,     0,     0,     0,     0,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-210))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       0,    59,    35,    60,    86,    65,   181,    98,    99,    69,
       4,     4,    60,     4,    24,    93,    23,     0,     4,    26,
       4,    24,    52,   111,   156,   141,    78,    27,    28,   238,
      18,    88,    32,    65,    44,    88,    36,    85,    86,    87,
      88,    44,    52,    86,    69,     4,    76,    77,    78,    56,
       4,    81,   130,    36,    77,     4,    84,    62,    81,    59,
      73,   270,    87,   111,    71,     4,    76,    77,    78,    99,
      64,    81,   188,    80,    83,    58,    99,    71,    83,   170,
      64,   172,    66,    13,    14,    83,   177,   178,   179,    99,
     265,   169,    75,    86,    82,   152,    43,    44,   176,   152,
      86,    69,    69,   194,   152,    64,    64,   164,   160,    65,
      64,   164,    71,    69,     4,    64,   164,    71,   234,    87,
      87,   209,   210,   211,    71,    64,    85,    66,    69,    83,
     160,    69,    71,    65,   141,    69,   227,    69,   141,   139,
     170,    69,    69,   156,    85,    83,   129,   170,    78,    79,
     160,    85,    64,   231,     8,     9,    10,    85,    85,    64,
     170,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   236,
     193,   259,   314,   236,   272,   188,    64,    48,   236,    64,
      64,    66,    66,    54,   207,    71,    33,    71,    65,   256,
      76,    77,    69,   256,    64,   238,    66,    70,   256,    65,
      64,    48,    66,    69,    68,   228,    65,    85,    65,    65,
      69,    58,    69,    69,   272,    11,    12,   234,   235,    72,
      73,   234,   235,   324,   325,   235,    80,    64,    75,    66,
      15,    16,   242,   321,   322,   323,   253,   216,   217,   218,
     219,   309,    67,   310,    69,   212,   213,   310,    81,   214,
     215,    85,   310,    17,    84,    65,   269,   345,    69,   269,
     220,   221,    67,   330,   352,    83,    55,   355,    85,     4,
      65,     4,   330,     4,    64,    67,     3,    65,    64,    53,
       3,     4,     5,     6,     7,    65,     9,    10,    65,    65,
      69,   314,    67,     3,    85,    65,   222,   224,   223,   309,
      71,   225,    64,   226,    76,   328,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,   153,    50,    51,    52,
     242,    54,    55,    56,    57,    58,    59,    60,    61,   253,
      36,    64,   170,    -1,    -1,   129,    -1,    70,    71,    72,
      73,    74,    75,     3,     4,     5,     6,     7,    -1,     9,
      10,    -1,    85,    86,    87,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    -1,    -1,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    -1,
      50,    51,    52,    -1,    54,    55,    56,    57,    58,    59,
      60,    61,     7,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      70,    71,    72,    73,    74,    75,     3,     4,     5,     6,
      84,    -1,     9,    10,    29,    85,    86,    87,    -1,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
       9,    10,    -1,    50,    51,    52,    -1,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    87,    70,    71,    72,    73,    74,    75,     3,
       4,     5,     6,    -1,    -1,     9,    10,    -1,    85,    86,
      87,    50,    51,    52,    -1,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    70,    71,    72,    73,    74,    75,     3,     4,     5,
       6,    -1,    -1,     9,    10,    -1,    85,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      64,     9,    10,    -1,    -1,    -1,    70,    71,    72,    73,
      74,    75,     3,     4,     5,     6,    -1,    -1,     9,    10,
      -1,    -1,    86,    87,    50,    51,    52,    -1,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    70,    71,    72,    73,    74,    75,
      -1,     3,     4,     5,     6,    -1,    64,     9,    10,    85,
      86,    -1,    70,    71,    72,    73,    74,    75,     7,     3,
       4,     5,     6,    64,    -1,     9,    10,    -1,    86,    70,
      71,    72,    73,    74,    75,    -1,    -1,    -1,    -1,    -1,
      29,    -1,    -1,    -1,    85,    -1,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,    73,    74,    75,    -1,    -1,    -1,     7,    -1,    -1,
      64,    65,    -1,    85,    -1,    -1,    70,    71,    72,    73,
      74,    75,    -1,    -1,    -1,    -1,    -1,     7,    87,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,     9,    10,    -1,    84,    -1,    86,     3,     4,     5,
       6,    -1,    -1,     9,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    86,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,     3,     4,     5,     6,    -1,    -1,     9,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    73,    74,    75,    64,    -1,
      -1,    67,    -1,    -1,    70,    71,    72,    73,    74,    75,
       3,     4,     5,     6,    -1,    -1,     9,    10,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,     9,    10,
      -1,    -1,    64,    -1,    -1,    67,    -1,    -1,    70,    71,
      72,    73,    74,    75,     3,     4,     5,     6,    -1,    -1,
       9,    10,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    -1,    -1,    70,    71,    72,
      73,    74,    75,    64,    65,    -1,    -1,    -1,    -1,    70,
      71,    72,    73,    74,    75,     3,     4,     5,     6,    -1,
      -1,     9,    10,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    70,    71,    72,    73,    74,    75,    64,    -1,    -1,
      -1,    -1,    -1,    70,    71,    72,    73,    74,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,
      -1,    -1,    -1,     4,    -1,    -1,     7,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    73,    74,    75,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,     4,    -1,
      -1,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,     4,    -1,    -1,     7,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    -1,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,     4,    -1,    -1,     7,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,     7,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,     7,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,     7,    -1,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    29,    -1,    -1,    -1,    -1,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     7,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    64,    71,   110,   111,   114,   115,   116,
     117,   123,   126,   127,   128,   129,   149,   150,   151,     4,
      86,   127,   126,   129,   130,    85,   112,   113,   127,   111,
     111,     4,    86,   111,    86,   110,   111,   142,   143,    64,
      66,   128,     0,   150,    86,     4,   124,   125,    65,   126,
     129,    69,    85,    84,   142,   143,    86,   115,   118,   119,
     120,   126,     3,     4,     5,     6,     9,    10,    50,    51,
      52,    54,    55,    56,    57,    58,    59,    60,    61,    64,
      70,    71,    72,    73,    74,    75,    85,    87,    89,    90,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   108,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   127,   110,   142,     4,    63,
      65,   111,   131,   132,   133,   134,     4,    67,    92,   105,
     109,   124,    84,    69,    87,   113,    86,   106,   138,   142,
     118,   120,    87,   119,    83,   121,   122,   127,   120,    83,
      64,    92,    64,    92,    92,   109,    83,    64,    64,    64,
     140,    64,     4,    85,    85,    85,   108,   108,   120,   135,
       8,     9,    10,    64,    66,    68,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    84,   107,    94,    71,
      76,    77,    72,    73,    11,    12,    13,    14,    78,    79,
      15,    16,    70,    80,    81,    17,    18,    82,    69,    85,
      87,   144,    87,   140,   111,    64,    66,   127,   129,   136,
     137,    65,    69,    65,    69,    67,    87,   109,   125,   138,
     139,    87,   109,    69,    85,    62,    83,   140,   135,    83,
     140,   108,   108,   108,    55,   145,    85,    85,    65,    64,
     129,   136,    65,     4,    65,    91,   106,   108,     4,   106,
      94,    94,    94,    95,    95,    96,    96,    97,    97,    97,
      97,    98,    98,    99,   100,   101,   102,   103,   108,   106,
      87,   127,   136,    65,   131,   136,    67,   109,   137,    64,
      66,    49,   133,     4,    69,    87,   122,    64,   109,    65,
     140,    65,    65,    65,    64,   145,    94,    65,    69,    67,
      83,    65,    65,    67,    65,   131,    67,   109,    87,   138,
       3,   140,   140,   140,   108,    65,   108,   106,   105,    65,
      67,    69,    53,    65,   140,    65,     3,   140,    85,   140,
      65
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:

/* Line 1806 of yacc.c  */
#line 66 "c_grammer.y"
    {
		array_len = (yyvsp[(1) - (1)].val);
	}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 205 "c_grammer.y"
    {
		//printf("111\n");
		write_struct_info();
	}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 210 "c_grammer.y"
    {
		//printf("222\n");
		write_func_info((yyvsp[(1) - (3)].name));
		write_struct_info();
		write_newtype_info();
	}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 220 "c_grammer.y"
    {
	}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 223 "c_grammer.y"
    {
		is_typedef = 1;
	}
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 227 "c_grammer.y"
    {
	}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 230 "c_grammer.y"
    {
		char last_type[sizeof(g_type)] = {0};
		strcpy(last_type, g_type);
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, (yyval.name));
		strcat(g_type, " ");
		strcat(g_type, last_type);

		if ((yyval.name)) free((yyval.name));
		(yyval.name) = strdup(g_type);
	}
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 242 "c_grammer.y"
    {
	}
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 245 "c_grammer.y"
    {
	}
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 269 "c_grammer.y"
    {	
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, (yyvsp[(1) - (1)].name));
	}
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 274 "c_grammer.y"
    {
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, (yyvsp[(1) - (1)].name));
	}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 279 "c_grammer.y"
    {
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, (yyvsp[(1) - (1)].name));
	}
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 284 "c_grammer.y"
    {
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, (yyvsp[(1) - (1)].name));
	}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 289 "c_grammer.y"
    {
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, (yyvsp[(1) - (1)].name));
	}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 294 "c_grammer.y"
    {
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, (yyvsp[(1) - (1)].name));
	}
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 299 "c_grammer.y"
    {
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, (yyvsp[(1) - (1)].name));
	}
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 304 "c_grammer.y"
    {
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, (yyvsp[(1) - (1)].name));
	}
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 309 "c_grammer.y"
    {
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, (yyvsp[(1) - (1)].name));
	}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 314 "c_grammer.y"
    {
		if ((yyval.name)) free((yyval.name));
		(yyval.name) = strdup(g_type);
	}
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 319 "c_grammer.y"
    {
	}
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 323 "c_grammer.y"
    {
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, (yyvsp[(1) - (1)].name));
	}
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 331 "c_grammer.y"
    {
		strcpy(g_type, (yyvsp[(1) - (5)].name));
		strcat(g_type, " ");
		strcat(g_type, (yyvsp[(2) - (5)].name));

		strcpy(s_newtype.orig_name, (yyvsp[(2) - (5)].name));

		strcpy(s_struct.type, (yyvsp[(1) - (5)].name));
		strcat(s_struct.type, " ");
		strcat(s_struct.type, (yyvsp[(2) - (5)].name));
		strcpy(s_struct.name, (yyvsp[(2) - (5)].name));

		is_struct_def = 1;
	}
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 346 "c_grammer.y"
    {
		strcpy(g_type, (yyvsp[(1) - (4)].name));
		is_struct_def = 1;
	}
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 351 "c_grammer.y"
    {
		strcpy(g_type, (yyvsp[(1) - (2)].name));
		strcat(g_type, " ");
		strcat(g_type, (yyvsp[(2) - (2)].name));
	}
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 370 "c_grammer.y"
    {
		//printf("%s\n", $1);
		list_write_data(&s_struct.para_list, (unsigned char *)&s_para, sizeof(s_para), 0);
		memset(&s_para, 0, sizeof(s_para));
	}
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 379 "c_grammer.y"
    {
		char last_type[sizeof(g_type)] = {0};
		strcpy(last_type, g_type);
		memset(g_type, 0, sizeof(g_type));
		strcat(g_type, (yyval.name));
		strcat(g_type, " ");
		strcat(g_type, last_type);
	}
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 388 "c_grammer.y"
    {
	}
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 391 "c_grammer.y"
    {
	}
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 394 "c_grammer.y"
    {
	}
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 405 "c_grammer.y"
    {
		trace("no mark\n");
		return -1;
	}
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 410 "c_grammer.y"
    {
		//memset(&s_para, 0, sizeof(s_para));
		strcpy(s_para.name, g_name);
		strcpy(s_para.type, g_type);
		s_para.len = array_len;
		s_para.string = (yyvsp[(6) - (7)].val);
		s_para.field_num = (yyvsp[(4) - (7)].val);
		s_para.field_type = gen_field_type(&s_para);
		
		memset(g_name, 0, sizeof(g_name));
		memset(g_type, 0, sizeof(g_type));
		array_len = 1;
	}
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 450 "c_grammer.y"
    {
		if (is_func) {
			s_func.pointer = 1;
		} else {
			s_para.pointer = 1;
		}
	}
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 462 "c_grammer.y"
    {
		strcpy(g_name, (yyvsp[(1) - (1)].name));
	}
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 466 "c_grammer.y"
    {
	}
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 469 "c_grammer.y"
    {
		array_len = -1;
	}
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 474 "c_grammer.y"
    {
		strcat(s_func.func_name, (yyvsp[(1) - (4)].name));
		is_func = 1;
	}
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 480 "c_grammer.y"
    {
/*		strcat(s_func.func_name, $1);*/
/*		is_func = 1;*/
		trace("func must have a para with type [struct rmi *]\n");
		return -1;
	}
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 513 "c_grammer.y"
    {
		if (0 != check_func_first_para()) {
			return -1;
		}
	}
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 519 "c_grammer.y"
    {
		if (0 != check_func_first_para()) {
			return -1;
		}
	}
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 525 "c_grammer.y"
    {
		if (0 != check_func_first_para()) {
			return -1;
		}
	}
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 531 "c_grammer.y"
    {
		if (0 != write_func_para(g_name, (yyvsp[(1) - (3)].name))) {
			return -1;
		}
	}
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 537 "c_grammer.y"
    {
		char name[16] = {0};
		sprintf(name, "para%d", list_size(&s_func.para_list));
		if (0 != write_func_para(name, (yyvsp[(1) - (3)].name))) {
			return -1;
		}
	}
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 545 "c_grammer.y"
    {		
		char name[16] = {0};
		sprintf(name, "para%d", list_size(&s_func.para_list));
		if (0 != write_func_para(name, (yyvsp[(1) - (2)].name))) {
			return -1;
		}
	}
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 566 "c_grammer.y"
    {
		s_para.pointer = 1;
	}
    break;



/* Line 1806 of yacc.c  */
#line 2489 "c_grammer.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 671 "c_grammer.y"

#include <stdio.h>

extern char yytext[];

void yyerror(s)
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
	memset(&s_newtype, 0, sizeof(s_newtype));
	list_init(&s_struct.para_list, BUF_SIZE, LIST_MAX_TIME, LIST_MAX_PACKET, LIST_MAX_USER);
	memset(&s_func, 0, sizeof(s_func));
	list_init(&s_func.para_list, BUF_SIZE, LIST_MAX_TIME, LIST_MAX_PACKET, LIST_MAX_USER);
	array_len = 1;
}

int write_func_info(char * ret_type) {
	if (is_func) {
		if (s_func.pointer || !(!strcmp(ret_type, "int") || !strcmp(ret_type, "void"))) {
			trace("return type error\n");
			return -1;
		}
		strcat(s_func.ret_type, ret_type);
		s_func.func_id = crc32(s_func.func_name, strlen(s_func.func_name));
		list_write_data(&g_func_list, (unsigned char *)&s_func, sizeof(s_func), 0);
		
		memset(&s_func, 0, sizeof(s_func));
		list_init(&s_func.para_list, BUF_SIZE, LIST_MAX_TIME, LIST_MAX_PACKET, LIST_MAX_USER);
	}
	is_func = 0;
	is_first_para = 1;

	return 0;
}

void write_struct_info() {
	if (is_struct_def) {
		if (0 == s_struct.name[0]) {
			strcpy(s_struct.name, g_name);
			strcpy(s_struct.type, g_name);
		}
		list_write_data(&g_struct_list, (unsigned char *)&s_struct, sizeof(s_struct), 0);
		
		memset(&s_struct, 0, sizeof(s_struct));
		list_init(&s_struct.para_list, BUF_SIZE, LIST_MAX_TIME, LIST_MAX_PACKET, LIST_MAX_USER);
	}
	is_struct_def = 0;
}

void write_newtype_info() {		
	if (is_typedef) {
		strcpy(s_newtype.new_type, g_name);
		strcpy(s_newtype.orig_type, g_type);

		list_write_data(&g_newtype_list, (unsigned char *)&s_newtype, sizeof(s_newtype), 0);
	}
	memset(&s_newtype, 0, sizeof(s_newtype));
	is_typedef = 0;
}

int write_func_para(char * name, char * dir) {	
	if (is_first_para) {
		trace("func first para do not need dir\n");
		return -1;
	}
	//printf("is_func: %d, pointer: %d\n", is_func, s_para.pointer);
	//memset(&s_para, 0, sizeof(s_para));
	strcpy(s_para.name, name);
	strcpy(s_para.type, g_type);
	s_para.len = array_len;
	if (0 == memcmp(dir, "_IN", strlen("_IN"))) {
		s_para.dir = PARA_IN;
	} else if (0 == memcmp(dir, "_OUT", strlen("_OUT"))) {
		s_para.dir = PARA_OUT;
	}
	if (PARA_OUT == s_para.dir && 1 != s_para.pointer) {
		trace("out para is not pointer!\n");
		return -1;
	}
	s_para.field_type = gen_field_type(&s_para);
	s_para.para_num = list_size(&s_func.para_list);
	
	list_write_data(&s_func.para_list, (unsigned char *)&s_para, sizeof(s_para), 0);
	memset(&s_para, 0, sizeof(s_para));
	
	memset(g_name, 0, sizeof(g_name));
	memset(g_type, 0, sizeof(g_type));
	array_len = 1;

	return 0;
}

int check_func_first_para() {
/*	trace("g_type: %s\n", g_type);*/
	if (is_first_para) {
		is_first_para = 0;
		if (0 != strcmp(g_type, "struct rmi") || !s_para.pointer) {
			trace("first para type of func must be [struct rmi *]\n");
			return -1;
		} else {
			memset(&s_para, 0, sizeof(s_para));
		}
	} else {
		if (0 != g_name[0]) {
			trace("para[%s] has no dir\n", g_name);
		} else {
			trace("para has no dir\n");
		}
		return -1;
	}
	
	return 0;
}


