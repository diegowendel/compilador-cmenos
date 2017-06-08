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

Parametro pHead = NULL;

EscopoGerador escopoHead = NULL;

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indent = 0;

/* macros to increase/decrease indentation */
#define INDENT indent+=4
#define UNINDENT indent-=4

static char str[32];

// Vetor de char não inicializado
static char temp[100];

static int linha = 0;

const char * toStringOpcode(Opcode op) {
    const char * strings[] = {
        "add", "addi", "sub", "subi", "mul", "muli", "div", "divi",
        "VEC",
        "and", "andi", "or", "ori", "xor", "xori", "not",
        "sl", "sr",
        "mov", "movi",
        "lw", "li", "POP", "sw", "PUSH",
        "beq", "bne", "blt", "blet", "bgt", "bget", "j", "jal", "jr",
        "func", "nop", "halt", "reset",
        "in", "out"
    };
    return strings[op];
}

typedef enum reg {
    T0, T1, T2, T3, T4, T5, T6, T7, T8, T9
} Register;

const char * getReg(Register r) {
    const char * strings[] = {
        "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9" /* Temporários */
    };
    return strings[r];
}



const char * getSavedReg(int i) {
    if(i > 7)   return "$inv"; /* Registrador que simboliza valor inválido */
    const char * strings[] = {
        "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7"
    };
    return strings[i];
}

const char * getArgReg(int i) {
    if(i > 3)   return "$inv"; /* Registrador que simboliza valor inválido */
    const char * strings[] = {
        "$a0", "$a1", "$a2", "$a3"
    };
    return strings[i];
}

const char * getReturnAddressReg() {
    return "$ra";
}

const char * getReturnValueReg() {
    return "$v0";
}

const char * getStackReg() {
    /* Ponteiro de pilhas */
    return "$sp";
}

const char * getInputReg() {
    /* Registrador para entrada de dados */
    return "$in";
}

void geraCodigoInstrucaoAritmetica(Quadruple q, Opcode op) {
    char * op1, * op1RegName, * op2, * op2RegName;

    /* OP1 */
    if(q->op1.kind == String) {
        op1RegName = getRegName(q->op1.contents.variable.name);
        op1 = q->op1.contents.variable.name;
        if(q->op1.contents.variable.scope == NULL) { // Scope é nulo, então é um temporário
            printCode(createObjectInstruction(toStringOpcode(_POP), "emp", op1, NULL));
        } else { // Scope não é nulo, então é uma variável e deve ser lida da memória
            printCode(createObjectInstruction(toStringOpcode(_LOAD), op1RegName, op1, NULL));
        }
    } else { // Valor imediato
        //op1RegName = "temp";
        op1 = itoa(q->op1.contents.val, str, 10);
        //printCode(createObjectInstruction(toStringOpcode(_LOADI), op1RegName, op1, NULL));
    }

    /* OP2 */
    op2RegName = getRegName(q->op2.contents.variable.name);
    if(q->op2.kind == String) {
        op2 = q->op2.contents.variable.name;
        if(q->op2.contents.variable.scope == NULL) { // Scope é nulo, então é um temporário
            printCode(createObjectInstruction(toStringOpcode(_POP), op2RegName, op2, NULL));
        } else { // Scope não é nulo, então é uma variável e deve ser lida da memória
            printCode(createObjectInstruction(toStringOpcode(_LOAD), op2RegName, op2, NULL));
        }
        printCode(createObjectInstruction(toStringOpcode(op), "need fix", op1RegName, op2RegName));
    } else {
        op2 = itoa(q->op2.contents.val, str, 10);
        int nextInstruction = op + 1;
        printCode(createObjectInstruction(toStringOpcode(opcode_map[nextInstruction]), "need fix", op1RegName, op2));
    }
    /* OP3 */
    printCode(createObjectInstruction(toStringOpcode(_PUSH), getReg(T3), NULL, NULL));
}

