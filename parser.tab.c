/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NAME = 258,
     STRING = 259,
     COMP = 260,
     INTNUM = 261,
     FLOATNUM = 262,
     CREATE = 263,
     TABLE = 264,
     PRIMARY = 265,
     KEY = 266,
     INDEX = 267,
     INT = 268,
     CHAR = 269,
     FLOAT = 270,
     UNIQUE = 271,
     NOT = 272,
     NULLX = 273,
     ON = 274,
     AND = 275,
     DROP = 276,
     QUIT = 277,
     EXEFILE = 278,
     TRANSACTION = 279,
     SELECT = 280,
     FROM = 281,
     WHERE = 282,
     DELETE = 283,
     ROLLBACK = 284,
     COMMIT = 285,
     INSERT = 286,
     INTO = 287,
     VALUES = 288
   };
#endif
/* Tokens.  */
#define NAME 258
#define STRING 259
#define COMP 260
#define INTNUM 261
#define FLOATNUM 262
#define CREATE 263
#define TABLE 264
#define PRIMARY 265
#define KEY 266
#define INDEX 267
#define INT 268
#define CHAR 269
#define FLOAT 270
#define UNIQUE 271
#define NOT 272
#define NULLX 273
#define ON 274
#define AND 275
#define DROP 276
#define QUIT 277
#define EXEFILE 278
#define TRANSACTION 279
#define SELECT 280
#define FROM 281
#define WHERE 282
#define DELETE 283
#define ROLLBACK 284
#define COMMIT 285
#define INSERT 286
#define INTO 287
#define VALUES 288




/* Copy the first part of user declarations.  */
#line 1 "src/parser/parser.y"


extern "C" int yylex();

int yyerror(const char *);
extern int yylineno;

#include "src/parser/stmt.h"
#include <iostream>
#include <cstring>
#include <algorithm>

// extern function and enum stmt type

extern void xyzsql_emit_stmt(stmt_type, statement *);

