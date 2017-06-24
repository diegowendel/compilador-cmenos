/****************************************************/
/* Arquivo: binary.c                                */
/* Implementação do Gerador de código binário       */
/* Diego Wendel de Oliveira Ferreira		        */
/****************************************************/

#include "target.h"
#include "code.h"

char temp[100];

void geraCodigoBinario(Objeto codigoObjeto) {
    emitCode("\n********** Código binário **********\n");
    Objeto obj = codigoObjeto;
    int linha = 0;

    while(obj != NULL) {
        sprintf(temp, "%d: (", linha++);

        strcat(temp, obj->opcode);
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
