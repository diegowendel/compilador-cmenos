/****************************************************/
/* Arquivo: target.h                                */
/* Gerador de código objeto                         */
/* Diego Wendel de Oliveira Ferreira		        */
/****************************************************/

#ifndef _TARGET_H_
#define _TARGET_H_

#include "cgen.h"

typedef enum opcode {
    _ADDI, _SUBI, _MULI, _DIVI, _MODI,
    _ANDI, _ORI, _XORI, _NOT, _LANDI, _LORI,
    _SLLI, _SRLI,
    _MOV, _LW, _LI, _LA, _SW,
    _IN, _OUT,
    _JF,
    _J, _JAL, _HALT,
    _RTYPE
} Opcode;

typedef enum function {
    _ADD, _SUB, _MUL, _DIV, _MOD,
    _AND, _OR, _XOR, _LAND, _LOR,
    _SLL, _SRL,
    _EQ, _NE, _LT, _LET, _GT, _GET,
    _JR,
    _DONT_CARE
} Function;

typedef enum type {
    TYPE_R, TYPE_I, TYPE_J
} Type;

typedef enum registerName {
    $rz, $v0, $v1, $out, $inv, $gp, $a0, $a1,
    $a2, $a3, $s0, $s1, $s2, $s3, $s4, $s5,
    $s6, $s7, $s8, $s9, $t0, $t1, $t2, $t3,
    $t4, $t5, $t6, $t7, $t8, $t9, $sp, $ra
} RegisterName;

typedef enum addressingType {
    IMEDIATO, REGISTRADOR, INDEXADO, LABEL
} AddressingType;

typedef struct instOperand {
    union {
        int imediato;
        RegisterName registrador;
        struct {
            RegisterName registrador;
            int offset;
        } indexado;
        char * label;
    } enderecamento;
    AddressingType tipoEnderecamento;
} * InstOperand;

typedef struct escopo {
    int argRegCount;
    int savedRegCount;
    int tempRegCount;
    int quantidadeParametros;
    int quantidadeVariaveis;
    int tamanhoBlocoMemoria;
    const char * nome;
    struct registrador * regList;
    struct escopo * next;
} * Escopo;

typedef struct registrador {
    Operand op;
    InstOperand instOperand;
    struct registrador * next;
} * Registrador;

typedef struct objeto {
    Opcode opcode;
    Function func;
    Type type;
    InstOperand op1;
    InstOperand op2;
    InstOperand op3;
    struct objeto * next;
} * Objeto;

typedef struct label {
    char * nome;
    int linha;
    struct label * next;
} * Label;

Escopo createEscopo(const char *);

void pushEscopo(Escopo eg);

Registrador createRegistrador(Operand op, InstOperand instOperand);

void insertRegistrador(Registrador r);

void removeRegistrador(RegisterName name);

Registrador getRegistrador(RegisterName name);

InstOperand getRegByName(char * name);

void saveRegistradores(void);

void recuperaRegistradores(void);

void geraCodigoObjeto(Quadruple q);

void printCode(Objeto instrucao);

Objeto createObjInstTypeR(Opcode opcode, Function func, Type type, InstOperand op1, InstOperand op2, InstOperand op3);

Objeto createObjInst(Opcode opcode, Type type, InstOperand op1, InstOperand op2, InstOperand op3);

Objeto insertObjInst(Objeto obj);

Objeto getCodigoObjeto(void);

void preparaRegistradoresEspeciais(void);

InstOperand getImediato(int valor);

InstOperand getOperandLabel(char * name);

Label createLabel(char * nome, int linha);

void insertLabel(char * nome, int linha);

int getLinhaLabel(char * nome);

#endif
