/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */


#include <stdio.h>
#include <bsd/string.h>
#include "symtable.h"
#include "codegen.h"
#include "vm.h"

extern int yylex();
extern void yyerror(const char *s);


#line 79 "parser.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TMAIN = 258,
    TID = 259,
    TNUM = 260,
    TCHAR = 261,
    TNUMLIT = 262,
    TCHARLIT = 263,
    TSTRLIT = 264,
    TCOMMA = 265,
    TSCOLON = 266,
    TLBRACK = 267,
    TRBRACK = 268,
    TLPAREN = 269,
    TRPAREN = 270,
    TLBRACE = 271,
    TRBRACE = 272,
    TRET = 273,
    TREAD = 274,
    TWRIT = 275,
    TIF = 276,
    TWHILE = 277,
    TELSE = 278,
    TASSIGN = 279,
    TCONDIF = 280,
    TCONDELSE = 281,
    TOR = 282,
    TAND = 283,
    TEQ = 284,
    TNEQ = 285,
    TLT = 286,
    TMT = 287,
    TLE = 288,
    TME = 289,
    TSUM = 290,
    TMIN = 291,
    TMULT = 292,
    TDIV = 293,
    TMOD = 294,
    TNOT = 295
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 14 "parser.y" /* yacc.c:355  */

    char id[100];
    char litchar;
    char *litstr;
    long int litnum;
    struct {
	int addr_goto;
	int addr_goto_false;
    } lbls;
    struct {
	char id[100];
	int addr_ret;
    } func;

#line 172 "parser.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 189 "parser.tab.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   179

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  97
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  192

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   295

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    54,    54,    54,    57,    57,    58,    58,    59,    60,
      60,    61,    64,    64,    67,    67,    68,    68,    69,    70,
      73,    74,    77,    78,    78,    79,    80,    83,    83,    84,
      87,    87,    88,    88,    89,    90,    94,    95,    98,    99,
     102,   103,   104,   105,   105,   106,   107,   108,   109,   110,
     110,   110,   111,   114,   115,   118,   118,   121,   124,   125,
     128,   129,   129,   129,   132,   133,   136,   137,   140,   141,
     142,   145,   146,   147,   148,   149,   152,   153,   154,   157,
     158,   159,   160,   163,   164,   165,   168,   169,   172,   173,
     173,   174,   175,   176,   177,   180,   181,   182
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TMAIN", "TID", "TNUM", "TCHAR",
  "TNUMLIT", "TCHARLIT", "TSTRLIT", "TCOMMA", "TSCOLON", "TLBRACK",
  "TRBRACK", "TLPAREN", "TRPAREN", "TLBRACE", "TRBRACE", "TRET", "TREAD",
  "TWRIT", "TIF", "TWHILE", "TELSE", "TASSIGN", "TCONDIF", "TCONDELSE",
  "TOR", "TAND", "TEQ", "TNEQ", "TLT", "TMT", "TLE", "TME", "TSUM", "TMIN",
  "TMULT", "TDIV", "TMOD", "TNOT", "$accept", "prog", "$@1", "funcvardecl",
  "$@2", "$@3", "$@4", "progdecl", "$@5", "vardecl", "$@6", "$@7",
  "paramlist", "paramlistcont", "$@8", "block", "$@9", "vardecllist",
  "$@10", "$@11", "type", "commlist", "comm", "$@12", "$@13", "$@14",
  "readvars", "ifstmt", "$@15", "expr", "assignexpr", "condexpr", "@16",
  "@17", "orexpr", "andexpr", "eqexpr", "ineqexpr", "addexpr", "multexpr",
  "unexpr", "lvalexpr", "primexpr", "@18", "exprlist", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295
};
# endif

#define YYPACT_NINF -165

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-165)))

