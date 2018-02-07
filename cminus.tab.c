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
#line 7 "cminus.y" /* yacc.c:339  */

    #define YYPARSER /* distinguishes Yacc output from other code files */

    #include "globals.h"
    #include "util.h"
    #include "scan.h"
    #include "parse.h"

    #define YYSTYPE TreeNode *
    static char * savedName; /* for use in assignments */
    static int savedLineNo;  /* ditto */
    static TreeNode * savedTree; /* stores syntax tree for later return */
    static int yylex(void);
    static int yyerror(char * message);
    static TreeNode * createIntFunction(char * name);
    static TreeNode * createVoidFunction(char * name);
    static void insertNewNode(TreeNode * node);

#line 85 "cminus.tab.c" /* yacc.c:339  */

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

/* In a future release of Bison, this section will be replaced
   by #include "cminus.tab.h".  */
#ifndef YY_YY_CMINUS_TAB_H_INCLUDED
# define YY_YY_CMINUS_TAB_H_INCLUDED
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
    IF = 258,
    ELSE = 259,
    WHILE = 260,
    RETURN = 261,
    ID = 262,
    NUM = 263,
    MAIOR = 264,
    MAIORIGUAL = 265,
    MENOR = 266,
    MENORIGUAL = 267,
    IGUAL = 268,
    DIFERENTE = 269,
    MAIS = 270,
    MENOS = 271,
    VEZES = 272,
    DIVISAO = 273,
    MODULO = 274,
    SHIFT_LEFT = 275,
    SHIFT_RIGHT = 276,
    AND = 277,
    OR = 278,
    XOR = 279,
    NOT = 280,
    LOGICAL_AND = 281,
    LOGICAL_OR = 282,
    ATRIBUICAO = 283,
    ATRIB_MAIS = 284,
    ATRIB_MENOS = 285,
    ATRIB_VEZES = 286,
    ATRIB_DIVISAO = 287,
    ATRIB_MODULO = 288,
    ATRIB_AND = 289,
    ATRIB_OR = 290,
    ATRIB_XOR = 291,
    ATRIB_SHIFT_LEFT = 292,
    ATRIB_SHIFT_RIGHT = 293,
    LPAREN = 294,
    RPAREN = 295,
    SEMI = 296,
    LBRACKET = 297,
    RBRACKET = 298,
    COMMA = 299,
    LKEY = 300,
    RKEY = 301,
    QUESTION = 302,
    COLON = 303,
    ERROR = 304,
    INT = 305,
    VOID = 306
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_CMINUS_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 188 "cminus.tab.c" /* yacc.c:358  */

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
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   153

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  100
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  152

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   306

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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    41,    41,    69,    82,    86,    87,    91,    98,   111,
     117,   126,   139,   140,   144,   157,   161,   167,   177,   187,
     200,   204,   217,   221,   222,   223,   224,   225,   229,   230,
     234,   241,   252,   262,   267,   276,   282,   286,   291,   296,
     301,   306,   311,   316,   321,   326,   331,   336,   344,   349,
     359,   367,   371,   378,   382,   389,   393,   400,   404,   411,
     415,   422,   426,   432,   436,   441,   449,   455,   459,   464,
     469,   474,   482,   488,   492,   497,   505,   511,   515,   520,
     528,   534,   538,   543,   548,   556,   561,   562,   563,   564,
     568,   573,   578,   586,   596,   597,   601,   614,   618,   627,
     636
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF", "ELSE", "WHILE", "RETURN", "ID",
  "NUM", "MAIOR", "MAIORIGUAL", "MENOR", "MENORIGUAL", "IGUAL",
  "DIFERENTE", "MAIS", "MENOS", "VEZES", "DIVISAO", "MODULO", "SHIFT_LEFT",
  "SHIFT_RIGHT", "AND", "OR", "XOR", "NOT", "LOGICAL_AND", "LOGICAL_OR",
  "ATRIBUICAO", "ATRIB_MAIS", "ATRIB_MENOS", "ATRIB_VEZES",
  "ATRIB_DIVISAO", "ATRIB_MODULO", "ATRIB_AND", "ATRIB_OR", "ATRIB_XOR",
  "ATRIB_SHIFT_LEFT", "ATRIB_SHIFT_RIGHT", "LPAREN", "RPAREN", "SEMI",
  "LBRACKET", "RBRACKET", "COMMA", "LKEY", "RKEY", "QUESTION", "COLON",
  "ERROR", "INT", "VOID", "$accept", "program", "declarationList",
  "declaration", "varDeclaration", "typeSpecifier", "funDeclaration",
  "params", "paramList", "param", "compoundStmt", "localDeclarations",
  "statementList", "statement", "expressionStmt", "selectionStmt",
  "iterationStmt", "returnStmt", "expression", "assigmentOperator", "var",
  "conditionalExpression", "logicalOrExpression", "logicalAndExpression",
  "inclusiveOrExpression", "exclusiveOrExpression", "andExpression",
  "equalityExpression", "equalityOperator", "relationalExpression",
  "relationalOperator", "shiftExpression", "shiftOperator",
  "additiveExpression", "additiveOperator", "multiplicativeExpression",
  "multiplicativeOperator", "unaryExpression", "unaryOperator", "ativacao",
  "args", "arg_lista", "id", "num", "vazio", YY_NULLPTR
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306
};
# endif