using namespace std;



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

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 21 "src/parser/parser.y"
{
    char *strval;
    int intval;
    float floatval;
    table_column *colval;
    vector<table_column *> *collist;
    attribute *attrval;
    vector<attribute *> *attrlist;
    vector<string *> *namelist;
    vector<record_value> *valuelist;
    condition *condval;
    vector<condition *> *condlist;
    record_value *valueval;
    
    // stmt types
    insert_stmt *insert_stmt_val;
    create_table_stmt *create_table_stmt_val;
    create_index_stmt *create_index_stmt_val;
    select_stmt *select_stmt_val;
    drop_index_stmt *drop_index_stmt_val;
    drop_table_stmt *drop_table_stmt_val;
    delete_stmt *delete_stmt_val;
    exefile_stmt *exefile_stmt_val;
}
/* Line 193 of yacc.c.  */
#line 207 "parser.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 220 "parser.tab.c"

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
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  36
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   102

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  62
/* YYNRULES -- Number of states.  */
#define YYNSTATES  114

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   288

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      35,    36,    39,     2,    37,     2,    38,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    34,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,    10,    12,    14,    16,    18,    20,
      22,    24,    26,    28,    30,    32,    34,    41,    43,    45,
      47,    48,    51,    53,    56,    58,    59,    63,    68,    74,
      79,    80,    82,    86,    95,    99,   103,   107,   109,   111,
     115,   117,   121,   123,   127,   131,   135,   139,   143,   148,
     155,   159,   165,   167,   169,   171,   173,   177,   185,   188,
     190,   192,   194
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      41,     0,    -1,    42,    34,    -1,    42,    34,    41,    -1,
      43,    -1,    57,    -1,    49,    -1,    61,    -1,    51,    -1,
      50,    -1,    58,    -1,    66,    -1,    65,    -1,    62,    -1,
      63,    -1,    64,    -1,     8,     9,     3,    35,    48,    36,
      -1,    13,    -1,    14,    -1,    15,    -1,    -1,    10,    11,
      -1,    12,    -1,    17,    18,    -1,    16,    -1,    -1,    35,
       6,    36,    -1,     3,    44,    46,    45,    -1,    10,    11,
      35,     3,    36,    -1,    12,    35,     3,    36,    -1,    -1,
      47,    -1,    47,    37,    48,    -1,     8,    12,     3,    19,
       3,    35,     3,    36,    -1,    21,     9,     3,    -1,    21,
      12,     3,    -1,     3,    38,     3,    -1,    39,    -1,    52,
      -1,    52,    37,    53,    -1,     3,    -1,     3,    37,    54,
      -1,    56,    -1,    56,    20,    55,    -1,    52,     5,    52,
      -1,    52,     5,     4,    -1,    52,     5,     6,    -1,    52,
       5,     7,    -1,    25,    53,    26,     3,    -1,    25,    53,
      26,    54,    27,    55,    -1,    28,    26,     3,    -1,    28,
      26,     3,    27,    55,    -1,     6,    -1,     7,    -1,     4,
      -1,    59,    -1,    59,    37,    60,    -1,    31,    32,     3,
      33,    35,    60,    36,    -1,    24,    19,    -1,    30,    -1,
      29,    -1,    22,    -1,    23,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    75,    75,    76,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    95,    98,    99,   100,
     103,   104,   105,   106,   107,   110,   111,   113,   114,   115,
     118,   119,   120,   124,   129,   134,   139,   142,   143,   144,
     147,   148,   151,   152,   154,   155,   156,   157,   161,   162,
     167,   168,   173,   174,   175,   178,   179,   182,   186,   189,
     192,   197,   200
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NAME", "STRING", "COMP", "INTNUM",
  "FLOATNUM", "CREATE", "TABLE", "PRIMARY", "KEY", "INDEX", "INT", "CHAR",
  "FLOAT", "UNIQUE", "NOT", "NULLX", "ON", "AND", "DROP", "QUIT",
  "EXEFILE", "TRANSACTION", "SELECT", "FROM", "WHERE", "DELETE",
  "ROLLBACK", "COMMIT", "INSERT", "INTO", "VALUES", "';'", "'('", "')'",
  "','", "'.'", "'*'", "$accept", "stmt_list", "stmt", "create_table_stmt",
  "data_type", "column_atts", "opt_length", "create_def",
  "create_col_list", "create_index_stmt", "drop_table_stmt",
  "drop_index_stmt", "attribute", "select_list", "from_list", "conditions",
  "condition", "select_stmt", "delete_stmt", "value", "value_list",
  "insert_stmt", "transaction_on", "commit", "rollback", "quit", "exefile", 0
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
     285,   286,   287,   288,    59,    40,    41,    44,    46,    42
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    40,    41,    41,    42,    42,    42,    42,    42,    42,
      42,    42,    42,    42,    42,    42,    43,    44,    44,    44,
      45,    45,    45,    45,    45,    46,    46,    47,    47,    47,
      48,    48,    48,    49,    50,    51,    52,    53,    53,    53,
      54,    54,    55,    55,    56,    56,    56,    56,    57,    57,
      58,    58,    59,    59,    59,    60,    60,    61,    62,    63,
      64,    65,    66
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     6,     1,     1,     1,
       0,     2,     1,     2,     1,     0,     3,     4,     5,     4,
       0,     1,     3,     8,     3,     3,     3,     1,     1,     3,
       1,     3,     1,     3,     3,     3,     3,     3,     4,     6,
       3,     5,     1,     1,     1,     1,     3,     7,     2,     1,
       1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,    61,    62,     0,     0,     0,    60,    59,
       0,     0,     0,     4,     6,     9,     8,     5,    10,     7,
      13,    14,    15,    12,    11,     0,     0,     0,     0,    58,
       0,    37,    38,     0,     0,     0,     1,     2,     0,     0,
      34,    35,     0,     0,     0,    50,     0,     3,    30,     0,
      36,    39,    40,     0,     0,     0,     0,     0,     0,    31,
       0,     0,     0,     0,     0,    51,    42,     0,    17,    18,
      19,    25,     0,     0,    30,    16,     0,    40,    41,    49,
       0,     0,    54,    52,    53,    55,     0,     0,    20,     0,
       0,    32,     0,    45,    46,    47,    44,    43,     0,    57,
       0,     0,    22,    24,     0,    27,     0,    29,    33,    56,
      26,    21,    23,    28
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    11,    12,    13,    71,   105,    88,    59,    60,    14,
      15,    16,    64,    33,    53,    65,    66,    17,    18,    85,
      86,    19,    20,    21,    22,    23,    24
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -57
static const yytype_int8 yypact[] =
{
      -7,    22,    27,   -57,   -57,   -15,    -1,   -13,   -57,   -57,
     -21,    19,   -14,   -57,   -57,   -57,   -57,   -57,   -57,   -57,
     -57,   -57,   -57,   -57,   -57,    32,    38,    39,    44,   -57,
      10,   -57,    12,    24,    48,    49,   -57,    -7,    18,    35,
     -57,   -57,    52,    -1,    53,    30,    25,   -57,     0,    56,
     -57,   -57,     6,    33,    58,    28,    31,    51,    29,    34,
      36,    40,    62,    58,    61,   -57,    47,    23,   -57,   -57,
     -57,    41,    42,    65,     0,   -57,    66,    43,   -57,   -57,
       2,    58,   -57,   -57,   -57,    45,    37,    64,    16,    75,
      50,   -57,    54,   -57,   -57,   -57,   -57,   -57,    23,   -57,
      55,    68,   -57,   -57,    63,   -57,    57,   -57,   -57,   -57,
     -57,   -57,   -57,   -57
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -57,    46,   -57,   -57,   -57,   -57,   -57,   -57,    11,   -57,
     -57,   -57,    -6,    59,    26,   -56,   -57,   -57,   -57,   -57,
     -11,   -57,   -57,   -57,   -57,   -57,   -57
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -49
static const yytype_int8 yytable[] =
{
      32,     1,    30,    56,    29,    30,    93,    79,    94,    95,
      57,    35,    58,    34,     2,     3,     4,     5,     6,    36,
      37,     7,     8,     9,    10,    97,   101,    82,   102,    83,
      84,    25,   103,   104,    26,    38,    27,    32,    31,    28,
     -48,    39,    40,    62,    68,    69,    70,    41,    42,    43,
      44,    45,    46,    48,    49,    50,    52,    54,    55,    61,
      63,    30,    72,    67,    73,    77,    80,    81,    90,    92,
     100,    74,    75,    99,    96,    76,    87,    89,   106,   111,
      62,   112,    98,    47,     0,    91,   107,   109,    78,     0,
     108,   110,     0,   113,     0,     0,     0,     0,     0,     0,
       0,     0,    51
};

static const yytype_int8 yycheck[] =
{
       6,     8,     3,     3,    19,     3,     4,    63,     6,     7,
      10,    32,    12,    26,    21,    22,    23,    24,    25,     0,
      34,    28,    29,    30,    31,    81,    10,     4,    12,     6,
       7,     9,    16,    17,    12,     3,     9,    43,    39,    12,
      34,     3,     3,    37,    13,    14,    15,     3,    38,    37,
      26,     3,     3,    35,    19,     3,     3,    27,    33,     3,
      27,     3,    11,    35,    35,     3,     5,    20,     3,     3,
       6,    37,    36,    36,    80,    35,    35,    35,     3,    11,
      37,    18,    37,    37,    -1,    74,    36,    98,    62,    -1,
      36,    36,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    43
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     8,    21,    22,    23,    24,    25,    28,    29,    30,
      31,    41,    42,    43,    49,    50,    51,    57,    58,    61,
      62,    63,    64,    65,    66,     9,    12,     9,    12,    19,
       3,    39,    52,    53,    26,    32,     0,    34,     3,     3,
       3,     3,    38,    37,    26,     3,     3,    41,    35,    19,
       3,    53,     3,    54,    27,    33,     3,    10,    12,    47,
      48,     3,    37,    27,    52,    55,    56,    35,    13,    14,
      15,    44,    11,    35,    37,    36,    35,     3,    54,    55,
       5,    20,     4,     6,     7,    59,    60,    35,    46,    35,
       3,    48,     3,     4,     6,     7,    52,    55,    37,    36,
       6,    10,    12,    16,    17,    45,     3,    36,    36,    60,
      36,    11,    18,    36
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
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
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
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
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
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
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



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
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
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
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
        case 2:
#line 75 "src/parser/parser.y"
    {  ;}
    break;

  case 3:
#line 76 "src/parser/parser.y"
    { cout << "Got a create stmt!" << endl; ;}
    break;

  case 4:
#line 79 "src/parser/parser.y"
    { xyzsql_emit_stmt(stmt_type::_create_table_stmt,   (yyvsp[(1) - (1)].create_table_stmt_val)); ;}
    break;

  case 5:
#line 80 "src/parser/parser.y"
    { xyzsql_emit_stmt(stmt_type::_select_stmt,         (yyvsp[(1) - (1)].select_stmt_val)); ;}
    break;

  case 6:
#line 81 "src/parser/parser.y"
    { xyzsql_emit_stmt(stmt_type::_create_index_stmt,   (yyvsp[(1) - (1)].create_index_stmt_val)); ;}
    break;

  case 7:
#line 82 "src/parser/parser.y"
    { xyzsql_emit_stmt(stmt_type::_insert_stmt,         (yyvsp[(1) - (1)].insert_stmt_val)); ;}
    break;

  case 8:
#line 83 "src/parser/parser.y"
    { xyzsql_emit_stmt(stmt_type::_drop_index_stmt,     (yyvsp[(1) - (1)].drop_index_stmt_val)); ;}
    break;

  case 9:
#line 84 "src/parser/parser.y"
    { xyzsql_emit_stmt(stmt_type::_drop_table_stmt,     (yyvsp[(1) - (1)].drop_table_stmt_val)); ;}
    break;

  case 10:
#line 85 "src/parser/parser.y"
    { xyzsql_emit_stmt(stmt_type::_delete_stmt,         (yyvsp[(1) - (1)].delete_stmt_val)); ;}
    break;

  case 11:
#line 86 "src/parser/parser.y"
    { xyzsql_emit_stmt(stmt_type::_exefile_stmt,        (yyvsp[(1) - (1)].exefile_stmt_val)); ;}
    break;

  case 12:
#line 87 "src/parser/parser.y"
    { xyzsql_emit_stmt(stmt_type::_quit_stmt,           NULL); ;}
    break;

  case 13:
#line 88 "src/parser/parser.y"
    { xyzsql_emit_stmt(stmt_type::_transaction_stmt,    NULL); ;}
    break;

  case 14:
#line 89 "src/parser/parser.y"
    { xyzsql_emit_stmt(stmt_type::_commit_stmt,         NULL); ;}
    break;

  case 15:
#line 90 "src/parser/parser.y"
    { xyzsql_emit_stmt(stmt_type::_rollback_stmt,       NULL); ;}
    break;

  case 16:
#line 95 "src/parser/parser.y"
    { (yyval.create_table_stmt_val) = new create_table_stmt((yyvsp[(3) - (6)].strval), (yyvsp[(5) - (6)].collist)); ;}
    break;

  case 17:
#line 98 "src/parser/parser.y"
    { (yyval.intval) = table_column::INTTYPE; cout << "INT" << endl; ;}
    break;

  case 18:
#line 99 "src/parser/parser.y"
    { (yyval.intval) = table_column::CHARTYPE; cout << "CHAR" << endl; ;}
    break;

  case 19:
#line 100 "src/parser/parser.y"
    { (yyval.intval) = table_column::FLOATTYPE; cout << "FLOAT" << endl; ;}
    break;

  case 20:
#line 103 "src/parser/parser.y"
    { (yyval.intval) = 0; cout << "No Attribute." << endl; ;}
    break;

  case 21:
#line 104 "src/parser/parser.y"
    { (yyval.intval) = table_column::primary_attr; cout << "PRIMARY" << endl; ;}
    break;

  case 22:
#line 105 "src/parser/parser.y"
    { (yyval.intval) = table_column::index_attr; cout << "INDEX" << endl; ;}
    break;

  case 23:
#line 106 "src/parser/parser.y"
    { (yyval.intval) = table_column::notnull_attr; cout << "NOT NULL" << endl; ;}
    break;

  case 24:
#line 107 "src/parser/parser.y"
    { (yyval.intval) = table_column::unique_attr; cout << "UNIQUE" << endl; ;}
    break;

  case 25:
#line 110 "src/parser/parser.y"
    { (yyval.intval) = 4; ;}
    break;

  case 26:
#line 111 "src/parser/parser.y"
    { (yyval.intval) = (yyvsp[(2) - (3)].intval); ;}
    break;

  case 27:
#line 113 "src/parser/parser.y"
    { (yyval.colval) = new table_column((yyvsp[(1) - (4)].strval), (yyvsp[(2) - (4)].intval), (yyvsp[(3) - (4)].intval), (yyvsp[(4) - (4)].intval)); cout << "This column is: " << (yyvsp[(1) - (4)].strval) << " " << (yyvsp[(2) - (4)].intval) << " " << (yyvsp[(3) - (4)].intval) << endl; ;}
    break;

  case 28:
#line 114 "src/parser/parser.y"
    {;}
    break;

  case 29:
#line 115 "src/parser/parser.y"
    {;}
    break;

  case 30:
#line 118 "src/parser/parser.y"
    {  ;}
    break;

  case 31:
#line 119 "src/parser/parser.y"
    { (yyval.collist) = new vector<table_column *>(); (yyval.collist)->push_back((yyvsp[(1) - (1)].colval));;}
    break;

  case 32:
#line 120 "src/parser/parser.y"
    { (yyvsp[(3) - (3)].collist)->push_back((yyvsp[(1) - (3)].colval)); (yyval.collist) = (yyvsp[(3) - (3)].collist); ;}
    break;

  case 33:
#line 124 "src/parser/parser.y"
    { (yyval.create_index_stmt_val) = new create_index_stmt((yyvsp[(3) - (8)].strval), (yyvsp[(5) - (8)].strval), (yyvsp[(7) - (8)].strval)); ;}
    break;

  case 34:
#line 129 "src/parser/parser.y"
    { (yyval.drop_table_stmt_val) = new drop_table_stmt((yyvsp[(3) - (3)].strval)); ;}
    break;

  case 35:
#line 134 "src/parser/parser.y"
    { (yyval.drop_index_stmt_val) = new drop_index_stmt((yyvsp[(3) - (3)].strval)); ;}
    break;

  case 36:
#line 139 "src/parser/parser.y"
    { (yyval.attrval) = new attribute((yyvsp[(1) - (3)].strval), (yyvsp[(3) - (3)].strval)); cout << "Attribute!" << endl; ;}
    break;

  case 37:
#line 142 "src/parser/parser.y"
    { (yyval.attrlist) = new vector<attribute *>(); (yyval.attrlist)->push_back(new attribute("*", "*"));;}
    break;

  case 38:
#line 143 "src/parser/parser.y"
    { (yyval.attrlist) = new vector<attribute *>(); (yyval.attrlist)->push_back((yyvsp[(1) - (1)].attrval));;}
    break;

  case 39:
#line 144 "src/parser/parser.y"
    { (yyval.attrlist) = (yyvsp[(3) - (3)].attrlist); (yyval.attrlist)->push_back((yyvsp[(1) - (3)].attrval)); ;}
    break;

  case 40:
#line 147 "src/parser/parser.y"
    { (yyval.namelist) = new vector<string *>(); (yyval.namelist)->push_back(new string((yyvsp[(1) - (1)].strval))); ;}
    break;

  case 41:
#line 148 "src/parser/parser.y"
    { (yyval.namelist) = (yyvsp[(3) - (3)].namelist); (yyval.namelist)->push_back(new string((yyvsp[(1) - (3)].strval))); ;}
    break;

  case 42:
#line 151 "src/parser/parser.y"
    { (yyval.condlist) = new vector<condition *>(); (yyval.condlist)->push_back((yyvsp[(1) - (1)].condval)); ;}
    break;

  case 43:
#line 152 "src/parser/parser.y"
    { (yyval.condlist) = (yyvsp[(3) - (3)].condlist); (yyval.condlist)->push_back((yyvsp[(1) - (3)].condval)); ;}
    break;

  case 44:
#line 154 "src/parser/parser.y"
    { (yyval.condval) = new condition((yyvsp[(1) - (3)].attrval), (yyvsp[(3) - (3)].attrval), (yyvsp[(2) - (3)].intval)); ;}
    break;

  case 45:
#line 155 "src/parser/parser.y"
    { (yyval.condval) = new condition((yyvsp[(1) - (3)].attrval), (yyvsp[(3) - (3)].strval), (yyvsp[(2) - (3)].intval)); ;}
    break;

  case 46:
#line 156 "src/parser/parser.y"
    { (yyval.condval) = new condition((yyvsp[(1) - (3)].attrval), (yyvsp[(3) - (3)].intval), (yyvsp[(2) - (3)].intval)); ;}
    break;

  case 47:
#line 157 "src/parser/parser.y"
    { (yyval.condval) = new condition((yyvsp[(1) - (3)].attrval), (yyvsp[(3) - (3)].floatval), (yyvsp[(2) - (3)].intval)); ;}
    break;

  case 48:
#line 161 "src/parser/parser.y"
    { (yyval.select_stmt_val) = new select_stmt((yyvsp[(2) - (4)].attrlist), new vector<string *>(1, new string((yyvsp[(4) - (4)].strval))), new vector<condition *>()); ;}
    break;

  case 49:
#line 162 "src/parser/parser.y"
    { (yyval.select_stmt_val) = new select_stmt((yyvsp[(2) - (6)].attrlist), (yyvsp[(4) - (6)].namelist), (yyvsp[(6) - (6)].condlist)); ;}
    break;

  case 50:
#line 167 "src/parser/parser.y"
    { (yyval.delete_stmt_val) = new delete_stmt((yyvsp[(3) - (3)].strval), new vector<condition *>); ;}
    break;

  case 51:
#line 168 "src/parser/parser.y"
    { (yyval.delete_stmt_val) = new delete_stmt((yyvsp[(3) - (5)].strval), (yyvsp[(5) - (5)].condlist)); ;}
    break;

  case 52:
#line 173 "src/parser/parser.y"
    { (yyval.valueval) = new record_value((yyvsp[(1) - (1)].intval)); cout << "int: " << (yyvsp[(1) - (1)].intval) << " "; ;}
    break;

  case 53:
#line 174 "src/parser/parser.y"
    { (yyval.valueval) = new record_value((yyvsp[(1) - (1)].floatval)); cout << "float: " << (yyvsp[(1) - (1)].floatval) << " "; ;}
    break;

  case 54:
#line 175 "src/parser/parser.y"
    { (yyval.valueval) = new record_value((yyvsp[(1) - (1)].strval)); cout << "string: " << (yyvsp[(1) - (1)].strval) << " "; ;}
    break;

  case 55:
#line 178 "src/parser/parser.y"
    { (yyval.valuelist) = new vector<record_value>; (yyval.valuelist)->push_back(*((yyvsp[(1) - (1)].valueval))); delete (yyvsp[(1) - (1)].valueval);;}
    break;

  case 56:
#line 179 "src/parser/parser.y"
    { (yyval.valuelist)->push_back(*((yyvsp[(1) - (3)].valueval))); delete (yyvsp[(1) - (3)].valueval); ;}
    break;

  case 57:
#line 182 "src/parser/parser.y"
    { (yyval.insert_stmt_val) = new insert_stmt((yyvsp[(3) - (7)].strval), (yyvsp[(6) - (7)].valuelist)); ;}
    break;

  case 58:
#line 186 "src/parser/parser.y"
    {;}
    break;

  case 59:
#line 189 "src/parser/parser.y"
    {;}
    break;

  case 60:
#line 192 "src/parser/parser.y"
    {;}
    break;

  case 61:
#line 197 "src/parser/parser.y"
    {;}
    break;

  case 62:
#line 200 "src/parser/parser.y"
    {;}
    break;


/* Line 1267 of yacc.c.  */
#line 1819 "parser.tab.c"
      default: break;
    }
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
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
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
      if (yyn != YYPACT_NINF)
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

  if (yyn == YYFINAL)
    YYACCEPT;

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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
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


#line 203 "src/parser/parser.y"


int yyerror(const char *s) {
    cout << "Got An Error: " << yylineno << endl;
}