#define YYTABLE_NINF -88

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -165,    12,    94,  -165,  -165,  -165,    14,    33,  -165,  -165,
       2,    32,    23,  -165,    65,    73,    60,  -165,    74,    94,
      41,  -165,  -165,    65,    78,    78,  -165,  -165,  -165,    29,
      76,    89,    62,    87,    77,  -165,    82,  -165,    93,    91,
    -165,    94,    73,   105,  -165,   125,    65,  -165,   119,    66,
    -165,  -165,    73,  -165,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    65,     9,    94,
      24,   131,   126,    32,    79,   123,    65,  -165,    65,   128,
      65,    76,    89,    62,    62,    87,    87,    87,    87,    77,
      77,  -165,  -165,  -165,  -165,   133,    65,    73,  -165,  -165,
      65,   137,    11,   130,   132,  -165,   134,    24,   122,   136,
      15,    94,    94,  -165,   135,   129,  -165,    26,   139,    94,
    -165,   142,  -165,  -165,   138,   144,  -165,   146,   147,    65,
    -165,  -165,  -165,    24,  -165,   143,    65,    73,  -165,  -165,
      94,   149,    65,  -165,  -165,  -165,   140,    73,    73,  -165,
      65,   150,  -165,  -165,   148,    65,  -165,   151,  -165,   154,
    -165,    94,  -165,    67,  -165,  -165,   157,   137,   156,  -165,
    -165,    73,    73,    94,  -165,  -165,  -165,   150,  -165,    24,
     158,   160,   161,  -165,  -165,  -165,    24,    94,    94,  -165,
    -165,  -165
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,    11,     1,    36,    37,     0,     0,    12,     3,
       4,     0,     0,     9,     0,    19,    27,    13,     0,    21,
      88,    94,    93,     0,     0,     0,     6,    57,    58,    60,
      64,    66,    68,    71,    76,    79,     0,    83,     0,     0,
      29,    35,    19,     0,    20,     0,     0,    89,     0,    88,
      84,    85,    19,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    14,    11,
       0,     0,     0,     0,    22,     0,    97,    92,     0,     0,
       0,    65,    67,    69,    70,    72,    73,    74,    75,    77,
      78,    80,    81,    82,    59,     0,     0,    19,     5,    40,
       0,     0,     0,     0,     0,    52,     0,    38,    47,     0,
      30,    11,    11,    23,     0,    91,    95,     0,     0,    11,
      62,     0,    16,    15,     0,    86,    43,     0,     0,     0,
      49,    28,    39,     0,    41,     0,     0,    19,     8,    10,
       0,    25,     0,    90,    91,     7,     0,    19,    19,    42,
       0,    54,    46,    45,     0,     0,    48,     0,    32,     0,
      24,     0,    96,     0,    18,    17,     0,     0,     0,    55,
      50,    19,    19,    35,    26,    63,    87,    54,    44,     0,
       0,     0,     0,    31,    53,    56,     0,    35,    35,    51,
      34,    33
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -165,  -165,  -165,   -62,  -165,  -165,  -165,  -165,  -165,   -39,
    -165,  -165,  -165,  -132,  -165,    -6,  -165,  -164,  -165,  -165,
     -19,    54,  -127,  -165,  -165,  -165,   -15,  -165,  -165,   -12,
     -66,    13,  -165,  -165,  -165,   120,   124,    69,    52,    68,
      -3,   -97,   103,  -165,  -165
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     6,    15,    52,    19,     9,    11,    39,
      97,   148,    43,    44,   140,   105,    41,    70,   137,   172,
       7,   106,   107,   151,   155,   180,   168,   108,   179,   109,
      27,    28,    80,   146,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    76,   117
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      45,    94,    26,    72,   126,    17,   156,    98,   160,   183,
     116,    48,     3,    79,    12,    20,    13,     8,    21,    22,
     127,    95,    71,   190,   191,    23,    14,   135,    20,   174,
      18,    21,    22,    96,    75,    99,   142,    10,    23,   136,
      16,   143,   100,   101,   102,   103,   104,    24,    16,   138,
     139,    25,   185,    46,    53,    47,    54,   145,   123,   189,
      24,    91,    92,    93,    25,   -86,   118,   112,   120,    20,
     177,    49,    21,    22,    21,    22,   162,    40,    78,    23,
      47,    23,    49,    38,   122,    21,    22,    42,   124,   113,
     128,   114,    23,    58,    59,    60,    61,    68,   159,     4,
       5,    24,    69,    24,    55,    25,    67,    25,   164,   165,
      85,    86,    87,    88,    64,    65,    66,   154,    56,    57,
      73,    45,    62,    63,   158,    83,    84,    50,    51,    74,
      89,    90,   181,   182,    77,   110,   115,   111,   166,   119,
     121,   125,    45,   170,   129,   133,   130,   134,   141,   149,
     157,   131,   144,   -87,    71,   147,   150,   152,   153,   161,
     167,   132,   184,   169,   171,   173,   163,   178,    71,    71,
     176,   187,   188,   186,    81,     0,   175,     0,     0,    82
};

