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

/* Cabeça da lista de instruções objeto */
Objeto objHead = NULL;

/* Cabeça da lista de registradores */
Registrador pHead = NULL;

/* Escopo atual */
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
        "lw", "li", "sw",
        "beq", "bne", "blt", "blet", "bgt", "bget", "j", "jal", "jr",
        "func", "nop", "halt", "reset",
        "in", "out"
    };
    return strings[op];
}

const char * getTempReg(int i) {
    if(i > 9)   return "$inv"; /* Registrador que simboliza valor inválido */
    const char * strings[] = {
        "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9" /* Registradores Temporários */
    };
    return strings[i];
}

const char * getSavedReg(int i) {
    if(i > 7)   return "$inv"; /* Registrador que simboliza valor inválido */
    const char * strings[] = {
        "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7" /* Registradores Salvos */
    };
    return strings[i];
}

const char * getArgReg(int i) {
    if(i > 3)   return "$inv"; /* Registrador que simboliza valor inválido */
    const char * strings[] = {
        "$a0", "$a1", "$a2", "$a3" /* Registradores de argumentos de funções */
    };
    return strings[i];
}

const char * getReturnAddressReg() {
    return "$ra"; /* Endereço para retorno da função atual */
}

const char * getReturnValueReg() {
    return "$v0"; /* Valor de retorno da função */
}

const char * getStackReg() {
    return "$sp"; /* Ponteiro de pilhas */
}

const char * getOutputReg() {
    return "$out"; /* Registrador para saída de dados */
}

const char * getVectorReg() {
    return "$vec"; /* Registrador auxiliar para vetores */
}

const char * getRZero() {
    return "$rz"; /* Registrador com valor Zero */
}

const char * getStackLocation(Operand op) {
    sprintf(str, "%d($sp)", getMemoryLocation(op.contents.variable.name, op.contents.variable.scope));
    return str;
}

char * getOperandRegName(Operand op) {
    char * opRegName, * op1;
    Operand operand;

    /* OPERANDO */
    if(op.kind == String) { /* Registrador */
        opRegName = getRegName(op.contents.variable.name);
        if(opRegName == NULL) {
            if(op.contents.variable.scope == NULL) { // Scope é nulo, então é um temporário
                opRegName = (char *) getTempReg(escopoHead->tempRegCount++);
                Registrador r = createRegistrador(op, opRegName);
                insertRegistrador(r);
                printCode(createObjectInstruction(toStringOpcode(_LOAD), opRegName, getRegName(op.contents.variable.name), NULL));
            } else { // Scope não é nulo, então é uma variável e deve ser lida da memória
                opRegName = (char *) getSavedReg(escopoHead->savedRegCount++);
                Registrador r = createRegistrador(op, opRegName);
                insertRegistrador(r);
                printCode(createObjectInstruction(toStringOpcode(_LOAD), opRegName, getStackLocation(op), NULL));
            }
        }
    } else { /* Valor Imediato */
        // Lê um valor imediato em um registrador
        op1 = itoa(op.contents.val, str, DEC);
        opRegName = (char *) getTempReg(escopoHead->tempRegCount++);
        operand.kind = String;
        operand.contents.variable.name = opRegName;
        operand.contents.variable.scope = NULL;
        Registrador r = createRegistrador(operand, opRegName);
        insertRegistrador(r);
        printCode(createObjectInstruction(toStringOpcode(_LOADI), opRegName, op1, NULL));
    }
    return opRegName;
}

char * getOperandResultRegName(Operand op) {
    char * opRegName;
    // Cria registrador temporário para armazenar o resultado da expressão
    opRegName = (char *) getTempReg(escopoHead->tempRegCount++);
    Registrador r = createRegistrador(op, opRegName);
    insertRegistrador(r);
    return opRegName;
}

char * getTempRegName(Operand op) {
    char * opRegName;
    opRegName = (char *) getTempReg(escopoHead->tempRegCount++);
    Registrador r = createRegistrador(op, opRegName);
    insertRegistrador(r);
    return opRegName;
}

void geraCodigoInstrucaoAritmetica(Quadruple q, Opcode op) {
    char * op1RegName, * op2RegName, * op3RegName;

    /* Busca ou atribui o registrador do operando 1 */
    op1RegName = getOperandRegName(q->op1);
    /* Atribui um registrador para o resultado da expressão */
    op3RegName = getOperandResultRegName(q->op3);

    /* OPERANDO 2 */
    if(q->op2.kind == String) { /* Registrador */
        /* Busca ou atribui o registrador do operando 2 */
        op2RegName = getOperandRegName(q->op2);
        /* Imprime a instrução aritmética */
        printCode(createObjectInstruction(toStringOpcode(op), op3RegName, op1RegName, op2RegName));
    } else { /* Valor Imediato */
        // Valor imediato
        op2RegName = itoa(q->op2.contents.val, str, 10);
        // nextInstruction é a versão imediato da instrução atual
        int nextInstruction = op + 1;
        /* Imprime a instrução aritmética, versão imediato */
        printCode(createObjectInstruction(toStringOpcode(opcode_map[nextInstruction]), op3RegName, op1RegName, op2RegName));
    }
}

