/****************************************************/
/* Arquivo: binary.c                                */
/* Implementação do Gerador de código binário       */
/* Diego Wendel de Oliveira Ferreira		        */
/****************************************************/

#include "target.h"
#include "code.h"

char temp[100];

const char * toBinaryOpcode(Opcode op) {
    const char * strings[] = {
        // "add", "addi", "sub", "subi", "mul", "muli", "div", "divi",
        "000001", "000010", "000011", "000100", "000101", "000110", "000111", "001000",
        // "and", "andi", "or", "ori", "xor", "xori", "not",
        "001001", "001010", "001011", "001100", "001101", "001110", "001111",
        // "sl", "sr",
        "010000", "010001",
        // "mov", "lw", "li", "la", "sw",
        "010010", "010011", "010100", "010101", "010110",
        // "beq", "bne", "blt", "blet", "bgt", "bget", "j", "jal", "jr",
        "010111", "011000", "011001", "011010", "011011", "011100", "011101", "011110", "011111",
        // "nop", "halt", "reset",
        "000000", "111111", "111110",
        // "in", "out"
        "100000", "100001"
    };
    return strings[op];
}

void geraCodigoBinario(Objeto codigoObjeto) {
    emitCode("\n********** Código binário **********\n");
    Objeto obj = codigoObjeto;

    // TODO jump to main, first instruction, need to be made

    while(obj != NULL) {
        // Limpa o vetor de caracteres auxiliar
        memset(temp, '\0', sizeof(temp));
        // Traduz o opcode para binário
        strcat(temp, toBinaryOpcode(obj->opcode));
        strcat(temp, "\t");
        if(obj->op1 != NULL) {
            strcat(temp, obj->op1);
        }
        if(obj->op2 != NULL) {
            strcat(temp, ", ");
            strcat(temp, obj->op2);
        }
        if(obj->op3 != NULL) {
            strcat(temp, ", ");
            strcat(temp, obj->op3);
        }
        emitCode(temp);
        obj = obj->next;
    }
}
