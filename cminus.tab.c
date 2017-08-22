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
    static TreeNode * insertIOFunctions();

#line 83 "cminus.tab.c" /* yacc.c:339  */

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
    WHILE = 259,
    RETURN = 260,
    ELSE = 261,
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
    INTERROGACAO = 283,
    COLON = 284,
    ATRIBUICAO = 285,
    ATRIB_MAIS = 286,
    ATRIB_MENOS = 287,
    ATRIB_VEZES = 288,
    ATRIB_DIVISAO = 289,
    ATRIB_MODULO = 290,
    ATRIB_AND = 291,
    ATRIB_OR = 292,
    ATRIB_XOR = 293,
    ATRIB_SHIFT_LEFT = 294,
    ATRIB_SHIFT_RIGHT = 295,
    LPAREN = 296,
    RPAREN = 297,
    SEMI = 298,
    LBRACKET = 299,
    RBRACKET = 300,
    COMMA = 301,
    LKEY = 302,
    RKEY = 303,
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

#line 186 "cminus.tab.c" /* yacc.c:358  */

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
#define YYLAST   137

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  96
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  147

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
       0,    38,    38,    44,    57,    59,    60,    62,    69,    80,
      86,    93,   104,   105,   107,   120,   122,   129,   138,   145,
     158,   160,   173,   175,   176,   177,   178,   179,   181,   182,
     184,   190,   198,   205,   209,   215,   221,   223,   228,   233,
     238,   243,   248,   253,   258,   263,   268,   273,   279,   285,
     294,   301,   303,   310,   312,   319,   321,   328,   330,   337,
     339,   346,   348,   354,   356,   361,   367,   373,   375,   380,
     385,   390,   396,   402,   404,   409,   415,   421,   423,   428,
     434,   440,   442,   447,   452,   458,   459,   460,   461,   463,
     470,   471,   473,   486,   488,   495,   501
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF", "WHILE", "RETURN", "ELSE", "ID",
  "NUM", "MAIOR", "MAIORIGUAL", "MENOR", "MENORIGUAL", "IGUAL",
  "DIFERENTE", "MAIS", "MENOS", "VEZES", "DIVISAO", "MODULO", "SHIFT_LEFT",
  "SHIFT_RIGHT", "AND", "OR", "XOR", "NOT", "LOGICAL_AND", "LOGICAL_OR",
  "INTERROGACAO", "COLON", "ATRIBUICAO", "ATRIB_MAIS", "ATRIB_MENOS",
  "ATRIB_VEZES", "ATRIB_DIVISAO", "ATRIB_MODULO", "ATRIB_AND", "ATRIB_OR",
  "ATRIB_XOR", "ATRIB_SHIFT_LEFT", "ATRIB_SHIFT_RIGHT", "LPAREN", "RPAREN",
  "SEMI", "LBRACKET", "RBRACKET", "COMMA", "LKEY", "RKEY", "ERROR", "INT",
  "VOID", "$accept", "programa", "declaracao_lista", "declaracao",
  "var_declaracao", "tipo_especificador", "fun_declaracao", "params",
  "param_lista", "param", "composto_decl", "local_declaracoes",
  "statement_lista", "statement", "expressao_decl", "selecao_decl",
  "iteracao_decl", "retorno_decl", "expressao", "atribuicao_operador",
  "var", "condicional_expressao", "or_logico_expressao",
  "and_logico_expressao", "or_inclusivo_expressao",
  "or_exclusivo_expressao", "and_expressao", "igualdade_expressao",
  "igualdade_operador", "relacional_expressao", "relacional_operador",
  "shift_expressao", "shift_operador", "soma_expressao", "soma_operador",
  "mult_expressao", "mult_operador", "fator", "ativacao", "args",
  "arg_lista", "id", "num", "vazio", YY_NULLPTR
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

#define YYPACT_NINF -96

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-96)))