#define YYPACT_NINF -123

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-123)))

#define YYTABLE_NINF -14

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -35,  -123,  -123,     3,   -35,  -123,  -123,     0,  -123,  -123,
    -123,  -123,   -29,   -30,  -123,    16,   -13,     0,    -7,    -9,
    -123,  -123,    12,    17,    22,   -35,    28,    35,  -123,  -123,
    -123,  -123,  -123,   -35,  -123,  -123,     0,    38,  -123,   -12,
      41,    43,    86,  -123,  -123,  -123,     1,  -123,  -123,  -123,
    -123,  -123,  -123,  -123,  -123,    47,    84,  -123,   -22,    63,
      68,    71,    70,    48,    87,    44,    59,   111,  -123,     1,
    -123,    58,  -123,     1,     1,  -123,    62,    67,  -123,  -123,
    -123,  -123,  -123,  -123,  -123,  -123,  -123,  -123,  -123,  -123,
       1,     1,     1,     1,     1,     1,     1,     1,  -123,  -123,
       1,  -123,  -123,  -123,  -123,     1,  -123,  -123,     1,  -123,
    -123,     1,  -123,  -123,  -123,     1,    66,  -123,     1,    69,
      91,  -123,  -123,  -123,    92,    80,  -123,  -123,    63,    78,
      68,    71,    70,    48,    87,    44,    59,   111,  -123,    90,
      65,    65,  -123,     1,     1,  -123,   130,  -123,  -123,  -123,
      65,  -123
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     9,    10,     0,     2,     4,     5,     0,     6,     1,
       3,    98,     0,     0,     7,     0,    10,     0,     0,    12,
      15,    99,     0,    16,     0,     0,     0,     0,   100,    11,
      14,     8,    17,   100,    20,    19,     0,     0,    22,     0,
       0,     0,     0,    92,    90,    91,     0,    29,    18,    24,
      21,    23,    25,    26,    27,     0,    87,    36,    51,    53,
      55,    57,    59,    61,    63,    67,    73,    77,    81,     0,
      88,    48,    89,     0,     0,    33,     0,     0,    28,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
     100,     0,     0,     0,     0,     0,     0,     0,    64,    65,
       0,    70,    71,    69,    68,     0,    74,    75,     0,    78,
      79,     0,    82,    83,    84,     0,    87,    85,     0,     0,
       0,    34,    86,    97,     0,    94,    95,    35,    52,     0,
      54,    56,    58,    60,    62,    66,    72,    76,    80,     0,
       0,     0,    93,     0,     0,    49,    30,    32,    96,    50,
       0,    31
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -123,  -123,  -123,   131,   103,   -11,  -123,  -123,  -123,   112,
     114,  -123,  -123,  -122,  -123,  -123,  -123,  -123,   -42,  -123,
     -58,    -5,  -123,    50,    46,    51,    45,    52,  -123,    53,
    -123,    39,  -123,    37,  -123,    32,  -123,   -63,  -123,  -123,
    -123,  -123,    49,   132,   -32
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,     5,     6,     7,     8,    18,    19,    20,
      49,    33,    37,    50,    51,    52,    53,    54,    55,    91,
      56,    57,    58,    59,    60,    61,    62,    63,   100,    64,
     105,    65,   108,    66,   111,    67,   115,    68,    69,    70,
     124,   125,    71,    72,    34
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      76,    38,    17,     9,    77,    92,   117,    11,    11,    21,
      13,   116,    14,    15,    17,     1,     2,    43,   146,   147,
       1,    16,    36,    44,    21,    93,    45,   -13,   151,    14,
      15,   119,   120,    24,   116,    25,   116,   116,   116,   116,
      46,    40,   116,    41,    42,    11,    21,   116,   123,   127,
     116,   129,   138,   116,    43,    26,    12,   116,   126,    27,
      44,    98,    99,    45,   106,   107,    23,    28,    40,    31,
      41,    42,    11,    21,   109,   110,   139,    46,    32,    47,
      73,    43,    74,    28,    48,    39,   116,    44,    78,    94,
      45,    95,    97,    11,    21,    96,   101,   102,   103,   104,
     118,   148,    43,   121,    46,    90,    47,   122,    44,   140,
      28,    45,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,   143,    46,   144,    75,   112,   113,
     114,   141,   142,   145,   150,    10,    35,    30,    29,   149,
     130,   132,   128,   137,   135,   136,   131,    22,     0,   133,
       0,     0,     0,   134
};

