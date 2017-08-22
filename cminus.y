/****************************************************/
/* File: cminus.y                                   */
/* Gramática da Linguagem C-                        */
/* Diego Wendel de Oliveira Ferreira                */
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
    static TreeNode * insertIOFunctions();
%}

%token IF WHILE RETURN ELSE
%token ID NUM
%token MAIOR MAIORIGUAL MENOR MENORIGUAL IGUAL DIFERENTE MAIS MENOS VEZES DIVISAO MODULO
%token SHIFT_LEFT SHIFT_RIGHT AND OR XOR NOT LOGICAL_AND LOGICAL_OR
%token ATRIBUICAO ATRIB_MAIS ATRIB_MENOS ATRIB_VEZES ATRIB_DIVISAO ATRIB_MODULO
%token ATRIB_AND ATRIB_OR ATRIB_XOR ATRIB_SHIFT_LEFT ATRIB_SHIFT_RIGHT
%token LPAREN RPAREN SEMI LBRACKET RBRACKET COMMA LKEY RKEY INTERROGACAO COLON
%token ERROR
%token INT VOID
%nonassoc RPAREN
%nonassoc ELSE

%% /* Gramática C- */

programa                : declaracao_lista
                            {
                                savedTree = insertIOFunctions();
                                savedTree->sibling->sibling = $1;
                            }
                        ;
declaracao_lista        : declaracao_lista declaracao
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
                        | declaracao { $$ = $1; }
                        ;
declaracao              : var_declaracao { $$ = $1; }
                        | fun_declaracao { $$ = $1; }
                        ;
var_declaracao          : tipo_especificador id SEMI
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
tipo_especificador      : INT
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
fun_declaracao          : tipo_especificador id LPAREN params RPAREN composto_decl
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
params                  : param_lista { $$ = $1; }
                        | VOID { $$ = NULL; }
                        ;
param_lista             : param_lista COMMA param
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
param                   : tipo_especificador id
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
                                $$->child[0]->kind.exp = VectorK;
                            }
                        ;
composto_decl           : LKEY local_declaracoes statement_lista RKEY
                            {
                                $$ = newStmtNode(CompK);
                                $$->child[0] = $2;
                                $$->child[1] = $3;
                            }
                        ;
local_declaracoes       : local_declaracoes var_declaracao
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
statement_lista         : statement_lista statement
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
statement               : expressao_decl { $$ = $1; }
                        | composto_decl { $$ = $1; }
                        | selecao_decl { $$ = $1; }
                        | iteracao_decl { $$ = $1; }
                        | retorno_decl { $$ = $1; }
                        ;
expressao_decl          : expressao SEMI { $$ = $1; }
                        | SEMI { $$ = NULL; }
                        ;
selecao_decl            : IF LPAREN expressao RPAREN statement
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
iteracao_decl           : WHILE LPAREN expressao RPAREN statement
                            {
                            	$$ = newStmtNode(WhileK);
                            	$$->child[0] = $3;
                            	$$->child[1] = $5;
                            }
                        ;
retorno_decl            : RETURN SEMI
                            {
                            	$$ = newStmtNode(ReturnK);
                            }
                        | RETURN expressao SEMI
                            {
                            	$$ = newStmtNode(ReturnK);
                            	$$->child[0] = $2;
                            }
                        ;
expressao               : var atribuicao_operador expressao
                            {
                                $$ = $2;
                                $$->child[0] = $1;
                            	$$->child[1] = $3;
                            }
                        | condicional_expressao { $$ = $1; }
                        ;
atribuicao_operador     : ATRIBUICAO
                            {
                            	$$ = newExpNode(OpK);
                            	$$->attr.op = ATRIBUICAO;
                            }
                        | ATRIB_MAIS
                            {
                            	$$ = newExpNode(OpK);
                            	$$->attr.op = ATRIB_MAIS;
                            }
                        | ATRIB_MENOS
                            {
                            	$$ = newExpNode(OpK);
                            	$$->attr.op = ATRIB_MENOS;
                            }
                        | ATRIB_VEZES
                            {
                            	$$ = newExpNode(OpK);
                            	$$->attr.op = ATRIB_VEZES;
                            }
                        | ATRIB_DIVISAO
                            {
                            	$$ = newExpNode(OpK);
                            	$$->attr.op = ATRIB_DIVISAO;
                            }
                        | ATRIB_MODULO
                            {
                            	$$ = newExpNode(OpK);
                            	$$->attr.op = ATRIB_MODULO;
                            }
                        | ATRIB_AND
                            {
                            	$$ = newExpNode(OpK);
                            	$$->attr.op = ATRIB_AND;
                            }
                        | ATRIB_OR
                            {
                            	$$ = newExpNode(OpK);
                            	$$->attr.op = ATRIB_OR;
                            }
                        | ATRIB_XOR
                            {
                            	$$ = newExpNode(OpK);
                            	$$->attr.op = ATRIB_XOR;
                            }
                        | ATRIB_SHIFT_LEFT
                            {
                            	$$ = newExpNode(OpK);
                            	$$->attr.op = ATRIB_SHIFT_LEFT;
                            }
                        | ATRIB_SHIFT_RIGHT
                            {
                            	$$ = newExpNode(OpK);
                            	$$->attr.op = ATRIB_SHIFT_RIGHT;
                            }
                        ;