#define YYTABLE_NINF -14

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       9,   -96,   -96,     4,     9,   -96,   -96,     5,   -96,   -96,
     -96,   -96,   -20,    40,   -96,    12,   -17,     5,    -7,    -9,
     -96,   -96,     1,     8,    19,     9,     6,    58,   -96,   -96,
     -96,   -96,   -96,     9,   -96,   -96,     5,    10,   -96,    49,
      27,    30,    31,     2,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,   -96,    61,    45,   -96,    67,    80,    82,    83,    87,
      84,    53,    79,    86,    70,   -96,   -96,    64,   -96,     2,
       2,   -96,    68,    71,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,   -96,   -96,   -96,   -96,   -96,     2,     2,     2,     2,
       2,     2,     2,     2,   -96,   -96,     2,   -96,   -96,   -96,
     -96,     2,   -96,   -96,     2,   -96,   -96,     2,   -96,   -96,
     -96,     2,     2,    72,    73,   -96,   -96,   -96,    74,    66,
     -96,   -96,    69,    80,    88,    82,    83,    87,    84,    53,
      79,    86,    70,   -96,    75,    26,    26,   -96,     2,     2,
     -96,   112,   -96,   -96,   -96,    26,   -96
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     9,    10,     0,     2,     4,     5,     0,     6,     1,
       3,    94,     0,     0,     7,     0,    10,     0,     0,    12,
      15,    95,     0,    16,     0,     0,     0,     0,    96,    11,
      14,     8,    17,    96,    20,    19,     0,     0,    22,     0,
       0,     0,     0,     0,    29,    18,    24,    21,    23,    25,
      26,    27,     0,    86,    36,    51,    53,    55,    57,    59,
      61,    63,    67,    73,    77,    81,    87,    48,    88,     0,
       0,    33,     0,     0,    28,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    96,     0,     0,     0,
       0,     0,     0,     0,    64,    65,     0,    70,    71,    69,
      68,     0,    74,    75,     0,    78,    79,     0,    82,    83,
      84,     0,     0,     0,     0,    34,    85,    93,     0,    90,
      91,    35,    86,    52,     0,    54,    56,    58,    60,    62,
      66,    72,    76,    80,     0,     0,     0,    89,     0,     0,
      49,    30,    32,    92,    50,     0,    31
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -96,   -96,   -96,   115,    89,    23,   -96,   -96,   -96,    96,
      99,   -96,   -96,   -95,   -96,   -96,   -96,   -96,   -42,   -96,
     -85,   -15,   -96,    37,    36,    38,    35,    39,   -96,    32,
     -96,    29,   -96,    33,   -96,    24,   -96,    22,   -96,   -96,
     -96,    25,   119,   -31
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,     5,     6,     7,     8,    18,    19,    20,
      46,    33,    37,    47,    48,    49,    50,    51,    52,    87,
      53,    54,    55,    56,    57,    58,    59,    60,    96,    61,
     101,    62,   104,    63,   107,    64,   111,    65,    66,   118,
     119,    67,    68,    34
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      72,    73,    38,   122,     9,   122,   122,   122,   122,    11,
      21,   122,    11,    40,    41,    42,   122,    11,    21,   122,
      21,    13,   122,    14,    15,   -13,   122,   113,   114,    40,
      41,    42,    12,    11,    21,    24,    17,    25,    11,    21,
     141,   142,    23,    43,   117,   121,    26,   124,    17,    31,
     146,    43,    27,    44,   122,   120,    36,    28,    45,     1,
       2,    39,    97,    98,    99,   100,    28,    43,    69,    44,
     134,    70,    43,    28,    71,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,   108,   109,   110,
       1,    16,    14,    15,    88,    89,   143,    94,    95,   102,
     103,   105,   106,    32,    74,    91,    90,    92,   112,    93,
      86,   115,   138,   116,   135,   136,   137,   139,   145,    10,
     140,    30,    35,    29,   144,   123,   125,   127,   129,   126,
     130,   132,   128,   133,    22,     0,     0,   131
};

