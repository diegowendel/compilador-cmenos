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
      _LOAD, _STORE,
      _BEQ, _BNE, _BLT, _BLET, _BGT, _BGET, _JUMP,
      _NOP, _HALT, _RESET,
      _IN, _OUT} Opcode;

const char * toStringOpcode(enum opcode op);

const char * toStringBinary(enum opcode op);

void geraCodigoObjeto(Quadruple codigoIntermediario, char * codefile);

#endif
