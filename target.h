/****************************************************/
/* Arquivo: target.h                                */
/* Gerador de código objeto                         */
/* Diego Wendel de Oliveira Ferreira		        */
/****************************************************/

#ifndef _TARGET_H_
#define _TARGET_H_

#include "cgen.h"

typedef enum opcode {_ADD, _ADDI, _SUB, _SUBI, _MULT, _MULTI, _DIV, _DIVI,
      _VEC,
      _AND, _ANDI, _OR, _ORI, _XOR, _XORI, _NOT,
      _SL, _SR,
      _MOV, _MOVI,
      _LOAD, _LOADI, _POP, _STORE, _PUSH,
      _BEQ, _BNE, _BLT, _BLET, _BGT, _BGET, _JUMP,
      _FUNC, _CALL, _NOP, _HALT, _RESET,
      _IN, _OUT} Opcode;

typedef enum tipoInstucao {
    R_TYPE,
    I_TYPE,
    J_TYPE
} TipoInstucao;

typedef struct objeto {
    const char * opcode;
    const char * op1;
    const char * op2;
    const char * op3;
    struct objeto * next;
} * Objeto;

const char * toStringOpcode(enum opcode op);

void geraCodigoObjeto(Quadruple q);

void printCode(Objeto instrucao);

Objeto createObjectInstruction(const char * opcode, const char * op1, const char * op2, const char * op3);

#endif
