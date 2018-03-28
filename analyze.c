/****************************************************/
/* File: analyze.c                                  */
/* Implementacao do analisador semantico            */
/* Diego Wendel de Oliveira Ferreira		        */
/* 86774                               		        */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "util.h"
#include <string.h>

char * funcName;

/* Contador de posições para a memória de dados */
int location = 0;

/* Flag para verificar se a função main já foi declarada */
int mainDeclarada = FALSE;

/* Procedure traverse is a generic recursive
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc
 * in postorder to tree pointed to by t
 */
static void traverse( TreeNode * t,
               void (* preProc) (TreeNode *),
               void (* postProc) (TreeNode *) )
{ if (t != NULL)
  { preProc(t);
    { int i;
      for (i=0; i < MAXCHILDREN; i++)
        traverse(t->child[i],preProc,postProc);
    }
    postProc(t);
    traverse(t->sibling,preProc,postProc);
  }
}

static void declError(TreeNode * t, const char * message) {
    fprintf(listing, "Erro de declaração na linha %d: %s\n", t->lineno, message);
    Error = TRUE;
}

static void varError(TreeNode * t, const char * message) {
    fprintf(listing, "Erro de variável na linha %d: %s\n", t->lineno, message);
    Error = TRUE;
}

/* Procedure insertNode inserts
 * identifiers stored in t into
 * the symbol table
 */
static void insertNode(TreeNode * t) {
    BucketList l;
    if (t->node == VARK) {
        switch (t->kind.var.varKind) {
            case IDK:
                if(t->kind.var.acesso == ACCESSK) {
                    /* Verifica se existe alguma entrada com o respectivo nome da variável */
                    l = st_bucket(t->kind.var.attr.name);
                    /* Se retornar algum resultado então existe */
                    if (l != NULL) {
                        if(l->treeNode->kind.var.varKind == FUNCTIONK) {
                            varError(t, "Não é possível referenciar uma função como variável");
                            break;
                        } else {
                            st_add_lineno(t);
                            break;
                        }
                    }
                    /* Se chegou até aqui a variável está sendo acessada sem ser declarada previamente */
                    varError(t, "Variável não declarada no escopo");
                } else {
                    /* Verifica se existe alguma entrada com o respectivo nome da variável */
                    BucketList l = st_bucket(t->kind.var.attr.name);
                    /* Se retornar algum resultado então existe */
                    if(l != NULL) {
                        if (l->treeNode != NULL) {
                            /* Se o resultado for do tipo FUNCTIONK, já foi declarado como função */
                            if (l->treeNode->kind.var.varKind == FUNCTIONK) {
                                declError(t, "Nome da variável já é usado para declarar uma função");
                                break;
                            }
                            /* Se o resultado for do tipo IDK, verifica se já foi declarado como variável */
                            /* Verifica se o escopo da variável é igual ao escopo atual, se for igual
                             * vai para o else e declara erro, caso contrário, não faz nada no if para
                             * o procedimento st_insert() ser executado
                             */
                            if(strcmp(l->treeNode->kind.var.scope->funcName, sc_top()->funcName)) {
                                /* Do nothing */
                            } else {
                                declError(t, "Variável já declarada neste escopo");
                                break;
                            }
                        }
                    }
                    /* Se nenhum erro de declaração ocorrer e não existir o nome da variável,
                     * adiciona a variável na tabela de símbolos
                     */
                    st_insert(t->kind.var.attr.name, t->lineno, location++, t, 1);
                }
                break;
            case VECTORK:
                if(t->kind.var.acesso == ACCESSK) {
                    /* Verifica se existe alguma entrada com o respectivo nome da variável */
                    l = st_bucket(t->kind.var.attr.name);
                    /* Se retornar algum resultado então existe */
                    if (l != NULL) {
                        if(l->treeNode->kind.var.varKind == FUNCTIONK) {
                            varError(t, "Não é possível referenciar uma função como variável");
                            break;
                        } else {
                            st_add_lineno(t);
                            break;
                        }
                    }
                    /* Se chegou até aqui a variável está sendo acessada sem ser declarada previamente */
                    varError(t, "Variável não declarada no escopo");
                } else {
                    /* Verifica se existe alguma entrada com o respectivo nome da variável */
                    BucketList l = st_bucket(t->kind.var.attr.name);
                    /* Se retornar algum resultado então existe */
                    if(l != NULL) {
                        if (l->treeNode != NULL) {
                            /* Se o resultado for do tipo FunctionK, já foi declarado como função */
                            if (l->treeNode->kind.var.varKind == FUNCTIONK) {
                                declError(t, "Nome da variável já é usado para declarar uma função");
                                break;
                            }
                            /* Se o resultado for do tipo IdK, verifica se já foi declarado como variável */
                            /* Verifica se o escopo da variável é igual ao escopo atual, se for igual
                             * vai para o else e declara erro, caso contrário, não faz nada no if para
                             * o procedimento st_insert() ser executado
                             */
                            if(strcmp(l->treeNode->kind.var.scope->funcName, sc_top()->funcName)) {
                                /* Do nothing */
                            } else {
                                declError(t, "Variável já declarada neste escopo");
                                break;
                            }
                        }
                    }
                    /* Se nenhum erro de declaração ocorrer e não existir o nome da variável,
                     * adiciona a variável na tabela de símbolos
                     */
                    int tamanhoVetor = t->child[0] != NULL ? t->child[0]->kind.var.attr.val : 1;
                    st_insert(t->kind.var.attr.name, t->lineno, location++, t, tamanhoVetor);
                    // Pula n posições na memória, onde n é o tamanhoVetor
                    location += tamanhoVetor - 1;
                }
                break;
            case CONSTK:
                break;
            case FUNCTIONK:
                funcName = t->kind.var.attr.name;
                if(strcmp(funcName, "main") == 0) mainDeclarada = TRUE; // main sendo declarada
                if (st_lookup_func(funcName)) {
                    declError(t, "Função main já declarada");
                    break;
                }
                // Zera o location
                location = 0;
                sc_push(sc_create(funcName));
                st_insert_func(funcName, t->lineno, t);
                break;
            case CALLK:
                funcName = t->kind.var.attr.name;
                if (sys_lookup(funcName) != NULL) {
                    // Do nothing
                } else if (!st_lookup_func(funcName)) {
                    declError(t, "Função não declarada");
                } else {
                    BucketList l = st_bucket_func(funcName);
                    if (l->treeNode != NULL) {
                        t->type = l->treeNode->type;
                    }
                }
                break;
      	}
  	} else if (t->node == SYSK) {
        sys_insert(sys_create(toStringSysCall(t->kind.sys), t));
    }
}