static const yytype_int16 yycheck[] =
{
      42,    43,    33,    88,     0,    90,    91,    92,    93,     7,
       8,    96,     7,     3,     4,     5,   101,     7,     8,   104,
       8,    41,   107,    43,    44,    42,   111,    69,    70,     3,
       4,     5,     7,     7,     8,    42,    13,    46,     7,     8,
     135,   136,    17,    41,    86,    87,    45,    89,    25,    43,
     145,    41,    44,    43,   139,    86,    33,    47,    48,    50,
      51,    36,     9,    10,    11,    12,    47,    41,    41,    43,
     112,    41,    41,    47,    43,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    17,    18,    19,
      50,    51,    43,    44,    27,    28,   138,    13,    14,    20,
      21,    15,    16,    45,    43,    23,    26,    24,    44,    22,
      41,    43,    46,    42,    42,    42,    42,    29,     6,     4,
      45,    25,    33,    24,   139,    88,    90,    92,    96,    91,
     101,   107,    93,   111,    15,    -1,    -1,   104
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    50,    51,    53,    54,    55,    56,    57,    58,     0,
      55,     7,    93,    41,    43,    44,    51,    57,    59,    60,
      61,     8,    94,    93,    42,    46,    45,    44,    47,    62,
      61,    43,    45,    63,    95,    56,    57,    64,    95,    93,
       3,     4,     5,    41,    43,    48,    62,    65,    66,    67,
      68,    69,    70,    72,    73,    74,    75,    76,    77,    78,
      79,    81,    83,    85,    87,    89,    90,    93,    94,    41,
      41,    43,    70,    70,    43,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    71,    27,    28,
      26,    23,    24,    22,    13,    14,    80,     9,    10,    11,
      12,    82,    20,    21,    84,    15,    16,    86,    17,    18,
      19,    88,    44,    70,    70,    43,    42,    70,    91,    92,
      95,    70,    72,    75,    70,    76,    77,    78,    79,    81,
      83,    85,    87,    89,    70,    42,    42,    42,    46,    29,
      45,    65,    65,    70,    73,     6,    65
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
      87,    87,    88,    88,    88,    89,    89,    89,    89,    90,
      91,    91,    92,    92,    93,    94,    95
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
       3,     1,     1,     1,     1,     3,     1,     1,     1,     4,
       1,     1,     3,     1,     1,     1,     0
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
#line 39 "cminus.y" /* yacc.c:1646  */
    {
                                savedTree = insertIOFunctions();
                                savedTree->sibling->sibling = (yyvsp[0]);
                            }
#line 1385 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 45 "cminus.y" /* yacc.c:1646  */
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
#line 1402 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 57 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1408 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 59 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1414 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 60 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1420 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 63 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = (yyvsp[-2]);
                                (yyval)->child[0] = (yyvsp[-1]);
                                (yyval)->child[0]->type = (yyval)->type;
                                (yyval)->child[0]->varMemK = LOCAL;
                            }
#line 1431 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 70 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = (yyvsp[-5]);
                                (yyval)->child[0] = (yyvsp[-4]);
                                (yyval)->child[0]->kind.exp = VectorK;
                                (yyval)->child[0]->type = (yyval)->type;
                                (yyval)->child[0]->varMemK = LOCAL;
                                (yyval)->child[0]->child[0] = (yyvsp[-2]);
                                (yyval)->child[0]->child[0]->type = Integer;
                            }
#line 1445 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 81 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = newStmtNode(IntegerK);
                                (yyval)->attr.name = "int";
                                (yyval)->type = Integer;
                            }
#line 1455 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 87 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = newStmtNode(VoidK);
                                (yyval)->attr.name = "void";
                                (yyval)->type = Void;
                            }
#line 1465 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 94 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = (yyvsp[-5]);
                                (yyval)->child[0] = (yyvsp[-4]);
                                (yyval)->child[0]->type = (yyval)->type;
                                (yyval)->child[0]->kind.exp = FunctionK;
                                (yyval)->child[0]->varMemK = FUNCAO;
                                (yyval)->child[0]->child[0] = (yyvsp[-2]);
                                (yyval)->child[0]->child[1] = (yyvsp[0]);
                            }
#line 1479 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 104 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1485 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 105 "cminus.y" /* yacc.c:1646  */
    { (yyval) = NULL; }
#line 1491 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 108 "cminus.y" /* yacc.c:1646  */
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
#line 1508 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 120 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1514 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 123 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = (yyvsp[-1]);
                                (yyval)->child[0] = (yyvsp[0]);
                                (yyval)->child[0]->type = (yyval)->type;
                                (yyval)->child[0]->varMemK = PARAM;
                            }
#line 1525 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 130 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = (yyvsp[-3]);
                                (yyval)->child[0] = (yyvsp[-2]);
                                (yyval)->child[0]->type = (yyval)->type;
                                (yyval)->child[0]->varMemK = PARAM;
                                (yyval)->child[0]->kind.exp = VectorK;
                            }
#line 1537 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 139 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = newStmtNode(CompK);
                                (yyval)->child[0] = (yyvsp[-2]);
                                (yyval)->child[1] = (yyvsp[-1]);
                            }
#line 1547 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 146 "cminus.y" /* yacc.c:1646  */
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
#line 1564 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 158 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1570 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 161 "cminus.y" /* yacc.c:1646  */
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
#line 1587 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 173 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1593 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 175 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1599 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 176 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1605 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 177 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1611 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 178 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1617 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 179 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1623 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 181 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 1629 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 182 "cminus.y" /* yacc.c:1646  */
    { (yyval) = NULL; }
