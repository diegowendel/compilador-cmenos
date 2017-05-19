/****************************************************/
/* File: symtab.c                                   */
/* Implementacao da tabela de simbolos C minus      */
/* A tabela de simbolos eh implementada como uma    */
/* tabela hash com encadeamento                     */
/* Diego Wendel de Oliveira Ferreira		    */
/* 86774                               		    */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "symtab.h"

/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

#define MAX_SCOPE 1000

#define ESCOPO_GLOBAL 0
#define ESCOPO_NAO_GLOBAL 1

static Scope scopes[MAX_SCOPE];
static int nScope = 0;
static Scope scopeStack[MAX_SCOPE];
static int nScopeStack = 0;

/* the hash function */
static int hash (char * key) {
    int temp = 0;
    int i = 0;
    while (key[i] != '\0') {
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        ++i;
    }
    return temp;
}

/* Função toString do enum ExpType */
const char * expTypeToString(enum exp e) {
    const char * strings[] = {"Void", "Inteiro"};
    return strings[e];
}

Scope sc_top(void) {
    return scopeStack[nScopeStack - 1];
}

void sc_pop(void) {
    //printf("pop %s\n", sc_top()->funcName);
    --nScopeStack;
}

void sc_push(Scope scope) {
    /* Se scope não for o escopo global,
     * adiciona o último escopo como escopo pai do mesmo
     */
    if(nScopeStack) {
        scope->parent = sc_top();
    }
    scopeStack[nScopeStack++] = scope;
}

Scope sc_create(char * funcName) {
    Scope newScope;

    newScope = (Scope) malloc(sizeof(struct ScopeRec));
    newScope->funcName = funcName;
    newScope->nestedLevel = nScopeStack;
    newScope->parent = sc_top();

    scopes[nScope++] = newScope;

    return newScope;
}

BucketList st_bucket(char * name) {
    int h = hash(name);
    Scope sc = sc_top();
    while(sc) {
        BucketList l = sc->hashTable[h];
        while ((l != NULL) && (strcmp(name,l->name) != 0)) {
            l = l->next;
        }
        if (l != NULL) return l;
        sc = sc->parent;
    }
    return NULL;
}

int getMemoryLocation(TreeNode * treeNode) {
    Scope escopo = treeNode->scope;
    int h = hash(treeNode->attr.name);

    while (escopo != NULL) {
        BucketList l = escopo->hashTable[h];

        while (l != NULL) {
            /* !strcmp(const char *s1, const char *s2) verifica se as duas
             * strings passadas como parâmetro são iguais. Retorna 0 em caso
             * verdadeiro, por isso o símbolo '!' antes da função
             */
             // Verifica primeiro se o escopo é igual
            if(!strcmp(escopo->funcName, treeNode->scope->funcName)) {
                // Se o escopo for igual verifica o nome da variável
                if(!strcmp(l->name, treeNode->attr.name)) {
                    return l->memloc;
                }
            }
            l = l->next;
        }
        escopo = escopo->parent;
    }
    return -1;
}

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert(char * name, int lineno, int loc, TreeNode * treeNode) {
	int h = hash(name);
	Scope top = sc_top();
  	BucketList l = top->hashTable[h];

  	while ((l != NULL) && (strcmp(name,l->name) != 0)) {
    	l = l->next;
    }
  	if (l == NULL) { /* Variável ainda não existente na tabela */
        // Adiciona o escopo ao nó da árvore sintática
        treeNode->scope = top;
        // Adiciona um novo item na tabela de símbolos
  		l = (BucketList) malloc(sizeof(struct BucketListRec));
    	l->name = name;
    	l->lines = (LineList) malloc(sizeof(struct LineListRec));
	    l->lines->lineno = lineno;
        l->lines->next = NULL;
        l->memloc = loc;
        l->treeNode = treeNode;
	    l->next = top->hashTable[h];
	   	top->hashTable[h] = l;
	} else { /* Variável encontrada na tabela, só adiciona o número da linha */
        LineList ll = l->lines;
        while(ll->next != NULL) {
            ll = ll->next;
        }
        ll->next = (LineList) malloc(sizeof(struct LineListRec));
        ll->next->lineno = lineno;
        ll->next->next = NULL;
    }
} /* st_insert */

/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */
int st_lookup (char * name) {
    BucketList l = st_bucket(name);
    if (l != NULL) return l->memloc;
    return -1;
}