static const yytype_int16 yycheck[] =
{
      42,    33,    13,     0,    46,    27,    69,     7,     7,     8,
      39,    69,    41,    42,    25,    50,    51,    16,   140,   141,
      50,    51,    33,    22,     8,    47,    25,    40,   150,    41,
      42,    73,    74,    40,    92,    44,    94,    95,    96,    97,
      39,     3,   100,     5,     6,     7,     8,   105,    90,    91,
     108,    93,   115,   111,    16,    43,     7,   115,    90,    42,
      22,    13,    14,    25,    20,    21,    17,    45,     3,    41,
       5,     6,     7,     8,    15,    16,   118,    39,    43,    41,
      39,    16,    39,    45,    46,    36,   144,    22,    41,    26,
      25,    23,    22,     7,     8,    24,     9,    10,    11,    12,
      42,   143,    16,    41,    39,    39,    41,    40,    22,    40,
      45,    25,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    44,    39,    48,    41,    17,    18,
      19,    40,    40,    43,     4,     4,    33,    25,    24,   144,
      94,    96,    92,   111,   105,   108,    95,    15,    -1,    97,
      -1,    -1,    -1,   100
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    50,    51,    53,    54,    55,    56,    57,    58,     0,
      55,     7,    94,    39,    41,    42,    51,    57,    59,    60,
      61,     8,    95,    94,    40,    44,    43,    42,    45,    62,
      61,    41,    43,    63,    96,    56,    57,    64,    96,    94,
       3,     5,     6,    16,    22,    25,    39,    41,    46,    62,
      65,    66,    67,    68,    69,    70,    72,    73,    74,    75,
      76,    77,    78,    79,    81,    83,    85,    87,    89,    90,
      91,    94,    95,    39,    39,    41,    70,    70,    41,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    71,    27,    47,    26,    23,    24,    22,    13,    14,
      80,     9,    10,    11,    12,    82,    20,    21,    84,    15,
      16,    86,    17,    18,    19,    88,    72,    89,    42,    70,
      70,    41,    40,    70,    92,    93,    96,    70,    75,    70,
      76,    77,    78,    79,    81,    83,    85,    87,    89,    70,
      40,    40,    40,    44,    48,    43,    65,    65,    70,    73,
       4,    65
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    52,    53,    54,    54,    55,    55,    56,    56,    57,
      57,    58,    59,    59,    60,    60,    61,    61,    62,    63,
      63,    64,    64,    65,    65,    65,    65,    65,    66,    66,
      67,    67,    68,    69,    69,    70,    70,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    72,    72,
      73,    73,    74,    74,    75,    75,    76,    76,    77,    77,
      78,    78,    79,    79,    80,    80,    81,    81,    82,    82,
      82,    82,    83,    83,    84,    84,    85,    85,    86,    86,
      87,    87,    88,    88,    88,    89,    89,    89,    89,    89,
      90,    90,    90,    91,    92,    92,    93,    93,    94,    95,
      96
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     3,     6,     1,
       1,     6,     1,     1,     3,     1,     2,     4,     4,     2,
       1,     2,     1,     1,     1,     1,     1,     1,     2,     1,
       5,     7,     5,     2,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       5,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     1,     2,     3,     1,     1,     1,
       1,     1,     1,     4,     1,     1,     3,     1,     1,     1,
       0
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
#line 42 "cminus.y" /* yacc.c:1646  */
    {
            insertNewNode(createIntFunction("input"));              // Insert
            insertNewNode(createVoidFunction("output"));            // Output
            insertNewNode(createIntFunction("ldk"));                // LoadDisk
            insertNewNode(createVoidFunction("sdk"));               // StoreDisk
            insertNewNode(createVoidFunction("lim"));               // LoadInstMem
            insertNewNode(createVoidFunction("sim"));               // StoreInstMem
            insertNewNode(createVoidFunction("checkHD"));           // CheckHardDisk
            insertNewNode(createVoidFunction("checkIM"));           // CheckInstMem
            insertNewNode(createVoidFunction("checkDM"));           // CheckDataMem
            insertNewNode(createVoidFunction("addProgramStart"));   // AddProgramStart
            insertNewNode(createIntFunction("readProgramStart"));   // ReadProgramStart
            insertNewNode(createVoidFunction("mmuLower"));          // MMULower
            insertNewNode(createVoidFunction("mmuUpper"));          // MMUUpper
            insertNewNode(createVoidFunction("exec"));              // Exec
            insertNewNode(createVoidFunction("sysCall"));           // SysCall

            TreeNode * temp;
            temp = savedTree;
            while (temp->sibling != NULL) {
                temp = temp->sibling;
            }
            temp->sibling = (yyvsp[0]);
        }
#line 1417 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 70 "cminus.y" /* yacc.c:1646  */
    {
            YYSTYPE t = (yyvsp[-1]);
            if (t != NULL) {
                while (t->sibling != NULL) {
                    t = t->sibling;
                }
                t->sibling = (yyvsp[0]);
                (yyval) = (yyvsp[-1]);
            } else {
                (yyval) = (yyvsp[0]);
            }
        }
#line 1434 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 82 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1440 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 86 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1446 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 87 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1452 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 92 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = (yyvsp[-2]);
            (yyval)->child[0] = (yyvsp[-1]);
            (yyval)->child[0]->type = (yyval)->type;
            (yyval)->child[0]->kind.var.mem = LOCALK;
        }
