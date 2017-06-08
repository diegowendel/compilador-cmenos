/****************************************************/
/* File: cgen.h                                     */
/* The code generator interface to the TINY compiler*/
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"

#ifndef _CGEN_H_
#define _CGEN_H_

typedef enum {Empty, IntConst, String} OperandKind;

typedef enum instrucao {ADD, SUB, MULT, DIV, VEC,
      EQ, NE, LT, LET, GT, GET, ASN,
      FUNC, RTN, GET_PARAM, SET_PARAM, CALL, PARAM_LIST,
      JPF, GOTO, LBL, HALT} InstructionKind;

typedef struct  {
    OperandKind kind;
    union {
        int val;
        struct {
            char * name;
            struct ScopeRec * scope;
        } variable;
    } contents;
} Operand;

/* Estrutura Quádrupla que armazena os dados do código
 * de três endereços
 */
typedef struct Quad {
    InstructionKind instruction;
    Operand op1, op2, op3;
    int linha;
    struct Quad * next;
} * Quadruple;

typedef struct Location {
    Quadruple * quad;
    struct Location * next;
} * LocationStack;

typedef struct Param {
    int * count;
    struct Param * next;
} * ParamStack;

Quadruple * insertQuad(Quadruple q);

Quadruple createQuad(InstructionKind instruction, Operand op1, Operand op2, Operand op3);

void pushLocation(LocationStack ls);

void popLocation();

LocationStack createLocation(Quadruple * quad);

void updateLocation(Operand op);

void pushParam(int * count);

void popParam();

void preparaVazio();

void printIntermediateCode();

Quadruple getCodigoIntermediario(void);

/* Procedure codeGen generates code to a code
 * file by traversal of the syntax tree. The
 * second parameter (codefile) is the file name
 * of the code file, and is used to print the
 * file name as a comment in the code file
 */
void codeGen(TreeNode * syntaxTree, char * codefile);

#endif
