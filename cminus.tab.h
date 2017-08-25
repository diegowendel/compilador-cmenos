/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
