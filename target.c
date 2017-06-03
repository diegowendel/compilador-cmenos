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
#include "util.h"

#define BIN 2
#define DEC 10

/* Cabeça da Lista de instruções objeto */
Objeto objHead = NULL;

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indent = 0;

/* macros to increase/decrease indentation */
#define INDENT indent+=4
#define UNINDENT indent-=4

int R0 = 0, R1 = 1, R2 = 2, R3 = 3, R4 = 4;

static char str[32];

// Vetor de char não inicializado
static char temp[100];

static int linha = 0;

const char * toStringOpcode(enum opcode op) {
    const char * strings[] = {
        "ADD", "ADDI", "SUB", "SUBI", "MULT", "MULTI", "DIV", "DIVI",
        "VEC",
        "AND", "ANDI", "OR", "ORI", "XOR", "XORI", "NOT",
        "SL", "SR",
        "MOV", "MOVI",
        "LOAD", "LOADI", "POP", "STORE", "PUSH",
        "BEQ", "BNE", "BLT", "BLET", "BGT", "BGET", "JUMP",
        "FUNC", "CALL", "NOP", "HALT", "RESET",
        "IN", "OUT"
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

void geraCodigoInstrucaoAritmetica(Quadruple q, enum opcode op) {
    char * op1, * op2;
    int loc;

    /* OP1 */
    if(q->op1.kind == String) {
        op1 = q->op1.contents.variable.name;
        if(q->op1.contents.variable.scope == NULL) { // Scope é nulo, então é um temporário
            printCode(createObjectInstruction(toStringOpcode(_POP), getRegisterName(R1), op1, NULL));
        } else { // Scope não é nulo, então é uma variável e deve ser lida da memória
            loc = getMemoryLocation(q->op1.contents.variable.name, q->op1.contents.variable.scope);
            printCode(createObjectInstruction(toStringOpcode(_LOAD), getRegisterName(R1), op1, NULL));
        }
    } else { // Valor imediato
        op1 = itoa(q->op1.contents.val, str, 10);
        printCode(createObjectInstruction(toStringOpcode(_LOADI), getRegisterName(R1), op1, NULL));
    }

    /* OP2 */
    if(q->op2.kind == String) {
        op2 = q->op2.contents.variable.name;
        if(q->op2.contents.variable.scope == NULL) { // Scope é nulo, então é um temporário
            printCode(createObjectInstruction(toStringOpcode(_POP), getRegisterName(R2), op2, NULL));
        } else { // Scope não é nulo, então é uma variável e deve ser lida da memória
            loc = getMemoryLocation(q->op2.contents.variable.name, q->op2.contents.variable.scope);
            printCode(createObjectInstruction(toStringOpcode(_LOAD), getRegisterName(R2), op2, NULL));
        }
    } else {
        op2 = itoa(q->op2.contents.val, str, 10);
        printCode(createObjectInstruction(toStringOpcode(_LOADI), getRegisterName(R2), op2, NULL));
    }

    printCode(createObjectInstruction(toStringOpcode(op), getRegisterName(R3), getRegisterName(R1), getRegisterName(R2)));

    /* OP3 */
    printCode(createObjectInstruction(toStringOpcode(_PUSH), getRegisterName(R3), NULL, NULL));
}

void geraCodigoInstrucaoLogica(Quadruple q, enum opcode op) {
    char * op1, * op2;
    char str[32];
    int loc;

    /* OP1 */
    if(q->op1.kind == String) {
        op1 = q->op1.contents.variable.name;
        if(q->op1.contents.variable.scope == NULL) { // Scope é nulo, então é um temporário
            printCode(createObjectInstruction(toStringOpcode(_POP), getRegisterName(R1), op1, NULL));
        } else { // Scope não é nulo, então é uma variável e deve ser lida da memória
            loc = getMemoryLocation(q->op1.contents.variable.name, q->op1.contents.variable.scope);
            printCode(createObjectInstruction(toStringOpcode(_LOAD), getRegisterName(R1), op1, NULL));
        }
    } else { // Valor imediato
        op1 = itoa(q->op1.contents.val, str, 10);
        printCode(createObjectInstruction(toStringOpcode(_LOADI), getRegisterName(R1), op1, NULL));
    }

    /* OP2 */
    if(q->op2.kind == String) {
        op2 = q->op2.contents.variable.name;
        if(q->op2.contents.variable.scope == NULL) { // Scope é nulo, então é um temporário
            printCode(createObjectInstruction(toStringOpcode(_POP), getRegisterName(R2), op2, NULL));
        } else { // Scope não é nulo, então é uma variável e deve ser lida da memória
            loc = getMemoryLocation(q->op2.contents.variable.name, q->op2.contents.variable.scope);
            printCode(createObjectInstruction(toStringOpcode(_LOAD), getRegisterName(R2), op2, NULL));
        }
    } else {
        op2 = itoa(q->op2.contents.val, str, 10);
        printCode(createObjectInstruction(toStringOpcode(_LOADI), getRegisterName(R2), op2, NULL));
    }
    printCode(createObjectInstruction(toStringOpcode(op), itoa(q->linha, str, 10), getRegisterName(R1), getRegisterName(R2)));
}

void geraCodigoInstrucaoChamada(Quadruple q, enum opcode op) {
    printCode(createObjectInstruction(toStringOpcode(op), q->op1.contents.variable.name, NULL, NULL));
}

void geraCodigoInstrucaoFuncao(Quadruple q, enum opcode op) {
    // Atribui fim de string para todas posições de temp, isso é feito pois o Procedimento
    // strcat só insere de forma correta strings inicializadas.
    memset(temp, '\0', sizeof(temp));
    strcat(temp, "\n");
    strcat(temp, q->op1.contents.variable.name);
    strcat(temp, ":");
    emitCode(temp);
}

void geraCodigoInstrucaoLabel(Quadruple q) {
    // Atribui fim de string para todas posições de temp, isso é feito pois o Procedimento
    // strcat só insere de forma correta strings inicializadas.
    memset(temp, '\0', sizeof(temp));
    strcat(temp, ".");
    strcat(temp, q->op1.contents.variable.name);
    emitCode(temp);
}

void printCode(Objeto instrucao) {
    memset(temp, '\0', sizeof(temp));
    sprintf(temp, "%d:\t", linha++);
    strcat(temp, instrucao->opcode);
    strcat(temp, " ");
    if(instrucao->op1 != NULL)
    strcat(temp, instrucao->op1);
    strcat(temp, " ");
    if(instrucao->op2 != NULL)
    strcat(temp, instrucao->op2);
    strcat(temp, " ");
    if(instrucao->op3 != NULL)
    strcat(temp, instrucao->op3);
    emitObjectCode(temp, indent);
    free(instrucao);
}

void geraCodigoObjeto(Quadruple q) {
    INDENT;
    emitCode("\n********** Código objeto **********\n");
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

            case EQ:
                geraCodigoInstrucaoLogica(q, _BEQ);
                break; /* BEQ */

            case NE:
                geraCodigoInstrucaoLogica(q, _BNE);
                break; /* BNE */

            case LT:
                geraCodigoInstrucaoLogica(q, _BLT);
                break; /* BLT */

            case LET:
                geraCodigoInstrucaoLogica(q, _BLET);
                break; /* BLET */

            case GT:
                geraCodigoInstrucaoLogica(q, _BGT);
                break; /* BGT */

            case GET:
                geraCodigoInstrucaoLogica(q, _BGET);
                break; /* BGET */

            case ASN:
                printCode(createObjectInstruction(toStringOpcode(_STORE), q->op1.contents.variable.name, NULL, NULL));
                break; /* STORE */

            case FUNC:
                geraCodigoInstrucaoFuncao(q, _FUNC);
                break; /* FUNC */

            case RTN:

                break; /* RTN */

            case PARAM:
                printCode(createObjectInstruction(toStringOpcode(_PUSH), q->op1.contents.variable.name, NULL, NULL));
                break; /* PARAM */

            case CALL:
                geraCodigoInstrucaoChamada(q, _CALL);
                break; /* CALL */

            case ARGS:

                break;

            case JPF:

                break;

            case GOTO:
                printCode(createObjectInstruction(toStringOpcode(_JUMP), q->op1.contents.variable.name, NULL, NULL));
                break;

            case LBL:
                geraCodigoInstrucaoLabel(q);
                break;

            case HALT:
                printCode(createObjectInstruction(toStringOpcode(_HALT), NULL, NULL, NULL));
                break; /* HALT */

            default:
                break;
        }
        q = q->next;
    }
}

/*Binario createBinarieInstructionTypeJ(const char * opcode, const char * imediato) {
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
}*/

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
