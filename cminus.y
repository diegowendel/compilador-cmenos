/****************************************************/
/* File: cminus.y                                   */
/* Gramática da Linguagem C-                        */
/* Diego Wendel de Oliveira Ferreira		        */
/* 86774                               		        */
/****************************************************/

%{
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
%}

%token IF WHILE RETURN ELSE
%token ID NUM
%token ATRIBUICAO MAIOR MAIORIGUAL MENOR MENORIGUAL IGUAL DIFERENTE MAIS MENOS VEZES DIVISAO
%token LPAREN RPAREN SEMI LBRACKET RBRACKET COMMA LKEY RKEY
%token ERROR
%token INT VOID
%nonassoc RPAREN
%nonassoc ELSE

%% /* Gramática C- */

programa    		: declaracao_lista { savedTree = $1; }
           		;
declaracao_lista	: declaracao_lista declaracao
                 	{
				YYSTYPE t = $1;
                   		if (t != NULL) {
					while (t->sibling != NULL) {
						 t = t->sibling;
					}
                    	 		t->sibling = $2;
                    			$$ = $1;
				} else {
					$$ = $2;
				}
                 	}
            		| declaracao  { $$ = $1; }
            		;
declaracao	        : var_declaracao { $$ = $1; }
            		| fun_declaracao { $$ = $1; }
            		;
var_declaracao     	: tipo_especificador id SEMI
			{
				$$ = $1;
				$$->child[0] = $2;
				$$->child[0]->type = $$->type;
                $$->child[0]->varMemK = LOCAL;
			}
            		| tipo_especificador id LBRACKET num RBRACKET SEMI
			{
				$$ = $1;
				$$->child[0] = $2;
				$$->child[0]->kind.exp = VectorK;
				$$->child[0]->type = $$->type;
                $$->child[0]->varMemK = LOCAL;
				$$->child[0]->child[0] = $4;
				$$->child[0]->child[0]->type = Integer;
			}
            		;
tipo_especificador 	: INT
			{
				$$ = newStmtNode(IntegerK);
				$$->attr.name = "int";
				$$->type = Integer;
			}
                 	| VOID
			{
				$$ = newStmtNode(VoidK);
				$$->attr.name = "void";
				$$->type = Void;
			}
            		;
fun_declaracao		: tipo_especificador id LPAREN params RPAREN composto_decl
			{
				$$ = $1;
				$$->child[0] = $2;
				$$->child[0]->type = $$->type;
				$$->child[0]->kind.exp = FunctionK;
                $$->child[0]->varMemK = FUNCAO;
				$$->child[0]->child[0] = $4;
				$$->child[0]->child[1] = $6;
			}
			;
params			: param_lista { $$ = $1; }
			| VOID { $$ = NULL; }
			;
param_lista 		: param_lista COMMA param
			{
				YYSTYPE t = $1;
				if (t != NULL) {
					while (t->sibling != NULL) {
						t = t->sibling;
					}
					t->sibling = $3;
					$$ = $1;
				} else {
					$$ = $3;
				}
			}
			| param { $$ = $1; }
			;
param 			: tipo_especificador id
			{
				$$ = $1;
				$$->child[0] = $2;
				$$->child[0]->type = $$->type;
                $$->child[0]->varMemK = PARAM;
			}
			| tipo_especificador id LBRACKET RBRACKET
			{
				$$ = $1;
				$$->child[0] = $2;
				$$->child[0]->type = $$->type;
                $$->child[0]->varMemK = PARAM;
			}
			;
composto_decl		: LKEY local_declaracoes statement_lista RKEY
			{
				$$ = newStmtNode(CompK);
				$$->child[0] = $2;
				$$->child[1] = $3;
			}
			;
local_declaracoes 	: local_declaracoes var_declaracao
			{
				YYSTYPE t = $1;
				if (t != NULL) {
					while (t->sibling != NULL) {
						t = t->sibling;
					}
					t->sibling = $2;
					$$ = $1;
				} else {
					$$ = $2;
				}
			}
			| vazio { $$ = $1; }
			;
statement_lista 	: statement_lista statement
			{
				YYSTYPE t = $1;
				if (t != NULL) {
					while (t->sibling != NULL) {
						t = t->sibling;
					}
					t->sibling = $2;
					$$ = $1;
				} else {
					$$ = $2;
				}
			}
			| vazio { $$ = $1; }
			;
statement 	: expressao_decl { $$ = $1; }
			| composto_decl { $$ = $1; }
			| selecao_decl { $$ = $1; }
			| iteracao_decl { $$ = $1; }
			| retorno_decl { $$ = $1; }
			;
