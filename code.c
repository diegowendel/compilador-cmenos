/****************************************************/
/* File: code.c                                     */
/* TM Code emitting utilities                       */
/* implementation for the TINY compiler             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "code.h"

/* Função toString que retorna o mnemônico da instrução
 * conforme o enum de parâmetro
 */
const char * toString(enum instrucao i) {
    const char * strings[] = {  "addition", "subtraction", "multiplication", "division", "vector",
                   "equal", "not_equal", "less_than", "less_than_equal_to",
                   "greater_than", "greater_than_equal_to", "assign",
                   "function", "return", "get_param", "set_param", "call", "param_list",
                   "jump_if_false", "goto", "label", "halt"};
    return strings[i];
}

void emitSpaces(int indent){
    int i;
    for(i = 0; i < indent; ++i) {
        fprintf(code, " ");
    }
}

void emitComment(const char * c, int indent) {
    if (TraceCode) {
        emitSpaces(indent);
        fprintf(code, "# %s\n", c);
    }
}

void emitCode(const char * c) {
    fprintf(code, "%s\n", c);
}

void emitObjectCode(const char * c, int indent) {
    emitSpaces(indent);
    fprintf(code, "%s\n", c);
}
