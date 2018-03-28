/****************************************************/
/* File: cminus.l                                   */
/* Especificação léxica para o C-                   */
/* Diego Wendel de Oliveira Ferreira                */
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

digit           [0-9]
number          {digit}+
letter          [a-zA-Z_]
letterOrNumber  [a-zA-Z_0-9]
identifier      {letter}{letterOrNumber}*
newline         \n
whitespace      [ \t\r]+

%%

"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"return"        { return RETURN; }
"int"           { return INT; }
"void"          { return VOID; }

"+"             { return MAIS; }
"-"             { return MENOS; }
"*"             { return VEZES; }
"/"             { return DIVISAO; }
"%"             { return MODULO; }

"<<"            { return SHIFT_LEFT; }
">>"            { return SHIFT_RIGHT; }

"&"             { return AND; }
"|"             { return OR; }
"^"             { return XOR; }
"!"             { return NOT; }

"&&"            { return LOGICAL_AND; }
"||"            { return LOGICAL_OR; }

"="             { return ATRIBUICAO; }
"+="            { return ATRIB_MAIS; }
"-="            { return ATRIB_MENOS; }
"*="            { return ATRIB_VEZES; }
"/="            { return ATRIB_DIVISAO; }
"%="            { return ATRIB_MODULO; }
"&="            { return ATRIB_AND; }
"|="            { return ATRIB_OR; }
"^="            { return ATRIB_XOR; }
"<<="           { return ATRIB_SHIFT_LEFT; }
">>="           { return ATRIB_SHIFT_RIGHT; }

"<"             { return MENOR; }
"<="            { return MENORIGUAL; }
">"             { return MAIOR; }
">="            { return MAIORIGUAL; }
"=="            { return IGUAL; }
"!="            { return DIFERENTE; }

"?"             { return QUESTION; }
":"             { return COLON; }
";"             { return SEMI; }
","             { return COMMA; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"["             { return LBRACKET; }
"]"             { return RBRACKET; }
"{"             { return LKEY; }
"}"             { return RKEY; }

{number}        { return NUM; }
{identifier}    { return ID; }
{newline}       { lineno++; }
{whitespace}    { /* skip whitespace */ }

.               { return ERROR; }

"//"            {
    char c;
    while (c = input()) {
        if (c == EOF) break;
        else if (c == '\n') {
            lineno++;
            break;
        }
    }
}

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