expressao_decl 		: expressao SEMI { $$ = $1; }
			| SEMI { $$ = NULL; }
			;
selecao_decl	 	: IF LPAREN expressao RPAREN statement
			{
				$$ = newStmtNode(IfK);
				$$->child[0] = $3;
				$$->child[1] = $5;
			}
			| IF LPAREN expressao RPAREN statement ELSE statement
			{
				$$ = newStmtNode(IfK);
				$$->child[0] = $3;
				$$->child[1] = $5;
				$$->child[2] = $7;
			}
			;
iteracao_decl 		: WHILE LPAREN expressao RPAREN statement
			{
				$$ = newStmtNode(WhileK);
				$$->child[0] = $3;
				$$->child[1] = $5;
			}
			;
retorno_decl 		: RETURN SEMI
			{
				$$ = newStmtNode(ReturnK);
			}
			| RETURN expressao SEMI
			{
				$$ = newStmtNode(ReturnK);
				$$->child[0] = $2;
			}
			;
expressao 		: var ATRIBUICAO expressao
			{
				$$ = newExpNode(OpK);
				$$->attr.op = ATRIBUICAO;
				$$->child[0] = $1;
				$$->child[1] = $3;
			}
			| simples_expressao { $$ = $1; }
			;
var 			: id
			{
				$$ = $1;
                $$->type = Integer;
                $$->varAccess = ACESSANDO;
			}
			| id LBRACKET expressao RBRACKET
			{
				$$ = $1;
				$$->kind.exp = VectorK;
                $$->type = Integer;
                $$->varAccess = ACESSANDO;
				$$->child[0] = $3;
			}
			;
simples_expressao 	: soma_expressao relacional soma_expressao
			{
				$$ = $2;
				$$->child[0] = $1;
				$$->child[1] = $3;
			}
			| soma_expressao
			{
				$$ = $1;
			}
			;
relacional 	: MENORIGUAL
			{
				$$ = newExpNode(OpK);
				$$->attr.op = MENORIGUAL;
			}
			| MENOR
			{
				$$ = newExpNode(OpK);
				$$->attr.op = MENOR;
			}
			| MAIOR
			{
				$$ = newExpNode(OpK);
				$$->attr.op = MAIOR;
			}
			| MAIORIGUAL
			{
				$$ = newExpNode(OpK);
				$$->attr.op = MAIORIGUAL;
			}
			| IGUAL
			{
				$$ = newExpNode(OpK);
				$$->attr.op = IGUAL;
			}
			| DIFERENTE
			{
				$$ = newExpNode(OpK);
				$$->attr.op = DIFERENTE;
			}
			;
soma_expressao 		: soma_expressao soma termo
			{
				$$ = $2;
				$$->child[0] = $1;
				$$->child[1] = $3;
			}
			| termo { $$ = $1; }
			;
soma		: MAIS
			{
				$$ = newExpNode(OpK);
				$$->attr.op = MAIS;
			}
			| MENOS
			{
				$$ = newExpNode(OpK);
				$$->attr.op = MENOS;
			}
			;
termo 			: termo mult fator
			{
				$$ = $2;
				$$->child[0] = $1;
				$$->child[1] = $3;
			}
			| fator { $$ = $1; }
			;
mult 		: VEZES
			{
				$$ = newExpNode(OpK);
				$$->attr.op = VEZES;
			}
			| DIVISAO
			{
				$$ = newExpNode(OpK);
				$$->attr.op = DIVISAO;
			}
			;
fator 			: LPAREN expressao RPAREN { $$ = $2; }
			| var { $$ = $1; }
			| ativacao { $$ = $1; }
			| num { $$ = $1; }
			;
ativacao 		: var LPAREN args RPAREN
			{
				$$ = $1;
				$$->kind.exp = CallK;
				$$->child[0] = $3;
			}
			;
args 			: arg_lista { $$ = $1; }
			|   vazio { $$ = $1; }
			;
arg_lista 		: arg_lista COMMA expressao
			{
				YYSTYPE t = $1;
				if (t != NULL) {
					while (t->sibling != NULL) {
						t = t->sibling;
					}
					t->sibling = $3;
					$$ = $1;
				} else {
					$$ = $3;
				}
			}
			| expressao { $$ = $1; }
			;
id			: ID
			{
				$$ = newExpNode(IdK);
				$$->attr.name = copyString(tokenString);
                $$->varAccess = DECLARANDO;
			}
			;
num			: NUM
			{
				$$ = newExpNode(ConstK);
				$$->attr.val = atoi(tokenString);
			}
			;
vazio			: { $$ = NULL; }
			;
%%

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