#line 1463 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 99 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = (yyvsp[-5]);
            (yyval)->child[0] = (yyvsp[-4]);
            (yyval)->child[0]->type = (yyval)->type;
            (yyval)->child[0]->kind.var.mem = LOCALK;
            (yyval)->child[0]->kind.var.varKind = VECTORK;
            (yyval)->child[0]->child[0] = (yyvsp[-2]);
            (yyval)->child[0]->child[0]->type = INTEGER_TYPE;
        }
#line 1477 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 112 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newStmtNode(INTEGERK);
            (yyval)->type = INTEGER_TYPE;
            (yyval)->op = INT;
        }
#line 1487 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 118 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newStmtNode(VOIDK);
            (yyval)->type = VOID_TYPE;
            (yyval)->op = VOID;
        }
#line 1497 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 127 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = (yyvsp[-5]);
            (yyval)->child[0] = (yyvsp[-4]);
            (yyval)->child[0]->type = (yyval)->type;
            (yyval)->child[0]->kind.var.varKind = FUNCTIONK;
            (yyval)->child[0]->kind.var.mem = FUNCTION_MEM;
            (yyval)->child[0]->child[0] = (yyvsp[-2]);
            (yyval)->child[0]->child[1] = (yyvsp[0]);
        }
#line 1511 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 139 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1517 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 140 "cminus.y" /* yacc.c:1646  */
    { (yyval) = NULL; }
#line 1523 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 145 "cminus.y" /* yacc.c:1646  */
    {
            YYSTYPE t = (yyvsp[-2]);
            if (t != NULL) {
            	while (t->sibling != NULL) {
            		t = t->sibling;
            	}
            	t->sibling = (yyvsp[0]);
            	(yyval) = (yyvsp[-2]);
            } else {
            	(yyval) = (yyvsp[0]);
            }
        }