var                     : id
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
condicional_expressao   : or_logico_expressao INTERROGACAO expressao COLON condicional_expressao
                            {
                                $$ = newStmtNode(IfK);
                                $$->child[0] = $1;
                                $$->child[1] = $3;
                                $$->child[2] = $5;
                            }
                        | or_logico_expressao { $$ = $1; }
                        ;
or_logico_expressao     : or_logico_expressao LOGICAL_OR and_logico_expressao
                            {
                                $$ = newExpNode(OpK);
                                $$->attr.op = LOGICAL_OR;
                                $$->child[0] = $1;
                                $$->child[1] = $3;
                            }
                        | and_logico_expressao { $$ = $1; }
                        ;
and_logico_expressao    : and_logico_expressao LOGICAL_AND or_inclusivo_expressao
                            {
                                $$ = newExpNode(OpK);
                                $$->attr.op = LOGICAL_AND;
                                $$->child[0] = $1;
                                $$->child[1] = $3;
                            }
                        | or_inclusivo_expressao { $$ = $1; }
                        ;
or_inclusivo_expressao  : or_inclusivo_expressao OR or_exclusivo_expressao
                            {
                                $$ = newExpNode(OpK);
                                $$->attr.op = OR;
                                $$->child[0] = $1;
                                $$->child[1] = $3;
                            }
                        | or_exclusivo_expressao { $$ = $1; }
                        ;
or_exclusivo_expressao  : or_exclusivo_expressao XOR and_expressao
                            {
                                $$ = newExpNode(OpK);
                                $$->attr.op = XOR;
                                $$->child[0] = $1;
                                $$->child[1] = $3;
                            }
                        | and_expressao { $$ = $1; }
                        ;
and_expressao           : and_expressao AND igualdade_expressao
                            {
                                $$ = newExpNode(OpK);
                                $$->attr.op = AND;
                                $$->child[0] = $1;
                                $$->child[1] = $3;
                            }
                        | igualdade_expressao { $$ = $1; }
                        ;
igualdade_expressao     : igualdade_expressao igualdade_operador relacional_expressao
                            {
                                $$ = $2;
                                $$->child[0] = $1;
                                $$->child[1] = $3;
                            }
                        | relacional_expressao { $$ = $1; }
                        ;
igualdade_operador      : IGUAL
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
relacional_expressao    : relacional_expressao relacional_operador shift_expressao
                            {
                            	$$ = $2;
                            	$$->child[0] = $1;
                            	$$->child[1] = $3;
                            }
                        | shift_expressao { $$ = $1; }
                        ;
relacional_operador     : MENORIGUAL
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
                        ;
shift_expressao         : shift_expressao shift_operador soma_expressao
                            {
                                $$ = $2;
                                $$->child[0] = $1;
                                $$->child[1] = $3;
                            }
                        | soma_expressao { $$ = $1; }
                        ;
shift_operador          : SHIFT_LEFT
                            {
                                $$ = newExpNode(OpK);
                                $$->attr.op = SHIFT_LEFT;
                            }
                        | SHIFT_RIGHT
                            {
                                $$ = newExpNode(OpK);
                                $$->attr.op = SHIFT_RIGHT;
                            }
                        ;
soma_expressao          : soma_expressao soma_operador mult_expressao
                            {
                            	$$ = $2;
                            	$$->child[0] = $1;
                            	$$->child[1] = $3;
                            }
            			| mult_expressao { $$ = $1; }
            			;
soma_operador           : MAIS
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
mult_expressao          : mult_expressao mult_operador fator
                            {
                            	$$ = $2;
                            	$$->child[0] = $1;
                            	$$->child[1] = $3;
                            }
                        | fator { $$ = $1; }
                        ;
mult_operador           : VEZES
                            {
                            	$$ = newExpNode(OpK);
                            	$$->attr.op = VEZES;
                            }
                        | DIVISAO
                            {
                            	$$ = newExpNode(OpK);
                            	$$->attr.op = DIVISAO;
                            }
                        | MODULO
                            {
                                $$ = newExpNode(OpK);
                                $$->attr.op = MODULO;
                            }
                        ;
fator                   : LPAREN expressao RPAREN { $$ = $2; }
                        | var { $$ = $1; }
                        | ativacao { $$ = $1; }
                        | num { $$ = $1; }
                        ;
ativacao                : var LPAREN args RPAREN
                            {
                            	$$ = $1;
                            	$$->kind.exp = CallK;
                            	$$->child[0] = $3;
                            }
			            ;
args                    : arg_lista { $$ = $1; }
                        |   vazio { $$ = $1; }
                        ;
arg_lista               : arg_lista COMMA expressao
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
id                      : ID
                            {
                            	$$ = newExpNode(IdK);
                            	$$->attr.name = copyString(tokenString);
                                $$->varAccess = DECLARANDO;
                            }
                        ;
num                     : NUM
                			{
                				$$ = newExpNode(ConstK);
                				$$->attr.val = atoi(tokenString);
                			}
            			;
vazio                   : { $$ = NULL; }
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
