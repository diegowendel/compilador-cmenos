/****************************************************/
/* File: cminus.l                                   */
/* Diego Wendel de Oliveira Ferreira		        */
/* 86774                               		        */
/****************************************************/

%{
#include "globals.h"
#include "util.h"
#include "scan.h"
/* lexeme of identifier or reserved word */
int comentario = 0;
char tokenString[MAXTOKENLEN + 1];
static int yylex(void);
%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t\r]+

%%

"if"            {return IF;}
"else"          {return ELSE;}
"while"         {return WHILE;}
"return"        {return RETURN;}
"int"           {return INT;}
"void"          {return VOID;}
"+"             {return MAIS;}
"-"             {return MENOS;}
"*"             {return VEZES;}
"/"             {return DIVISAO;}
"%"             {return MODULO;}
"<"             {return MENOR;}
"<="            {return MENORIGUAL;}
">"             {return MAIOR;}
">="            {return MAIORIGUAL;}
"=="            {return IGUAL;}
"!="            {return DIFERENTE;}
"="             {return ATRIBUICAO;}
";"             {return SEMI;}
","             {return COMMA;}
"("             {return LPAREN;}
")"             {return RPAREN;}
"["             {return LBRACKET;}
"]"             {return RBRACKET;}
"{"             {return LKEY;}
"}"             {return RKEY;}
{number}        {return NUM;}
{identifier}    {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}
.               {return ERROR;}
"/*"            {
    char c;
    do {
        c = input();
        if (c == EOF) break;
        if (c == '\n') lineno++;
		if (c == '*') comentario = 1;
	    else if (c == '/' && comentario == 1) comentario = 2;
		else comentario = 0;
    } while (comentario != 2);
}

%%

TokenType getToken(void) {
    static int firstTime = TRUE;
    TokenType currentToken;
    if (firstTime) {
        firstTime = FALSE;
        lineno++;
        yyin = source;
        yyout = listing;
    }
    currentToken = yylex();
    strncpy(tokenString, yytext, MAXTOKENLEN);
    if (TraceScan) {
        fprintf(listing, "\t%d: ", lineno);
        printToken(currentToken, tokenString);
    }
    return currentToken;
}
