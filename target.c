/****************************************************/
/* Arquivo: target.h                                */
/* Implementação do Gerador de código objeto        */
/* Diego Wendel de Oliveira Ferreira		        */
/****************************************************/

#include "target.h"
#include "globals.h"
#include "cgen.h"

const char * toStringOpcode(enum opcode op) {
    const char * strings[] = {
        "ADD", "ADDI", "SUB", "SUBI", "MULT", "MULTI", "DIV", "DIVI",
        "VEC",
        "AND", "ANDI", "OR", "ORI", "XOR", "XORI", "NOT",
        "SL", "SR",
        "MOV", "MOVI",
        "LOAD", "STORE",
        "BEQ", "BNE", "BLT", "BLET", "BGT", "BGET", "JUMP",
        "NOP", "HALT", "RESET",
        "IN", "OUT"
    };
    return strings[op];
}

const char * toStringBinary(enum opcode op) {
    const char * strings[] = {
        "000001", "000010", "000011", "000100"
        /* to be continued */
    };
    return strings[op];
}

void geraCodigoObjeto(Quadruple codigoIntermediario, char * codefile) {
    fprintf(code, "\n%s", "\n********** Código objeto **********\n");
    fprintf(code, "%s\n", "teste escrita targetcode");
}