#line 1635 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 185 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = newStmtNode(IfK);
                                (yyval)->child[0] = (yyvsp[-2]);
                                (yyval)->child[1] = (yyvsp[0]);
                            }
#line 1645 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 191 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = newStmtNode(IfK);
                                (yyval)->child[0] = (yyvsp[-4]);
                                (yyval)->child[1] = (yyvsp[-2]);
                                (yyval)->child[2] = (yyvsp[0]);
                            }
#line 1656 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 199 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newStmtNode(WhileK);
                            	(yyval)->child[0] = (yyvsp[-2]);
                            	(yyval)->child[1] = (yyvsp[0]);
                            }
#line 1666 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 206 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newStmtNode(ReturnK);
                            }
#line 1674 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 210 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newStmtNode(ReturnK);
                            	(yyval)->child[0] = (yyvsp[-1]);
                            }
#line 1683 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 216 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = (yyvsp[-1]);
                                (yyval)->child[0] = (yyvsp[-2]);
                            	(yyval)->child[1] = (yyvsp[0]);
                            }
#line 1693 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 221 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1699 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 224 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = ATRIBUICAO;
                            }
#line 1708 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 229 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = ATRIB_MAIS;
                            }
#line 1717 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 234 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = ATRIB_MENOS;
                            }
#line 1726 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 239 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = ATRIB_VEZES;
                            }
#line 1735 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 244 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = ATRIB_DIVISAO;
                            }
#line 1744 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 249 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = ATRIB_MODULO;
                            }
#line 1753 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 254 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = ATRIB_AND;
                            }
#line 1762 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 259 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = ATRIB_OR;
                            }
#line 1771 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 264 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = ATRIB_XOR;
                            }
#line 1780 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 269 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = ATRIB_SHIFT_LEFT;
                            }
#line 1789 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 274 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = ATRIB_SHIFT_RIGHT;
                            }
#line 1798 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 280 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = (yyvsp[0]);
                                (yyval)->type = Integer;
                                (yyval)->varAccess = ACESSANDO;
                            }
#line 1808 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 286 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = (yyvsp[-3]);
                            	(yyval)->kind.exp = VectorK;
                                (yyval)->type = Integer;
                                (yyval)->varAccess = ACESSANDO;
                            	(yyval)->child[0] = (yyvsp[-1]);
                            }
#line 1820 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 295 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = newStmtNode(IfK);
                                (yyval)->child[0] = (yyvsp[-4]);
                                (yyval)->child[1] = (yyvsp[-2]);
                                (yyval)->child[2] = (yyvsp[0]);
                            }
#line 1831 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 301 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1837 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 304 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = newExpNode(OpK);
                                (yyval)->attr.op = LOGICAL_OR;
                                (yyval)->child[0] = (yyvsp[-2]);
                                (yyval)->child[1] = (yyvsp[0]);
                            }
#line 1848 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 310 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1854 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 313 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = newExpNode(OpK);
                                (yyval)->attr.op = LOGICAL_AND;
                                (yyval)->child[0] = (yyvsp[-2]);
                                (yyval)->child[1] = (yyvsp[0]);
                            }
#line 1865 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 319 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1871 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 322 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = newExpNode(OpK);
                                (yyval)->attr.op = OR;
                                (yyval)->child[0] = (yyvsp[-2]);
                                (yyval)->child[1] = (yyvsp[0]);
                            }
#line 1882 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 328 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1888 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 331 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = newExpNode(OpK);
                                (yyval)->attr.op = XOR;
                                (yyval)->child[0] = (yyvsp[-2]);
                                (yyval)->child[1] = (yyvsp[0]);
                            }
#line 1899 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 337 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1905 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 340 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = newExpNode(OpK);
                                (yyval)->attr.op = AND;
                                (yyval)->child[0] = (yyvsp[-2]);
                                (yyval)->child[1] = (yyvsp[0]);
                            }
#line 1916 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 346 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1922 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 349 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = (yyvsp[-1]);
                                (yyval)->child[0] = (yyvsp[-2]);
                                (yyval)->child[1] = (yyvsp[0]);
                            }
#line 1932 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 354 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1938 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 357 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = newExpNode(OpK);
                                (yyval)->attr.op = IGUAL;
                            }