void geraCodigoInstrucaoLogica(Quadruple q, Opcode op) {
    char * op1, * op2;
    char str[32];
    int loc;

    /* OP1 */
    if(q->op1.kind == String) {
        op1 = q->op1.contents.variable.name;
        if(q->op1.contents.variable.scope == NULL) { // Scope é nulo, então é um temporário
            printCode(createObjectInstruction(toStringOpcode(_POP), getReg(T1), op1, NULL));
        } else { // Scope não é nulo, então é uma variável e deve ser lida da memória
            loc = getMemoryLocation(q->op1.contents.variable.name, q->op1.contents.variable.scope);
            printCode(createObjectInstruction(toStringOpcode(_LOAD), getReg(T1), op1, NULL));
        }
    } else { // Valor imediato
        op1 = itoa(q->op1.contents.val, str, 10);
        printCode(createObjectInstruction(toStringOpcode(_LOADI), getReg(T1), op1, NULL));
    }

    /* OP2 */
    if(q->op2.kind == String) {
        op2 = q->op2.contents.variable.name;
        if(q->op2.contents.variable.scope == NULL) { // Scope é nulo, então é um temporário
            printCode(createObjectInstruction(toStringOpcode(_POP), getReg(T2), op2, NULL));
        } else { // Scope não é nulo, então é uma variável e deve ser lida da memória
            loc = getMemoryLocation(q->op2.contents.variable.name, q->op2.contents.variable.scope);
            printCode(createObjectInstruction(toStringOpcode(_LOAD), getReg(T2), op2, NULL));
        }
    } else {
        op2 = itoa(q->op2.contents.val, str, 10);
        printCode(createObjectInstruction(toStringOpcode(_LOADI), getReg(T2), op2, NULL));
    }
    printCode(createObjectInstruction(toStringOpcode(op), itoa(q->linha, str, 10), getReg(T1), getReg(T2)));
}

void geraCodigoInstrucaoChamada(Quadruple q, Opcode op) {
    printCode(createObjectInstruction(toStringOpcode(op), q->op1.contents.variable.name, NULL, NULL));
}

void geraCodigoInstrucaoFuncao(Quadruple q, Opcode op) {
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
    strcat(temp, "\t");
    if(instrucao->op1 != NULL) {
        strcat(temp, instrucao->op1);
    }
    if(instrucao->op2 != NULL) {
        strcat(temp, ", ");
        strcat(temp, instrucao->op2);
    }
    if(instrucao->op3 != NULL) {
        strcat(temp, ", ");
        strcat(temp, instrucao->op3);
    }
    emitObjectCode(temp, indent);
    free(instrucao);
}