#line 1540 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 157 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1546 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 162 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = (yyvsp[-1]);
            (yyval)->child[0] = (yyvsp[0]);
            (yyval)->child[0]->kind.var.mem = PARAMK;
        }
#line 1556 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 168 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = (yyvsp[-3]);
            (yyval)->child[0] = (yyvsp[-2]);
            (yyval)->child[0]->kind.var.mem = PARAMK;
            (yyval)->child[0]->kind.var.varKind = VECTORK;
        }
#line 1567 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 178 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newStmtNode(COMPK);
            (yyval)->child[0] = (yyvsp[-2]);
            (yyval)->child[1] = (yyvsp[-1]);
            (yyval)->op = COMPK;
        }
#line 1578 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 188 "cminus.y" /* yacc.c:1646  */
    {
        	YYSTYPE t = (yyvsp[-1]);
        	if (t != NULL) {
        		while (t->sibling != NULL) {
        			t = t->sibling;
        		}
        		t->sibling = (yyvsp[0]);
        		(yyval) = (yyvsp[-1]);
        	} else {
        		(yyval) = (yyvsp[0]);
        	}
        }
#line 1595 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 200 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1601 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 205 "cminus.y" /* yacc.c:1646  */
    {
        	YYSTYPE t = (yyvsp[-1]);
        	if (t != NULL) {
        		while (t->sibling != NULL) {
        			t = t->sibling;
        		}
        		t->sibling = (yyvsp[0]);
        		(yyval) = (yyvsp[-1]);
        	} else {
        		(yyval) = (yyvsp[0]);
        	}
        }
#line 1618 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 217 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1624 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 221 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1630 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 222 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1636 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 223 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1642 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 224 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1648 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 225 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1654 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 229 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 1660 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 230 "cminus.y" /* yacc.c:1646  */
    { (yyval) = NULL; }
#line 1666 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 235 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newStmtNode(IFK);
            (yyval)->child[0] = (yyvsp[-2]);
            (yyval)->child[1] = (yyvsp[0]);
            (yyval)->op = IFK;
        }
#line 1677 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 242 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newStmtNode(IFK);
            (yyval)->child[0] = (yyvsp[-4]);
            (yyval)->child[1] = (yyvsp[-2]);
            (yyval)->child[2] = (yyvsp[0]);
            (yyval)->op = IFK;
        }
#line 1689 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 253 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newStmtNode(WHILEK);
        	(yyval)->child[0] = (yyvsp[-2]);
        	(yyval)->child[1] = (yyvsp[0]);
            (yyval)->op = WHILEK;
        }
#line 1700 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 263 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newStmtNode(RETURNK);
            (yyval)->op = RETURNK;
        }
#line 1709 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 268 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newStmtNode(RETURNK);
        	(yyval)->child[0] = (yyvsp[-1]);
            (yyval)->op = RETURNK;
        }
#line 1719 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 277 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = (yyvsp[-1]);
            (yyval)->child[0] = (yyvsp[-2]);
        	(yyval)->child[1] = (yyvsp[0]);
        }
#line 1729 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 282 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1735 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 287 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(ATRIBK);
        	(yyval)->op = ATRIBUICAO;
        }
#line 1744 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 292 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(ATRIBK);
        	(yyval)->op = ATRIB_MAIS;
        }
#line 1753 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 297 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(ATRIBK);
        	(yyval)->op = ATRIB_MENOS;
        }
#line 1762 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 302 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(ATRIBK);
        	(yyval)->op = ATRIB_VEZES;
        }
#line 1771 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 307 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(ATRIBK);
        	(yyval)->op = ATRIB_DIVISAO;
        }
#line 1780 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 312 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(ATRIBK);
        	(yyval)->op = ATRIB_MODULO;
        }
#line 1789 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 317 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(ATRIBK);
        	(yyval)->op = ATRIB_AND;
        }
#line 1798 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 322 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(ATRIBK);
        	(yyval)->op = ATRIB_OR;
        }
#line 1807 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 327 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(ATRIBK);
        	(yyval)->op = ATRIB_XOR;
        }
#line 1816 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 332 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(ATRIBK);
        	(yyval)->op = ATRIB_SHIFT_LEFT;
        }