void geraCodigoInstrucaoLogica(Quadruple q, Opcode op) {
    char * op1RegName, * op2RegName, * op3RegName;

    /* Busca ou atribui o registrador do operando 1 */
    op1RegName = getOperandRegName(q->op1);
    /* Atribui um registrador para o resultado da expressão */
    op3RegName = getOperandResultRegName(q->op3);

    /* OPERANDO 2 */
    if(q->op2.kind == String) { /* Registrador */
        /* Busca ou atribui o registrador do operando 2 */
        op2RegName = getOperandRegName(q->op2);
        /* Imprime a instrução aritmética */
        printCode(createObjectInstruction(toStringOpcode(op), op3RegName, op1RegName, op2RegName));
    } else { /* Valor Imediato */
        // Valor imediato
        op2RegName = itoa(q->op2.contents.val, str, 10);
        /* Imprime a instrução lógica */
        printCode(createObjectInstruction(toStringOpcode(op), op3RegName, op1RegName, op2RegName));
    }
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
    char * regName;
    emitCode("\n********** Código objeto **********");

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
                regName = getOperandRegName(q->op2);
                printCode(createObjectInstruction(toStringOpcode(_STORE), regName, getStackLocation(q->op1), NULL));
                break; /* STORE */

            case VEC:
                //printCode(createObjectInstruction(toStringOpcode(_VEC), getVectorReg(), q->, const char *op3))
                break; /* VEC */

            case FUNC:
                geraCodigoInstrucaoFuncao(q, _FUNC);
                pushEscopoGerador(createEscopoGerador(q->op1.contents.variable.name));
                break; /* FUNC */

            case RTN:
                /* Verifica se há valor a ser retornado */
                if(q->op1.contents.variable.name != NULL) {
                    regName = getRegName(q->op1.contents.variable.name);
                    if(regName == NULL) {
                        regName = getOperandRegName(q->op1);
                    }
                    printCode(createObjectInstruction(toStringOpcode(_MOV), getReturnValueReg(), regName, NULL));
                }
                /* Só retorna valor se o escopo atual não for o escopo da main */
                if(strcmp(escopoHead->nome, "main")) {
                    printCode(createObjectInstruction(toStringOpcode(_JUMPR), getReturnAddressReg(), NULL, NULL));
                }
                break; /* RTN */

            case GET_PARAM:
                /* Se a chamada de função tiver até 4 parâmetros, lê os registradores $a0 - $a3
                 * caso contrário, o excedente deve ser lido da stack
                 */
                if(escopoHead->argRegCount < 4) {
                    insertRegistrador(createRegistrador(q->op1, (char *) getArgReg(escopoHead->argRegCount++)));
                } else if(escopoHead->argRegCount >= 4) {
                    insertRegistrador(createRegistrador(q->op1, (char *) getSavedReg(escopoHead->savedRegCount++)));
                    printCode(createObjectInstruction(toStringOpcode(_LOAD), (char *) getSavedReg(escopoHead->savedRegCount), getStackLocation(q->op1), NULL));
                }
                break; /* GET_PARAM */

            case SET_PARAM:
                regName = getRegName(q->op1.contents.variable.name);
                if(regName == NULL) {
                    regName = getOperandRegName(q->op1);
                }
                /* Se a chamada de função tiver até 4 parâmetros, utiliza os registradores $a0 - $a3
                 * caso contrário, o excedente deve ser armazenado na stack
                 */
                if(escopoHead->argRegCount < 4) {
                    printCode(createObjectInstruction(toStringOpcode(_MOV), getArgReg(escopoHead->argRegCount++), regName, NULL));
                } else if(escopoHead->argRegCount >= 4) {
                    printCode(createObjectInstruction(toStringOpcode(_STORE), getSavedReg(escopoHead->savedRegCount++), getStackLocation(q->op1), NULL));
                }
                break; /* SET_PARAM */

            case CALL:
                if(!strcmp(q->op1.contents.variable.name, "input")) {
                    printCode(createObjectInstruction(toStringOpcode(_IN), getTempRegName(q->op3), NULL, NULL));
                } else if(!strcmp(q->op1.contents.variable.name, "output")) {
                    printCode(createObjectInstruction(toStringOpcode(_OUT), getOutputReg(), NULL, NULL));
                } else {
                    printCode(createObjectInstruction(toStringOpcode(_JUMPAL), q->op1.contents.variable.name, NULL, NULL));
                    printCode(createObjectInstruction(toStringOpcode(_MOV), getTempRegName(q->op3), getReturnValueReg(), NULL));
                }
                escopoHead->argRegCount = 0;
                escopoHead->savedRegCount = 0;
                break; /* CALL */

            case PARAM_LIST:
                /* Terá um novo chamado de função */
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
    eg->tempRegCount = 0;
    eg->nome = nome;
    eg->regList = NULL;
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

Registrador createRegistrador(Operand op, char * regName) {
    Registrador r = (Registrador) malloc(sizeof(struct registrador));
    r->op = op;
    r->regName = regName;
    r->next = NULL;
    return r;
}

void insertRegistrador(Registrador r) {
    Registrador head;
    if(escopoHead != NULL) {
        head = escopoHead->regList;
    }

    if(head == NULL) { /* Escopo ainda não tem nenhum registrador */
        escopoHead->regList = r;
    } else { /* Escopo já tem registrador, então coloca no fim da lista o novo registrador */
        Registrador temp = head;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = r;
    }
}

char * getRegName(char * name) {
    Registrador reg;
    if(escopoHead != NULL) {
        reg = escopoHead->regList;
    }
    while(reg != NULL) {
        if(!strcmp(name, reg->op.contents.variable.name)) {
            return reg->regName;
        }
        reg = reg->next;
    }
    return NULL;
}

void freeParamList(Registrador head) {
    Registrador temp;
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