void geraCodigoObjeto(Quadruple q) {
    INDENT;
    int paramCall = 0, paramFunc = 0;
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
                pushEscopoGerador(createEscopoGerador(q->op1.contents.variable.name));
                paramFunc = 0;
                break; /* FUNC */

            case RTN:
                printCode(createObjectInstruction(toStringOpcode(_MOV), getReturnValueReg(), q->op1.contents.variable.name, NULL));
                printCode(createObjectInstruction(toStringOpcode(_JUMPR), getReturnAddressReg(), NULL, NULL));
                break; /* RTN */

            case GET_PARAM:
                if(escopoHead->argRegCount < 4) {
                    insertParametro(createParametro(q->op1, (char *) getArgReg(escopoHead->argRegCount)));
                    ++escopoHead->argRegCount;
                } else if(escopoHead->argRegCount >= 4) {
                    insertParametro(createParametro(q->op1, (char *) getSavedReg(escopoHead->savedRegCount)));
                    sprintf(str, "%d($sp)", getMemoryLocation(q->op1.contents.variable.name, q->op1.contents.variable.scope));
                    printCode(createObjectInstruction(toStringOpcode(_LOAD), (char *) getSavedReg(escopoHead->savedRegCount), str, NULL));
                    ++escopoHead->savedRegCount;
                }
                break; /* GET_PARAM */

            case SET_PARAM:
                /* Se a chamada de função tiver até 4 parâmetros, utiliza os registradores $a0 - $a3
                 * caso contrário, o excedente deve ser armazenado na stack
                 */
                if(paramCall < 4) {
                    printCode(createObjectInstruction(toStringOpcode(_MOV), getArgReg(paramCall), q->op1.contents.variable.name, NULL));
                } else {
                    // TODO FIXxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
                    sprintf(str, "%d($sp)", getMemoryLocation(q->op1.contents.variable.name, q->op1.contents.variable.scope));
                    //insertParametro(createParametro(MEM_POS, str));
                    printCode(createObjectInstruction(toStringOpcode(_STORE), str, q->op1.contents.variable.name, NULL));
                }
                ++paramCall;
                break; /* SET_PARAM */

            case CALL:
                paramCall = 0;
                if(!strcmp(q->op1.contents.variable.name, "input")) {
                    if((q->next->instruction == ASN) && (!strcmp(q->op3.contents.variable.name, q->next->op2.contents.variable.name))) {
                        q = q->next;
                        printCode(createObjectInstruction(toStringOpcode(_IN), getInputReg(), NULL, NULL));
                        sprintf(str, "%d($sp)", getMemoryLocation(q->op1.contents.variable.name, q->op1.contents.variable.scope));
                        printCode(createObjectInstruction(toStringOpcode(_STORE), getInputReg(), str, NULL));
                    }
                } else if(!strcmp(q->op1.contents.variable.name, "output")) {
                    printCode(createObjectInstruction(toStringOpcode(_OUT), q->op3.contents.variable.name, NULL, NULL));
                } else if(!strcmp(q->op1.contents.variable.name, "main")) {
                    fprintf(listing, "%s\n", "main");
                    geraCodigoInstrucaoChamada(q, _JUMPAL);
                } else {


                    geraCodigoInstrucaoChamada(q, _JUMPAL);
                }
                /* Nome da função chamada */
                break; /* CALL */

            case PARAM_LIST:
                /* Terá um novo chamado de função */
                /* Empilha uma nova chamada de função da pilha de chamadas */
                //pushChamada(createChamada(q->op1.contents.val));
                break; /* PARAM_LIST */

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

EscopoGerador createEscopoGerador(const char * nome) {
    EscopoGerador eg = (EscopoGerador) malloc(sizeof(struct escopoGerador));
    eg->argRegCount = 0;
    eg->savedRegCount = 0;
    eg->nome = nome;
    eg->paramList = NULL;
    eg->next = NULL;
    return eg;
}

void pushEscopoGerador(EscopoGerador eg) {
    if(escopoHead == NULL) {
        escopoHead = eg;
    } else {
        eg->next = escopoHead;
        escopoHead = eg;
    }
}

void popEscopoGerador() {
    if(escopoHead != NULL) {
        EscopoGerador eg = escopoHead;
        escopoHead = eg->next;
        free(eg);
    }
}

Parametro createParametro(Operand op, char * regName) {
    Parametro p = (Parametro) malloc(sizeof(struct parametro));
    p->op = op;
    p->regName = regName;
    p->next = NULL;
    return p;
}

void insertParametro(Parametro p) {
    Parametro head;
    if(escopoHead != NULL) {
        head = escopoHead->paramList;
    }

    if(head == NULL) { /* Escopo ainda não tem nenhum parâmetro */
        escopoHead->paramList = p;
    } else { /* Escopo já tem parâmetro, então coloca no fim da lista o novo parâmetro */
        Parametro temp = head;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = p;
    }
}

char * getRegName(char * name) {
    Parametro pl;
    if(escopoHead != NULL) {
        pl = escopoHead->paramList;
    }
    while(pl != NULL) {
        if(!strcmp(name, pl->op.contents.variable.name)) {
            return pl->regName;
        }
        pl = pl->next;
    }
    return NULL;
}

void freeParamList(Parametro head) {
    Parametro temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
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
    } else {
        Objeto temp = objHead;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = obj;
    }
}