static const yytype_int16 yycheck[] =
{
      19,    67,    14,    42,   101,    11,   133,    69,   140,   173,
      76,    23,     0,    52,    12,     4,    14,     3,     7,     8,
       9,    12,    41,   187,   188,    14,    24,    12,     4,   161,
       7,     7,     8,    24,    46,    11,    10,     4,    14,    24,
      16,    15,    18,    19,    20,    21,    22,    36,    16,   111,
     112,    40,   179,    12,    25,    14,    27,   119,    97,   186,
      36,    64,    65,    66,    40,    24,    78,    73,    80,     4,
     167,     4,     7,     8,     7,     8,   142,    17,    12,    14,
      14,    14,     4,    10,    96,     7,     8,    13,   100,    10,
     102,    12,    14,    31,    32,    33,    34,     4,   137,     5,
       6,    36,    11,    36,    28,    40,    24,    40,   147,   148,
      58,    59,    60,    61,    37,    38,    39,   129,    29,    30,
      15,   140,    35,    36,   136,    56,    57,    24,    25,     4,
      62,    63,   171,   172,    15,     4,    13,    11,   150,    11,
       7,     4,   161,   155,    14,    23,    14,    11,    13,    11,
       7,    17,    13,    24,   173,    13,    12,    11,    11,    10,
      10,   107,   177,    15,    13,    11,    26,    11,   187,   188,
      13,    11,    11,    15,    54,    -1,   163,    -1,    -1,    55
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    42,    43,     0,     5,     6,    44,    61,     3,    48,
       4,    49,    12,    14,    24,    45,    16,    56,     7,    47,
       4,     7,     8,    14,    36,    40,    70,    71,    72,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    10,    50,
      17,    57,    13,    53,    54,    61,    12,    14,    70,     4,
      83,    83,    46,    25,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    24,     4,    11,
      58,    61,    50,    15,     4,    70,    84,    15,    12,    50,
      73,    76,    77,    78,    78,    79,    79,    79,    79,    80,
      80,    81,    81,    81,    71,    12,    24,    51,    44,    11,
      18,    19,    20,    21,    22,    56,    62,    63,    68,    70,
       4,    11,    56,    10,    12,    13,    71,    85,    70,    11,
      70,     7,    70,    50,    70,     4,    82,     9,    70,    14,
      14,    17,    62,    23,    11,    12,    24,    59,    44,    44,
      55,    13,    10,    15,    13,    44,    74,    13,    52,    11,
      12,    64,    11,    11,    70,    65,    63,     7,    70,    50,
      54,    10,    71,    26,    50,    50,    70,    10,    67,    15,
      70,    13,    60,    11,    54,    72,    13,    82,    11,    69,
      66,    50,    50,    58,    67,    63,    15,    11,    11,    63,
      58,    58
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    41,    43,    42,    45,    44,    46,    44,    44,    47,
      44,    44,    49,    48,    51,    50,    52,    50,    50,    50,
      53,    53,    54,    55,    54,    54,    54,    57,    56,    56,
      59,    58,    60,    58,    58,    58,    61,    61,    62,    62,
      63,    63,    63,    64,    63,    63,    63,    63,    63,    65,
      66,    63,    63,    67,    67,    69,    68,    70,    71,    71,
      72,    73,    74,    72,    75,    75,    76,    76,    77,    77,
      77,    78,    78,    78,    78,    78,    79,    79,    79,    80,
      80,    80,    80,    81,    81,    81,    82,    82,    83,    84,
      83,    83,    83,    83,    83,    85,    85,    85
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     0,     6,     0,     8,     8,     0,
       8,     0,     0,     3,     0,     4,     0,     6,     6,     0,
       1,     0,     2,     0,     5,     4,     6,     0,     5,     2,
       0,     6,     0,     8,     8,     0,     1,     1,     1,     2,
       1,     2,     3,     0,     5,     3,     3,     1,     3,     0,
       0,     7,     1,     3,     0,     0,     6,     1,     1,     3,
       1,     0,     0,     7,     1,     3,     1,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     3,     3,     1,     2,     2,     1,     4,     1,     0,
       5,     4,     3,     1,     1,     1,     3,     0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
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
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
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

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

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

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
      yychar = yylex ();
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

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
#line 54 "parser.y" /* yacc.c:1646  */
    {push_scope();}
#line 1398 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 54 "parser.y" /* yacc.c:1646  */
    {gen_code(HALT, 0); pop_scope(); YYACCEPT;}
#line 1404 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 57 "parser.y" /* yacc.c:1646  */
    {install((yyvsp[0].id), alloc_d());}
#line 1410 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 58 "parser.y" /* yacc.c:1646  */
    {install((yyvsp[-2].id), alloc_d()); context_check(STORE, (yyvsp[-2].id));}
#line 1416 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 60 "parser.y" /* yacc.c:1646  */
    {install((yyvsp[-1].id), gen_label());}
#line 1422 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 64 "parser.y" /* yacc.c:1646  */
    {install((yyvsp[0].id), gen_label()); set_main_offset(gen_label());}
#line 1428 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 67 "parser.y" /* yacc.c:1646  */
    {install((yyvsp[0].id), alloc_d());}
#line 1434 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 68 "parser.y" /* yacc.c:1646  */
    {install((yyvsp[-2].id), alloc_d()); context_check(STORE, (yyvsp[-2].id));}
#line 1440 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 77 "parser.y" /* yacc.c:1646  */
    {install((yyvsp[0].id), alloc_d()); context_check(STORE, (yyvsp[0].id));}
#line 1446 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 78 "parser.y" /* yacc.c:1646  */
    {install((yyvsp[-1].id), alloc_d()); context_check(STORE, (yyvsp[-1].id));}
#line 1452 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 83 "parser.y" /* yacc.c:1646  */
    {push_scope();}
#line 1458 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 83 "parser.y" /* yacc.c:1646  */
    {pop_scope();}
#line 1464 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 87 "parser.y" /* yacc.c:1646  */
    {install((yyvsp[0].id), alloc_d());}
#line 1470 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 88 "parser.y" /* yacc.c:1646  */
    {install((yyvsp[-2].id), alloc_d()); context_check(STORE, (yyvsp[-2].id));}
#line 1476 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 89 "parser.y" /* yacc.c:1646  */
    {install((yyvsp[-6].id), alloc_d());}
#line 1482 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 104 "parser.y" /* yacc.c:1646  */
    {gen_code(RET, 0);}
#line 1488 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 105 "parser.y" /* yacc.c:1646  */
    {gen_code(READ_INT, (yyvsp[0].litnum));}
#line 1494 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 106 "parser.y" /* yacc.c:1646  */
    {gen_code(WRITE_INT, 0);}
#line 1500 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 109 "parser.y" /* yacc.c:1646  */
    {back_patch((yyval.lbls).addr_goto, GOTO, gen_label());}
#line 1506 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 110 "parser.y" /* yacc.c:1646  */
    {(yyvsp[-1].lbls).addr_goto = gen_label();}
#line 1512 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 110 "parser.y" /* yacc.c:1646  */
    {(yyvsp[-3].lbls).addr_goto_false = alloc_c();}
#line 1518 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 110 "parser.y" /* yacc.c:1646  */
    {gen_code(GOTO, (yyvsp[-6].lbls).addr_goto);back_patch((yyvsp[-6].lbls).addr_goto_false, GOTO_FALSE, gen_label()); }
#line 1524 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 114 "parser.y" /* yacc.c:1646  */
    {gen_code(READ_INT, (yyvsp[-1].litnum));}
#line 1530 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 118 "parser.y" /* yacc.c:1646  */
    {(yyvsp[-3].lbls).addr_goto_false = alloc_c();}
#line 1536 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 118 "parser.y" /* yacc.c:1646  */
    {(yyvsp[-5].lbls).addr_goto = alloc_c(); (yyval.lbls) = (yyvsp[-5].lbls); back_patch((yyvsp[-5].lbls).addr_goto_false, GOTO_FALSE, gen_label()); back_patch((yyvsp[-5].lbls).addr_goto, GOTO, gen_label());}
#line 1542 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 125 "parser.y" /* yacc.c:1646  */
    {gen_code(STORE, (yyvsp[-2].litnum));}
#line 1548 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 129 "parser.y" /* yacc.c:1646  */
    {(yyval.lbls).addr_goto_false = alloc_c();}
#line 1554 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 129 "parser.y" /* yacc.c:1646  */
    {(yyval.lbls).addr_goto = alloc_c(); back_patch((yyvsp[-1].lbls).addr_goto_false, GOTO_FALSE, gen_label());}
#line 1560 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 129 "parser.y" /* yacc.c:1646  */
    {back_patch((yyvsp[-2].lbls).addr_goto, GOTO, gen_label());}
#line 1566 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 133 "parser.y" /* yacc.c:1646  */
    {gen_code(OR, 0);}
#line 1572 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 137 "parser.y" /* yacc.c:1646  */
    {gen_code(AND, 0);}
#line 1578 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 141 "parser.y" /* yacc.c:1646  */
    {gen_code(EQ, 0);}
#line 1584 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 142 "parser.y" /* yacc.c:1646  */
    {gen_code(NEQ, 0);}
#line 1590 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 146 "parser.y" /* yacc.c:1646  */
    {gen_code(LT, 0);}
#line 1596 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 147 "parser.y" /* yacc.c:1646  */
    {gen_code(GT, 0);}
#line 1602 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 148 "parser.y" /* yacc.c:1646  */
    {gen_code(LE, 0);}
#line 1608 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 149 "parser.y" /* yacc.c:1646  */
    {gen_code(GE, 0);}
#line 1614 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 153 "parser.y" /* yacc.c:1646  */
    {gen_code(ADD, 0);}
#line 1620 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 154 "parser.y" /* yacc.c:1646  */
    {gen_code(SUB, 0);}
#line 1626 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 158 "parser.y" /* yacc.c:1646  */
    {gen_code(MULT, 0);}
#line 1632 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 159 "parser.y" /* yacc.c:1646  */
    {gen_code(DIV, 0);}
#line 1638 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 160 "parser.y" /* yacc.c:1646  */
    {gen_code(MOD, 0);}
#line 1644 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 168 "parser.y" /* yacc.c:1646  */
    {(yyval.litnum) = getsymbol((yyvsp[0].id), 1)->offset;}
#line 1650 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 172 "parser.y" /* yacc.c:1646  */
    {context_check(LD_VAR, (yyvsp[0].id));}
#line 1656 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 173 "parser.y" /* yacc.c:1646  */
    {strlcpy((yyval.func).id, (yyvsp[-1].id), 100); (yyval.func).addr_ret = alloc_c();}
#line 1662 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 173 "parser.y" /* yacc.c:1646  */
    {context_check(CALL, (yyvsp[-2].func).id); back_patch((yyvsp[-2].func).addr_ret, LD_INT, gen_label());}
#line 1668 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 177 "parser.y" /* yacc.c:1646  */
    {gen_code(LD_INT, (yyvsp[0].litnum));}
#line 1674 "parser.tab.c" /* yacc.c:1646  */
    break;


#line 1678 "parser.tab.c" /* yacc.c:1646  */
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

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
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
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 185 "parser.y" /* yacc.c:1906  */


void
yyerror(const char *s) 
{
    fprintf(stderr, "%s\n", s);
}
