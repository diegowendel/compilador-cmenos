/****************************************************/
/* Arquivo: binary.c                                */
/* Implementação do Gerador de código binário       */
/* Diego Wendel de Oliveira Ferreira		        */
/****************************************************/

#include "target.h"
#include "code.h"
#include "util.h"

char temp[100];

const char * getZeros(int n) {
    int i = 0;
    char * zeros = (char *) malloc(n + 1);
    while(i < n) {
        zeros[i++] = '0';
    }
    zeros[i] = '\0';
    return zeros;
}

/* Função que converte um número decimal para uma string binária em complemento
 * de 2. Essa função não trata casos de overflow!!! Fica a cargo do programador
 * alocar a quantidade de bits suficiente para a conversão
 */
const char * decimalToBinaryStr(unsigned x, int qtdBits) {
    int i = 0;
    qtdBits--;
    char * bin = (char *) malloc(qtdBits + 1);
    for (unsigned bit = 1u << qtdBits; bit != 0; bit >>= 1) {
        bin[i++] = (x & bit) ? '1' : '0';
    }
    bin[i] = '\0';
    return bin;
}

void geraCodigoBinario(Objeto codigoObjeto) {
    emitCode("\n********** Código binário **********\n");
    Objeto obj = codigoObjeto;
    char str[26];
    int linha = 0;

    // Limpa o vetor de caracteres auxiliar
    memset(temp, '\0', sizeof(temp));
    // Boilerplate
    strcat(temp, "assign rom[");
    sprintf(str, "%d", linha++);
    strcat(temp, str);
    strcat(temp, "] = 32'b");
    strcat(temp, toBinaryOpcode(_J));
    strcat(temp, "_");
    strcat(temp, decimalToBinaryStr(getLinhaLabel((char*) "main"), 26));
    strcat(temp, ";\t\t// Jump to Main");
    emitCode(temp);

    while(obj != NULL) {
        // Limpa o vetor de caracteres auxiliar
        memset(temp, '\0', sizeof(temp));
        // Boilerplate
        strcat(temp, "assign rom[");
        sprintf(str, "%d", linha++);
        strcat(temp, str);
        strcat(temp, "] = 32'b");

        // Traduz o opcode para binário
        strcat(temp, toBinaryOpcode(obj->opcode));
        strcat(temp, "_");

        switch(obj->type) {
            case TYPE_R:
                if(obj->func == _JR) {
                    strcat(temp, toBinaryRegister(obj->op1->enderecamento.registrador));
                    strcat(temp, "_");
                    strcat(temp, getZeros(5));
                    strcat(temp, "_");
                    strcat(temp, getZeros(5));
                    strcat(temp, "_");
                    strcat(temp, getZeros(5));
                    strcat(temp, "_");
                    strcat(temp, toBinaryFunction(obj->func));
                    break;
                }
                strcat(temp, toBinaryRegister(obj->op2->enderecamento.registrador));
                strcat(temp, "_");
                strcat(temp, toBinaryRegister(obj->op3->enderecamento.registrador));
                strcat(temp, "_");
                strcat(temp, toBinaryRegister(obj->op1->enderecamento.registrador));
                strcat(temp, "_");
                strcat(temp, getZeros(5));
                strcat(temp, "_");
                strcat(temp, toBinaryFunction(obj->func));
                break;
            case TYPE_I:
                if(obj->opcode == _LI) {
                    strcat(temp, getZeros(5));
                    strcat(temp, "_");
                    strcat(temp, toBinaryRegister(obj->op1->enderecamento.registrador));
                    strcat(temp, "_");
                    strcat(temp, decimalToBinaryStr(obj->op2->enderecamento.imediato, 16));
                    break;
                } else if(obj->opcode == _JF) {
                    strcat(temp, toBinaryRegister(obj->op1->enderecamento.registrador));
                    strcat(temp, "_");
                    strcat(temp, getZeros(5));
                    strcat(temp, "_");
                    strcat(temp, decimalToBinaryStr(getLinhaLabel(obj->op2->enderecamento.label), 16));
                    break;
                }

                if(obj->op2 == NULL) {
                    strcat(temp, getZeros(5));
                } else {
                    if(obj->op2->tipoEnderecamento == REGISTRADOR) {
                        strcat(temp, toBinaryRegister(obj->op2->enderecamento.registrador));
                    } else if(obj->op2->tipoEnderecamento == INDEXADO) {
                        strcat(temp, toBinaryRegister(obj->op2->enderecamento.indexado.registrador));
                        strcat(temp, "_");
                        strcat(temp, toBinaryRegister(obj->op1->enderecamento.registrador));
                        strcat(temp, "_");
                        strcat(temp, decimalToBinaryStr(obj->op2->enderecamento.indexado.offset, 16));
                        break;
                    }
                }
                strcat(temp, "_");

                if(obj->op1 == NULL) {
                    strcat(temp, getZeros(5));
                } else {
                    if(obj->op1->tipoEnderecamento == REGISTRADOR) {
                        strcat(temp, toBinaryRegister(obj->op1->enderecamento.registrador));
                    }
                }
                strcat(temp, "_");

                if(obj->op3 == NULL) {
                    strcat(temp, getZeros(16));
                } else {
                    if(obj->op3->tipoEnderecamento == IMEDIATO) {
                        strcat(temp, decimalToBinaryStr(obj->op3->enderecamento.imediato, 16));
                    } else if(obj->op3->tipoEnderecamento == LABEL) {
                        strcat(temp, decimalToBinaryStr(getLinhaLabel(obj->op3->enderecamento.label), 16));
                    }
                }

                break;
            case TYPE_J:
                if(obj->opcode == _J || obj->opcode == _JAL) {
                    strcat(temp, decimalToBinaryStr(getLinhaLabel(obj->op1->enderecamento.label), 26));
                } else { // HALT, NOP
                    strcat(temp, getZeros(26));
                }
                break;
        }
        strcat(temp, "; \t// ");
        strcat(temp, obj->func == _DONT_CARE ? toStringOpcode(obj->opcode) : toStringFunction(obj->func));
        emitCode(temp);
        obj = obj->next;
    }
}