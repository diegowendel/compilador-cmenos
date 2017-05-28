/****************************************************/
/* Arquivo: target.h                                */
/* Implementação do Gerador de código objeto        */
/* Diego Wendel de Oliveira Ferreira		        */
/****************************************************/

#include "target.h"
#include "globals.h"
#include "cgen.h"
#include "code.h"
#include "symtab.h"

/* Cabeça da Lista de instruções objeto */
Objeto objHead = NULL;

/* Cabeça da Lista de instruções binárias */
Binario binHead = NULL;

int topoStack = 0;
int R0 = 0, R1 = 1, R2 = 2, R3 = 3, R4 = 4;

char auxString[20];

const char * toStringOpcode(enum opcode op) {
    const char * strings[] = {
        "ADD", "ADDI", "SUB", "SUBI", "MULT", "MULTI", "DIV", "DIVI",
        "VEC",
        "AND", "ANDI", "OR", "ORI", "XOR", "XORI", "NOT",
        "SL", "SR",
        "MOV", "MOVI",
        "LOAD", "LOADI", "LOADSTK", "STORE", "STORESTK",
        "BEQ", "BNE", "BLT", "BLET", "BGT", "BGET", "JUMP",
        "NOP", "HALT", "RESET",
        "IN", "OUT"
    };
    return strings[op];
}

const char * toStringOpcodeBinary(enum opcode op) {
    const char * strings[] = {
        "000001", "000010", "000011", "000100"
        /* to be continued */
    };
    return strings[op];
}

const char * getRegisterName(int n) {
    const char * strings[] = {
        "R0", "R1", "R2", "R3", "R4",
        "R5", "R6", "R7", "R8", "R9"
    };
    return strings[n];
}

const char * getRegisterBinary(int n) {
    const char * strings[] = {
        "00000", "00001", "00010", "00011", "00100",
        "00101", "00110", "00111", "01000", "01001"
    };
    return strings[n];
}

char * numberToString(int num) {
    sprintf(auxString, "%d", num);
    return auxString;
}

void geraCodigoInstrucaoAritmetica(Quadruple q, enum opcode op) {
    const char * op1, * op2;
    int loc;

    /* OP1 */
    if(q->op1.kind == String) {
        op1 = q->op1.contents.variable.name;
        if(q->op1.contents.variable.scope == NULL) { // Scope é nulo, então é um temporário
            insertObjectInstruction(createObjectInstruction(toStringOpcode(_LOADSTK), getRegisterName(R1), op1, NULL));
            insertBinarieInstruction(createBinarieInstructionTypeI(toStringOpcodeBinary(_LOADSTK), getRegisterBinary(R1), NULL, "numberToStringTemporario(op1)"));
        } else { // Scope não é nulo, então é uma variável e deve ser lida da memória
            loc = getMemoryLocation(q->op1.contents.variable.name, q->op1.contents.variable.scope);
            insertObjectInstruction(createObjectInstruction(toStringOpcode(_LOAD), getRegisterName(R1), op1, NULL));
            insertBinarieInstruction(createBinarieInstructionTypeI(toStringOpcodeBinary(_LOAD), getRegisterBinary(R1), NULL, numberToString(loc)));
        }
    } else { // Valor imediato
        op1 = numberToString(q->op1.contents.val);
        insertObjectInstruction(createObjectInstruction(toStringOpcode(_LOADI), getRegisterName(R1), op1, NULL));
        insertBinarieInstruction(createBinarieInstructionTypeI(toStringOpcodeBinary(_LOADI), getRegisterBinary(R1), NULL, op1));
    }

    /* OP2 */
    if(q->op2.kind == String) {
        op2 = q->op2.contents.variable.name;
        if(q->op2.contents.variable.scope == NULL) { // Scope é nulo, então é um temporário
            insertObjectInstruction(createObjectInstruction(toStringOpcode(_LOADSTK), getRegisterName(R2), op2, NULL));
            insertBinarieInstruction(createBinarieInstructionTypeI(toStringOpcodeBinary(_LOADSTK), getRegisterBinary(R2), NULL, "numberToStringTemporario(op2)"));
        } else { // Scope não é nulo, então é uma variável e deve ser lida da memória
            loc = getMemoryLocation(q->op2.contents.variable.name, q->op2.contents.variable.scope);
            insertObjectInstruction(createObjectInstruction(toStringOpcode(_LOAD), getRegisterName(R2), op2, NULL));
            insertBinarieInstruction(createBinarieInstructionTypeI(toStringOpcodeBinary(_LOAD), getRegisterBinary(R2), NULL, numberToString(loc)));
        }
    } else {
        op2 = numberToString(q->op2.contents.val);
        insertObjectInstruction(createObjectInstruction(toStringOpcode(_LOADI), getRegisterName(R2), op2, NULL));
        insertBinarieInstruction(createBinarieInstructionTypeI(toStringOpcodeBinary(_LOADI), getRegisterBinary(R2), NULL, op2));
    }

    insertObjectInstruction(createObjectInstruction(toStringOpcode(op), getRegisterName(R3), getRegisterName(R1), getRegisterName(R2)));
    insertBinarieInstruction(createBinarieInstructionTypeR(toStringOpcodeBinary(op), getRegisterBinary(R1), getRegisterBinary(R2), getRegisterBinary(R3), NULL, NULL));

    /* OP3 */
    insertObjectInstruction(createObjectInstruction(toStringOpcode(_STORESTK), getRegisterName(R3), "storeOnMemory", NULL));
    insertBinarieInstruction(createBinarieInstructionTypeI(toStringOpcodeBinary(_STORESTK), getRegisterBinary(R3), NULL, "POSICAO DE MEMORIA"));
}