static void afterInsertNode(TreeNode * t) {
	if (t->kind.var.varKind == FUNCTIONK) {
		if(t->kind.var.attr.name != NULL) {
			sc_pop();
        }
	}
}

/* Function buildSymtab constructs the symbol
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode * syntaxTree) {
	globalScope = sc_create((char *) "ESCOPO_GLOBAL");
	sc_push(globalScope);
	traverse(syntaxTree, insertNode, afterInsertNode);
	sc_pop();
    sys_free();
	if(mainDeclarada == FALSE) {
		fprintf(listing, "Erro de declaração: Função main não declarada\n");
    	return;
	}
 	if (TraceAnalyze) {
		fprintf(listing,"\nTabela de símbolos:\n");
    	printSymTab(listing);
  	}
}

static void typeError(TreeNode * t, const char * message) {
    fprintf(listing, "Erro de tipo na linha %d: %s\n", t->lineno, message);
    Error = TRUE;
}

static void beforeCheckNode(TreeNode * t) {
	if (t->node == EXPK) {
		if (t->kind.var.varKind == FUNCTIONK) {
			funcName = t->kind.var.attr.name;
		}
  	}
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */
static void checkNode(TreeNode * t) {
    if (t->node == STMTK) {
        switch (t->kind.stmt) {
            case INTEGERK: t->child[0]->type = INTEGER_TYPE; break;
            case VOIDK:
                if (t->child[0]->kind.var.varKind == IDK) {
                    typeError(t, "Variável não pode ser do tipo void");
                } else {
                    t->child[0]->type = VOID_TYPE;
                }
                break;
            case IFK: break;
            case WHILEK: break;
            case RETURNK: break;
            case COMPK: break;
        }
    } else if (t->node == EXPK) {
        switch (t->kind.exp) {
            case ATRIBK:
                if ((t->child[0]->type != INTEGER_TYPE) || (t->child[1]->type != INTEGER_TYPE)) {
                    typeError(t, "Atribuição inválida, era esperado um valor int e foi recebido void");
                } else  {
                    t->type = INTEGER_TYPE;
                }
                break;
            case RELK: t->type = VOID_TYPE; break;
            case ARITHK: t->type = INTEGER_TYPE; break;
            case LOGICK: t->type = VOID_TYPE; break;
            case UNARYK: t->type = INTEGER_TYPE; break;
        }
    } else if (t->node == VARK) {
        switch (t->kind.var.varKind) {
            case IDK: t->type = INTEGER_TYPE; break;
            case VECTORK: t->type = INTEGER_TYPE; break;
            case CONSTK: t->type = INTEGER_TYPE; break;
            case FUNCTIONK: break;
            case CALLK: break;
        }
    }
}

/* Procedure typeCheck performs type checking
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode * syntaxTree) {
	traverse(syntaxTree, beforeCheckNode, checkNode);
}
