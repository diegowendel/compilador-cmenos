/****************************************************/
/* Arquivo: binary.c                                */
/* Implementação do Gerador de código binário       */
/* Diego Wendel de Oliveira Ferreira		        */
/****************************************************/

#include "target.h"
#include "code.h"
#include "util.h"

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

const char * toBinaryRegister(RegisterName rn) {
    const char * strings[] = {
        "00000", "00001", "00010", "00011", "00100", "00101", "00110", "00111",
        "01000", "01001", "01010", "01011", "01100", "01101", "01110", "01111",
        "10000", "10001", "10010", "10011", "10100", "10101", "10110", "10111",
        "11000", "11001", "11010", "11011", "11100", "11101", "11110", "11111"
    };
    return strings[rn];
}

const char * getZeros(int n) {
    int i = 0;
    char * zeros = (char *) malloc(n + 1);
    while(i < n) {
        zeros[i++] = '0';
    }
    zeros[i] = '\0';
    return zeros;
}

void geraCodigoBinario(Objeto codigoObjeto) {
    emitCode("\n********** Código binário **********\n");
    Objeto obj = codigoObjeto;
    char str[26];

    // TODO jump to main, first instruction, need to be made

    while(obj != NULL) {
        // Limpa o vetor de caracteres auxiliar
        memset(temp, '\0', sizeof(temp));
        // Traduz o opcode para binário
        strcat(temp, toBinaryOpcode(obj->opcode));

        switch(obj->type) {
            case TYPE_R:
                strcat(temp, toBinaryRegister(obj->op1->enderecamento.registrador));
                strcat(temp, toBinaryRegister(obj->op2->enderecamento.registrador));
                strcat(temp, toBinaryRegister(obj->op3->enderecamento.registrador));
                strcat(temp, getZeros(11));
                break;
            case TYPE_I:

                if(obj->op1->tipoEnderecamento == REGISTRADOR) {
                    strcat(temp, toBinaryRegister(obj->op1->enderecamento.registrador));
                }

                if(obj->op2 == NULL) {
                    fprintf(listing, "%s\n", toStringOpcode(obj->opcode));
                } else if(obj->op2->tipoEnderecamento == REGISTRADOR) {
                    strcat(temp, "      ");
                    strcat(temp, toBinaryRegister(obj->op2->enderecamento.registrador));
                }

                break;
            case TYPE_J:
                if(obj->opcode == _JUMP || obj->opcode == _JUMPAL) {
                    strcat(temp, "\t");
                    strcat(temp, itoa(getLinhaLabel(obj->op1->enderecamento.label), str, 2));
                } else if(obj->opcode == _JUMPR) {

                } else { // HALT
                    strcat(temp, getZeros(26));
                }
                break;
        }
        emitCode(temp);
        obj = obj->next;
    }
}