void printObjectCode() {
    Objeto o = objHead;
    int linha = 0;
    char quad[100];

    emitCode("\n");
    while(o != NULL) {
        sprintf(quad, "%d:", linha++);
        strcat(quad, o->opcode);
        strcat(quad, " ");
        if(o->op1 != NULL)
        strcat(quad, o->op1);
        strcat(quad, " ");
        if(o->op2 != NULL)
        strcat(quad, o->op2);
        strcat(quad, " ");
        if(o->op3 != NULL)
        strcat(quad, o->op3);

        emitCode(quad);
        o = o->next;
    }
}

void geraCodigoObjeto(Quadruple q) {
    fprintf(listing, "CODIGO OBJETOO");
    while(q != NULL) {
        switch (q->instruction) {
            case ADD:
                geraCodigoInstrucaoAritmetica(q, _ADD);
                break; /* ADD */

            case SUB:
                geraCodigoInstrucaoAritmetica(q, _SUB);
                break; /* SUB */

            case MULT:
                geraCodigoInstrucaoAritmetica(q, _MULT);
                break; /* MULT */

            case DIV:
                geraCodigoInstrucaoAritmetica(q, _DIV);
                break; /* DIV */

            case HALT:
                createObjectInstruction(toStringOpcode(_HALT), NULL, NULL, NULL);
                createBinarieInstructionTypeJ(toStringOpcodeBinary(_HALT), NULL);
                break; /* HALT */

            default:
                break;
        }
        q = q->next;
    }
    printObjectCode();
}

Binario createBinarieInstructionTypeJ(const char * opcode, const char * imediato) {
    Binario bin = (Binario) malloc(sizeof(struct binario));
    bin->tipo = J_TYPE;
    bin->opcode = opcode;
    bin->contents.j.imediato = imediato;
    bin->next = NULL;
    return bin;
}

Binario createBinarieInstructionTypeI(const char * opcode, const char * rs, const char * rt, const char * imediato) {
    Binario bin = (Binario) malloc(sizeof(struct binario));
    bin->tipo = I_TYPE;
    bin->opcode = opcode;
    bin->contents.i.rs = rs;
    bin->contents.i.rt = rt;
    bin->contents.i.imediato = imediato;
    bin->next = NULL;
    return bin;
}

Binario createBinarieInstructionTypeR(const char * opcode, const char * rs, const char * rt, const char * rd, const char * shamt, const char * func) {
    Binario bin = (Binario) malloc(sizeof(struct binario));
    bin->tipo = I_TYPE;
    bin->opcode = opcode;
    bin->contents.r.rs = rs;
    bin->contents.r.rt = rt;
    bin->contents.r.rd = rd;
    bin->contents.r.shamt = shamt;
    bin->contents.r.func = func;
    bin->next = NULL;
    return bin;
}

void insertBinarieInstruction(Binario bin) {
    if(binHead == NULL) {
        binHead = bin;
        binHead->next = NULL;
    } else {
        Binario temp = binHead;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = bin;
        temp->next->next = NULL;
    }
}

Objeto createObjectInstruction(const char * opcode, const char * op1, const char * op2, const char * op3) {
    Objeto obj = (Objeto) malloc(sizeof(struct objeto));
    obj->opcode = opcode;
    obj->op1 = op1;
    obj->op2 = op2;
    obj->op3 = op3;
    obj->next = NULL;
    return obj;
}

void insertObjectInstruction(Objeto obj) {
    if(objHead == NULL) {
        objHead = obj;
        objHead->next = NULL;
    } else {
        Objeto temp = objHead;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = obj;
        temp->next->next = NULL;
    }
}