#line 1825 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 337 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(ATRIBK);
        	(yyval)->op = ATRIB_SHIFT_RIGHT;
        }
#line 1834 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 345 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = (yyvsp[0]);
            (yyval)->kind.var.acesso = ACCESSK;
        }
#line 1843 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 350 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = (yyvsp[-3]);
        	(yyval)->kind.var.varKind = VECTORK;
            (yyval)->kind.var.acesso = ACCESSK;
        	(yyval)->child[0] = (yyvsp[-1]);
        }
#line 1854 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 360 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newStmtNode(IFK);
            (yyval)->child[0] = (yyvsp[-4]);
            (yyval)->child[1] = (yyvsp[-2]);
            (yyval)->child[2] = (yyvsp[0]);
            (yyval)->op = IFK;
        }
#line 1866 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 367 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1872 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 372 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newExpNode(LOGICK);
            (yyval)->op = LOGICAL_OR;
            (yyval)->child[0] = (yyvsp[-2]);
            (yyval)->child[1] = (yyvsp[0]);
        }
#line 1883 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 378 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1889 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 383 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newExpNode(LOGICK);
            (yyval)->op = LOGICAL_AND;
            (yyval)->child[0] = (yyvsp[-2]);
            (yyval)->child[1] = (yyvsp[0]);
        }
#line 1900 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 389 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1906 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 394 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newExpNode(LOGICK);
            (yyval)->op = OR;
            (yyval)->child[0] = (yyvsp[-2]);
            (yyval)->child[1] = (yyvsp[0]);
        }
#line 1917 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 400 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1923 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 405 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newExpNode(LOGICK);
            (yyval)->op = XOR;
            (yyval)->child[0] = (yyvsp[-2]);
            (yyval)->child[1] = (yyvsp[0]);
        }
#line 1934 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 411 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1940 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 416 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newExpNode(LOGICK);
            (yyval)->op = AND;
            (yyval)->child[0] = (yyvsp[-2]);
            (yyval)->child[1] = (yyvsp[0]);
        }
#line 1951 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 422 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1957 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 427 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = (yyvsp[-1]);
            (yyval)->child[0] = (yyvsp[-2]);
            (yyval)->child[1] = (yyvsp[0]);
        }
#line 1967 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 432 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1973 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 437 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newExpNode(RELK);
            (yyval)->op = IGUAL;
        }
#line 1982 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 442 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newExpNode(RELK);
            (yyval)->op = DIFERENTE;
        }
#line 1991 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 450 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = (yyvsp[-1]);
        	(yyval)->child[0] = (yyvsp[-2]);
        	(yyval)->child[1] = (yyvsp[0]);
        }
#line 2001 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 455 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2007 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 460 "cminus.y" /* yacc.c:1646  */
    {
			(yyval) = newExpNode(RELK);
			(yyval)->op = MENORIGUAL;
		}
#line 2016 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 465 "cminus.y" /* yacc.c:1646  */
    {
			(yyval) = newExpNode(RELK);
			(yyval)->op = MENOR;
		}
#line 2025 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 470 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(RELK);
        	(yyval)->op = MAIOR;
        }
#line 2034 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 475 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(RELK);
        	(yyval)->op = MAIORIGUAL;
        }
#line 2043 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 483 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = (yyvsp[-1]);
            (yyval)->child[0] = (yyvsp[-2]);
            (yyval)->child[1] = (yyvsp[0]);
        }
#line 2053 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 488 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2059 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 493 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newExpNode(ARITHK);
            (yyval)->op = SHIFT_LEFT;
        }
#line 2068 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 498 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newExpNode(ARITHK);
            (yyval)->op = SHIFT_RIGHT;
        }
#line 2077 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 506 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = (yyvsp[-1]);
        	(yyval)->child[0] = (yyvsp[-2]);
        	(yyval)->child[1] = (yyvsp[0]);
        }
#line 2087 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 511 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2093 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 516 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(ARITHK);
        	(yyval)->op = MAIS;
        }
#line 2102 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 521 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(ARITHK);
        	(yyval)->op = MENOS;
        }
#line 2111 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 529 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = (yyvsp[-1]);
        	(yyval)->child[0] = (yyvsp[-2]);
        	(yyval)->child[1] = (yyvsp[0]);
        }
#line 2121 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 534 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2127 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 539 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(ARITHK);
        	(yyval)->op = VEZES;
        }
