/****************************************************/
/* Arquivo: target.h                                */
/* Gerador de código objeto                         */
/* Diego Wendel de Oliveira Ferreira		        */
/****************************************************/

#ifndef _TARGET_H_
#define _TARGET_H_

#include "cgen.h"

typedef enum opcode {_ADD, _ADDI, _SUB, _SUBI, _MULT, _MULTI, _DIV, _DIVI,
      _AND, _ANDI, _OR, _ORI, _XOR, _XORI, _NOT,
      _SL, _SR,
      _MOV, _LOAD, _LOADI, _LOADA, _STORE,
      _BEQ, _BNE, _BLT, _BLET, _BGT, _BGET, _JUMP, _JUMPAL, _JUMPR,
      _NOP, _HALT, _RESET,
      _IN, _OUT} Opcode;

/**
 * opcode_map mapeia todas posições dos enums de opcode, é utilizado para
 * encontrar opcodes em tempo de execução
 */
static const enum opcode opcode_map[] = {_ADD, _ADDI, _SUB, _SUBI, _MULT, _MULTI, _DIV, _DIVI,
      _AND, _ANDI, _OR, _ORI, _XOR, _XORI, _NOT,
      _SL, _SR,
      _MOV, _LOAD, _LOADI, _LOADA, _STORE,
      _BEQ, _BNE, _BLT, _BLET, _BGT, _BGET, _JUMP, _JUMPAL, _JUMPR,
      _NOP, _HALT, _RESET,
      _IN, _OUT};

typedef struct escopoGerador {
    int argRegCount;
    int savedRegCount;
    int tempRegCount;
    int quantidadeParametros;
    int quantidadeVariaveis;
    int tamanhoBlocoMemoria;
    const char * nome;
    struct registrador * regList;
    struct escopoGerador * next;
} * EscopoGerador;

typedef struct registrador {
    Operand op;
    char * regName;
    struct registrador * next;
} * Registrador;

typedef struct objeto {
    Opcode opcode;
    const char * op1;
    const char * op2;
    const char * op3;
    struct objeto * next;
} * Objeto;

const char * toStringOpcode(enum opcode op);

EscopoGerador createEscopoGerador(const char *);

void pushEscopoGerador(EscopoGerador eg);

void popEscopoGerador();

Registrador createRegistrador(Operand op, char *);

void insertRegistrador(Registrador r);

void removeRegistrador(char * name);

Registrador getRegistrador(char * name);

void moveRegistrador(char * dest, char * orig);

char * getRegName(char * name);

void geraCodigoObjeto(Quadruple q);

void printCode(Objeto instrucao);

Objeto createObjInst(Opcode opcode, const char * op1, const char * op2, const char * op3);

Objeto insertObjInst(Objeto obj);

Objeto getCodigoObjeto(void);

#endif
