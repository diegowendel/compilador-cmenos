/****************************************************/
/* File: cgen.h                                     */
/* The code generator interface to the TINY compiler*/
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"

#ifndef _CGEN_H_
#define _CGEN_H_

typedef enum {IntConst, String} OperandKind;

typedef enum instrucao {ADD, SUB, MULT, DIV, MOD,
    BITW_AND, BITW_OR, BITW_XOR, BITW_NOT, LOGIC_AND, LOGIC_OR,
    SHFT_LF, SHFT_RT, VEC, VEC_ADDR,
    EQ, NE, LT, LET, GT, GET, ASN,
    FUNC, RTN, GET_PARAM, SET_PARAM, CALL, PARAM_LIST,
    JPF, GOTO, LBL, SYSCALL, HALT} InstructionKind;

typedef struct operand {
    OperandKind kind;
    union {
        int val;
        struct {
            char * name;
            struct ScopeRec * scope;
        } variable;
    } contents;
} * Operand;

/* Estrutura Quádrupla que armazena os dados do código
 * de três endereços
 */
typedef struct Quad {
    InstructionKind instruction;
    int linha;
    int display;
    int offset;
    Operand op1;
    Operand op2;
    Operand op3;
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

Operand createOperand(void);

Quadruple * insertQuad(Quadruple q);

Quadruple createQuad(InstructionKind instruction, Operand op1, Operand op2, Operand op3);

void pushLocation(Quadruple * quad);

void popLocation();

void updateLocation(Operand op);

void pushParam(int * count);

void popParam();

void printIntermediateCode();

Quadruple getCodigoIntermediario(void);

/**
 * Libera memória usada para montar o código intermediário
 */
void freeCodigoIntermediario(void);

/* Procedure codeGen generates code to a code
 * file by traversal of the syntax tree. The
 * second parameter (codefile) is the file name
 * of the code file, and is used to print the
 * file name as a comment in the code file
 */
void codeGen(TreeNode * syntaxTree, char * codefile, CodeInfo codeInfo);

void verificaFimInstrucaoAnterior(void);

#endif