#line 1947 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 362 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = newExpNode(OpK);
                                (yyval)->attr.op = DIFERENTE;
                            }
#line 1956 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 368 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = (yyvsp[-1]);
                            	(yyval)->child[0] = (yyvsp[-2]);
                            	(yyval)->child[1] = (yyvsp[0]);
                            }
#line 1966 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 373 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1972 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 376 "cminus.y" /* yacc.c:1646  */
    {
                				(yyval) = newExpNode(OpK);
                				(yyval)->attr.op = MENORIGUAL;
                			}
#line 1981 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 381 "cminus.y" /* yacc.c:1646  */
    {
                				(yyval) = newExpNode(OpK);
                				(yyval)->attr.op = MENOR;
                			}
#line 1990 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 386 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = MAIOR;
                            }
#line 1999 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 391 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = MAIORIGUAL;
                            }
#line 2008 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 397 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = (yyvsp[-1]);
                                (yyval)->child[0] = (yyvsp[-2]);
                                (yyval)->child[1] = (yyvsp[0]);
                            }
#line 2018 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 402 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2024 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 405 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = newExpNode(OpK);
                                (yyval)->attr.op = SHIFT_LEFT;
                            }
#line 2033 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 410 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = newExpNode(OpK);
                                (yyval)->attr.op = SHIFT_RIGHT;
                            }
#line 2042 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 416 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = (yyvsp[-1]);
                            	(yyval)->child[0] = (yyvsp[-2]);
                            	(yyval)->child[1] = (yyvsp[0]);
                            }
#line 2052 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 421 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2058 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 424 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = MAIS;
                            }
#line 2067 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 429 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = MENOS;
                            }
#line 2076 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 435 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = (yyvsp[-1]);
                            	(yyval)->child[0] = (yyvsp[-2]);
                            	(yyval)->child[1] = (yyvsp[0]);
                            }
#line 2086 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 440 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2092 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 443 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = VEZES;
                            }
#line 2101 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 448 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(OpK);
                            	(yyval)->attr.op = DIVISAO;
                            }
#line 2110 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 453 "cminus.y" /* yacc.c:1646  */
    {
                                (yyval) = newExpNode(OpK);
                                (yyval)->attr.op = MODULO;
                            }
#line 2119 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 458 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 2125 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 459 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2131 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 460 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2137 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 461 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2143 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 464 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = (yyvsp[-3]);
                            	(yyval)->kind.exp = CallK;
                            	(yyval)->child[0] = (yyvsp[-1]);
                            }
#line 2153 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 470 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2159 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 471 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2165 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 474 "cminus.y" /* yacc.c:1646  */
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
#line 2182 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 486 "cminus.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2188 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 489 "cminus.y" /* yacc.c:1646  */
    {
                            	(yyval) = newExpNode(IdK);
                            	(yyval)->attr.name = copyString(tokenString);
                                (yyval)->varAccess = DECLARANDO;
                            }
#line 2198 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 496 "cminus.y" /* yacc.c:1646  */
    {
                				(yyval) = newExpNode(ConstK);
                				(yyval)->attr.val = atoi(tokenString);
                			}
#line 2207 "cminus.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 501 "cminus.y" /* yacc.c:1646  */
    { (yyval) = NULL; }
#line 2213 "cminus.tab.c" /* yacc.c:1646  */
    break;


#line 2217 "cminus.tab.c" /* yacc.c:1646  */
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
#line 503 "cminus.y" /* yacc.c:1906  */


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

/* Insere as funções input e output na árvore sintática */
static TreeNode * insertIOFunctions() {
    /*********** Output **********/
    TreeNode * output = newExpNode(FunctionK);
    output->attr.name = "output";
    output->type = Void;
    output->varMemK = FUNCAO;
    output->lineno = 0;

    TreeNode * voidNode = newStmtNode(VoidK);
    voidNode->attr.name = "void";
    voidNode->type = Void;
    voidNode->child[0] = output;

    /********** Input **********/
    TreeNode * input = newExpNode(FunctionK);
    input->attr.name = "input";
    input->type = Integer;
    input->varMemK = FUNCAO;
    input->lineno = 0;

    TreeNode * intNode = newStmtNode(IntegerK);
    intNode->attr.name = "int";
    intNode->type = Integer;
    intNode->child[0] = input;

    /********** Adicionando na árvore **********/
    intNode->sibling = voidNode;
    return intNode;
}
