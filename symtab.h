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
    int memloc ; /* Endereço de memória da variável */
    struct BucketListRec * next;
} * BucketList;

typedef struct ScopeRec {
    char * funcName;
    struct ScopeRec * parent;
    BucketList hashTable[SIZE]; /* the hash table */
} * Scope;

Scope globalScope;

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert(char * name, int lineno, int loc, TreeNode * treeNode);

BucketList st_create(char * name, int lineno, int loc, TreeNode * treeNode);

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

int getMemoryLocation(char * nome, Scope escopo);

/* Procedure printSymTab prints a formatted
 * listing of the symbol table contents
 * to the listing file
 */
void printSymTab(FILE * listing);

/*
 * getQuantidadeParams retorna a quantidade de parâmetros da
 * função
 */
int getQuantidadeParams(TreeNode * treeNode);

int getQuantidadeArgumentos(TreeNode * treeNode);

#endif
