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

static char * funcName;

/* Contador de posições para a memória de dados */
static int location = 0;

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

static int mainDeclarada = 0;

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
    if (t->nodekind == ExpK) {
        switch (t->kind.exp) {
            case IdK:
                if(t->varAccess == ACESSANDO) {
                    /* Verifica se existe alguma entrada com o respectivo nome da variável */
                    l = st_bucket(t->attr.name);
                    /* Se retornar algum resultado então existe */
                    if (l != NULL) {
                        if(l->treeNode->kind.exp == FunctionK) {
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
                    BucketList l = st_bucket(t->attr.name);
                    /* Se retornar algum resultado então existe */
                    if(l != NULL) {
                        if (l->treeNode != NULL) {
                            /* Se o resultado for do tipo FunctionK, já foi declarado como função */
                            if (l->treeNode->kind.exp == FunctionK) {
                                declError(t, "Nome da variável já é usado para declarar uma função");
                                break;
                            } /* Se o resultado for do tipo IdK, verifica se já foi declarado como variável */
                            /* Verifica se o escopo da variável é igual ao escopo atual, se for igual
                            * vai para o else e declara erro, caso contrário, não faz nada no if para
                            * o procedimento st_insert() ser executado
                            */
                            if(strcmp(l->treeNode->scope->funcName, sc_top()->funcName)) {
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
                    st_insert(t->attr.name, t->lineno, location++, t, 1);
                }
				break;
			case VectorK:
                if(t->varAccess == ACESSANDO) {
                    /* Verifica se existe alguma entrada com o respectivo nome da variável */
                    l = st_bucket(t->attr.name);
                    /* Se retornar algum resultado então existe */
                    if (l != NULL) {
                        if(l->treeNode->kind.exp == FunctionK) {
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
                    BucketList l = st_bucket(t->attr.name);
                    /* Se retornar algum resultado então existe */
                    if(l != NULL) {
                        if (l->treeNode != NULL) {
                            /* Se o resultado for do tipo FunctionK, já foi declarado como função */
                            if (l->treeNode->kind.exp == FunctionK) {
                                declError(t, "Nome da variável já é usado para declarar uma função");
                                break;
                            } /* Se o resultado for do tipo IdK, verifica se já foi declarado como variável */
                            /* Verifica se o escopo da variável é igual ao escopo atual, se for igual
                            * vai para o else e declara erro, caso contrário, não faz nada no if para
                            * o procedimento st_insert() ser executado
                            */
                            if(strcmp(l->treeNode->scope->funcName, sc_top()->funcName)) {
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
                    int tamanhoVetor = t->child[0]->attr.val;
                    st_insert(t->attr.name, t->lineno, location++, t, tamanhoVetor);
                    // Pula n posições na memória, onde n é o tamanhoVetor
                    location += tamanhoVetor - 1;
                }
				break;
			case CallK:
				if (!st_lookup_func(t->attr.name)) {
					declError(t, "Função não declarada");
				} else {
					BucketList l = st_bucket_func(t->attr.name);
					if (l->treeNode != NULL) {
						t->type = l->treeNode->type;
					}
				}
				break;
			case FunctionK:
				funcName = t->attr.name;
				if(strcmp(funcName, "main") == 0) mainDeclarada = 1; // main sendo declarada
        		if (st_lookup_func(funcName)) {
					declError(t, "Função já declarada");
					break;
        		}
                // Zera o location
                location = 0;
                sc_push(sc_create(funcName));
				st_insert_func(funcName, t->lineno, t);
				break;
       		default:
        		break;
      	}
  	}
}

static void afterInsertNode(TreeNode * t) {
	if (t->kind.exp == FunctionK) {
		if(t->attr.name != NULL) {
			sc_pop();
        }
	}
}

/* Function buildSymtab constructs the symbol
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode * syntaxTree) {
	globalScope = sc_create("ESCOPO_GLOBAL");
	sc_push(globalScope);
	traverse(syntaxTree, insertNode, afterInsertNode);
	sc_pop();
	if(mainDeclarada == 0) {
		fprintf(listing, "Erro de declaração: Função main não declarada\n");
    	return;
	}
 	if (TraceAnalyze) {
		fprintf(listing,"\n\nTabela de símbolos:\n");
    	printSymTab(listing);
  	}
}

static void typeError(TreeNode * t, const char * message) {
    fprintf(listing, "Erro de tipo na linha %d: %s\n", t->lineno, message);
    Error = TRUE;
}

static void beforeCheckNode(TreeNode * t) {
	if (t->nodekind == ExpK) {
		if (t->kind.exp == FunctionK) {
			funcName = t->attr.name;
		}
  	}
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */
static void checkNode(TreeNode * t) {
	switch (t->nodekind) {
		case ExpK:
      		switch (t->kind.exp) {
				case OpK:
					if (t->attr.op == ATRIBUICAO) {
						if ((t->child[0]->type != Integer) ||
							(t->child[1]->type != Integer)) {
							typeError(t,"Atribuição inválida");
						} else  {
							t->type = Integer;
						}
					} else if (	(t->attr.op == MAIS) ||
							(t->attr.op == MENOS) ||
							(t->attr.op == VEZES) ||
							(t->attr.op == DIVISAO)) {
						t->type = Integer;
					} else {
						t->type = Void;
					}
					break;
        		case ConstK:
					t->type = Integer;
					break;
				case IdK:
					t->type = Integer;
					break;
				case VectorK:
					t->type = Integer;
					break;
				case CallK:
          			break;
				case FunctionK:
					break;
       			default:
       				break;
      	}
      	break;
    	case StmtK:
      		switch (t->kind.stmt) {
				case CompK:
					break;
				case IntegerK:
					t->child[0]->type = Integer;
					break;
				case VoidK:
					if (t->child[0]->kind.exp == IdK) {
						typeError(t,"Variável não pode ser do tipo Void");
					} else {
						t->child[0]->type = Void;
					}
					break;
				case IfK:
					break;
				case ElseK:
					break;
				case ReturnK:
					break;
				case WhileK:
					break;
        		default:
        			break;
     		}
      		break;
    	default:
      		break;
  	}
}

/* Procedure typeCheck performs type checking
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode * syntaxTree) {
	traverse(syntaxTree, beforeCheckNode, checkNode);
}
