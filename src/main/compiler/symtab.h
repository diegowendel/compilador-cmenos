/****************************************************/
/* File: symtab.h                                   */
/* Interface para a tabela de simbolos C minus      */
/* Diego Wendel de Oliveira Ferreira		    */
/* 86774                               		    */
/****************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"

/* SIZE is the size of the hash table */
#define SIZE 211

/* the list of line numbers of the source
 * code in which a variable is referenced
 */
typedef struct LineListRec {
    int lineno;
    struct LineListRec * next;
} * LineList;

/* The record in the bucket lists for
 * each variable, including name,
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code
 */
typedef struct BucketListRec {
    char * name;
    LineList lines;
    TreeNode * treeNode;
    int memloc; /* Endereço de memória da variável */
    int tamanho; /* Tamanho da variável (útil para vetores) */
    struct BucketListRec * next;
} * BucketList;

typedef struct ScopeRec {
    char * funcName;
    struct ScopeRec * parent;
    int tamanhoBlocoMemoria; /* Tamanho do bloco de memória alocado */
    BucketList hashTable[SIZE]; /* the hash table */
} * Scope;

Scope globalScope;

typedef struct SysCallRec {
    char * name;
    TreeNode * treeNode;
    struct SysCallRec * next;
} * SysCall;

SysCall syscallHead;

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
SysCall sys_create(char * name, TreeNode * treeNode);

void sys_insert(SysCall syscall);

SysCall sys_lookup(char * name);

void sys_free(void);

void st_insert(char * name, int lineno, int loc, TreeNode * treeNode, int tamanho);

BucketList st_create(char * name, int lineno, int loc, TreeNode * treeNode, int tamanho);

void st_insert_func(char * name, int lineno, TreeNode * treeNode);

int verifyGlobalScope(TreeNode * treeNode);

/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */
int st_lookup (char * name);

BucketList st_bucket(char * name);

BucketList st_bucket_func (char * name);

void st_add_lineno(TreeNode * treeNode);

void incScope();

int st_lookup_top (char * name);

Scope sc_create(char * funcName);

Scope sc_top(void);

void sc_pop(void);

void sc_push(Scope scope);

int st_lookup_func (char * name);

BucketList getVarFromSymtab(char * nome, Scope escopo);

int getMemoryLocation(char * nome, Scope escopo);

/* Procedure printSymTab prints a formatted
 * listing of the symbol table contents
 * to the listing file
 */
void printSymTab(FILE * listing);

/*
 * getQuantidadeParametros retorna a quantidade de parâmetros da função
 *
 * @functionNode é obrigatoriamente o nó da árvore que representa a função
 */
int getQuantidadeParametros(TreeNode * functionNode);

/*
 * getQuantidadeVariaveis retorna a quantidade de variáveis declaradas no escopo da função
 *
 * @functionNode é obrigatoriamente o nó da árvore que representa a função
 */
int getQuantidadeVariaveis(TreeNode * functionNode);

/*
 * getTamanhoBlocoMemoriaEscopo retorna o tamanho do bloco de memória que deve ser alocado na stack
 * para o escopo
 *
 * @scopeName é obrigatoriamente o nome do escopo
 */
int getTamanhoBlocoMemoriaEscopo(char * scopeName);

int getTamanhoBlocoMemoriaEscopoGlobal(void);

#endif