#line 2136 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 544 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newExpNode(ARITHK);
        	(yyval)->op = DIVISAO;
        }
#line 2145 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 549 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newExpNode(ARITHK);
            (yyval)->op = MODULO;
        }
#line 2154 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 557 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = (yyvsp[-1]);
            (yyval)->child[0] = (yyvsp[0]);
        }
#line 2163 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 561 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 2169 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 562 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2175 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 563 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2181 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 564 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2187 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 569 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newExpNode(UNARYK);
            (yyval)->op = AND;
        }
#line 2196 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 574 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newExpNode(UNARYK);
            (yyval)->op = NOT;
        }
#line 2205 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 579 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newExpNode(UNARYK);
            (yyval)->op = MENOS;
        }
#line 2214 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 587 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = (yyvsp[-3]);
        	(yyval)->kind.var.varKind = CALLK;
        	(yyval)->child[0] = (yyvsp[-1]);
            (yyval)->op = CALLK;
        }
#line 2225 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 596 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2231 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 597 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2237 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 602 "cminus.y" /* yacc.c:1646  */
    {
        	YYSTYPE t = (yyvsp[-2]);
        	if (t != NULL) {
        		while (t->sibling != NULL) {
        			t = t->sibling;
        		}
        		t->sibling = (yyvsp[0]);
        		(yyval) = (yyvsp[-2]);
        	} else {
        		(yyval) = (yyvsp[0]);
        	}
        }
#line 2254 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 614 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2260 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 619 "cminus.y" /* yacc.c:1646  */
    {
        	(yyval) = newVarNode(IDK);
        	(yyval)->kind.var.attr.name = copyString(tokenString);
            (yyval)->type = INTEGER_TYPE;
        }
#line 2270 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 628 "cminus.y" /* yacc.c:1646  */
    {
            (yyval) = newVarNode(CONSTK);
            (yyval)->kind.var.attr.val = atoi(tokenString);
            (yyval)->type = INTEGER_TYPE;
		}
#line 2280 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 636 "cminus.y" /* yacc.c:1646  */
    { (yyval) = NULL; }
#line 2286 "cminus.tab.c" /* yacc.c:1646  */
    break;


#line 2290 "cminus.tab.c" /* yacc.c:1646  */
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
#line 639 "cminus.y" /* yacc.c:1906  */


static int yyerror(char * message) {
    fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
    fprintf(listing,"Current token: ");
    printToken(yychar,tokenString);
    Error = TRUE;
    return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with ealier versions of the TINY scanner
 */
static int yylex(void) {
    return getToken();
}

TreeNode * parse(void) {
    yyparse();
    return savedTree;
}

TreeNode * getIntNode(TreeNode * childNode) {
    TreeNode * intNode = newStmtNode(INTEGERK);
    intNode->op = INT;
    intNode->type = INTEGER_TYPE;
    intNode->child[0] = childNode;
    return intNode;
}

TreeNode * getVoidNode(TreeNode * childNode) {
    TreeNode * voidNode = newStmtNode(VOIDK);
    voidNode->op = VOID;
    voidNode->type = VOID_TYPE;
    voidNode->child[0] = childNode;
    return voidNode;
}

static TreeNode * createIntFunction(char * name) {
    TreeNode * function = newVarNode(FUNCTIONK);
    function->lineno = 0;
    function->op = ID;
    function->type = INTEGER_TYPE;
    function->kind.var.mem = FUNCTION_MEM;
    function->kind.var.attr.name = name;
    return getIntNode(function);
}

static TreeNode * createVoidFunction(char * name) {
    TreeNode * function = newVarNode(FUNCTIONK);
    function->lineno = 0;
    function->op = ID;
    function->type = VOID_TYPE;
    function->kind.var.mem = FUNCTION_MEM;
    function->kind.var.attr.name = name;
    return getVoidNode(function);
}

static void insertNewNode(TreeNode * node) {
    TreeNode * temp;
    if (savedTree == NULL) {
        savedTree = node;
        savedTree->sibling = NULL;
    } else {
        temp = savedTree;
        while (temp->sibling != NULL) {
            temp = temp->sibling;
        }
        temp->sibling = node;
        temp->sibling->sibling = NULL;
    }
}