int st_lookup_func (char * name ) {
	int h = hash(name);
	Scope sc = scopes[ESCOPO_GLOBAL];
	BucketList l = sc->hashTable[h];
	while ((l != NULL) && (strcmp(name,l->name) != 0)) {
        l = l->next;
    }
    if (l != NULL)  return TRUE;
	else   return FALSE;
}

int st_lookup_top (char * name) {
    int h = hash(name);
    Scope sc = sc_top();
    while(sc) {
        BucketList l = sc->hashTable[h];
        while ((l != NULL) && (strcmp(name,l->name) != 0)) {
            l = l->next;
        }
        if (l != NULL) return l->memloc;
        break;
    }
    return -1;
}

void st_add_lineno(char * name, int lineno) {
	BucketList l = st_bucket(name);
  	LineList ll = l->lines;
  	while (ll->next != NULL) {
		ll = ll->next;
    }
	if (ll->lineno != lineno) {
	 	ll->next = (LineList) malloc(sizeof(struct LineListRec));
	  	ll->next->lineno = lineno;
	  	ll->next->next = NULL;
	}
}

void printSymTabRows(BucketList *hashTable, FILE *listing, int escopo) {
	int j;
	for (j = 0; j < SIZE; ++j) {
		if (hashTable[j] != NULL) {
			BucketList l = hashTable[j];

      		while (l != NULL) {
				LineList t = l->lines;
        		fprintf(listing, "%-18s", l->name);
				fprintf(listing, "%-10s", expTypeToString(l->treeNode->type));
                if(strcmp(l->treeNode->typeVar, "Vetor") == 0) {
                    fprintf(listing, "%-12s", l->treeNode->typeVar);
                } else {
                    fprintf(listing, "%-13s", l->treeNode->typeVar);
                }

                /*
                 * Verifica se o item armazenado nessa posição da tabela de
                 * símbolos é uma função. Se for, imprime também o número de
                 * parâmetros
                 */
                if(escopo == ESCOPO_GLOBAL) {
                    if(strcmp("Função", l->treeNode->typeVar) == 0) {
                        int numParams = getQuantidadeParams(l->treeNode->child[0]);
                        fprintf(listing, " %-15d", numParams);

                        if(l->treeNode->child[0] == NULL) {
                            fprintf(listing, "%-17s", "void");
                        } else {
                            fprintf(listing, "%-17s", "int");
                        }
                    } else {
                        fprintf(listing, "%-32s", "");
                    }
                }

                if(l->memloc == -1) {
                    fprintf(listing, "%-8c", '-');
                } else {
                    fprintf(listing, "%-8d", l->memloc);
                }

                while (t != NULL) {
					if (t->lineno != -1) {
						fprintf(listing, "%-4d", t->lineno);
					} else {
						fprintf(listing, " ");
					}
          			t = t->next;
        		}
                fprintf(listing, "\n");
       			l = l->next;
     		}
   		}
  	}
}

int getQuantidadeParams(TreeNode * treeNode) {
    int qtd = 0;
    if(treeNode != NULL) {
        ++qtd;
        while(treeNode->sibling != NULL) {
            treeNode = treeNode->sibling;
            ++qtd;
        }
    }
    return qtd;
}

int getQuantidadeArgumentos(TreeNode * treeNode) {
    int qtd = 0;
    TreeNode * temp = treeNode->child[0];
    if(temp != NULL) {
        ++qtd;
        while(temp->sibling != NULL) {
            temp = temp->sibling;
            ++qtd;
        }
    }
    return qtd;
}

/* Procedure printSymTab prints a formatted
 * listing of the symbol table contents
 * to the listing file
 */
void printSymTab(FILE * listing) {
	int i;
	for (i = 0; i < nScope; ++i) {
		Scope scope = scopes[i];
		BucketList * hashTable = scope->hashTable;

		if (i == 0) { // Escopo global
			fprintf(listing, "<Escopo Global>\n");
            fprintf(listing, "Nome da variavel  Tipo ID   Tipo dados  Nº parametros  Tipo parametros  MemLoC  Numero das linhas\n");
      		fprintf(listing, "----------------  --------  ----------  -------------  ---------------  ------  -----------------\n");
            printSymTabRows(hashTable, listing, ESCOPO_GLOBAL);
		} else {
			fprintf(listing, "Nome da função: %s\n", scope->funcName);
            fprintf(listing, "Nome da variavel  Tipo ID   Tipo dados  MemLoc  Numero das linhas\n");
      		fprintf(listing, "----------------  --------  ----------  ------  -----------------\n");
            printSymTabRows(hashTable, listing, ESCOPO_NAO_GLOBAL);
		}

		fputc('\n', listing);
	}
} /* printSymTab */
