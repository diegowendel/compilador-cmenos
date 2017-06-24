/****************************************************/
/* Arquivo: target.c                                */
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

/* Macros para aumentar e diminuir o espaçamento da identação */
#define INDENT indent+=4
#define UNINDENT indent-=4

/* Cabeça da lista de instruções objeto */
Objeto objHead = NULL;

/* Escopo atual */
EscopoGerador escopoHead = NULL;

/* Variável usada para configurar a identação do código impresso */
static int indent = 0;

/* Variáveis auxiliares na geração de código objeto */
static char temp[100];
static char str[32];
static int linha = 0;

const char * toStringOpcode(Opcode op) {
    const char * strings[] = {
        "add", "addi", "sub", "subi", "mul", "muli", "div", "divi",
        "and", "andi", "or", "ori", "xor", "xori", "not",
        "sl", "sr",
        "mov", "lw", "li", "la", "sw",
        "beq", "bne", "blt", "blet", "bgt", "bget", "j", "jal", "jr",
        "func", "nop", "halt", "reset",
        "in", "out"
    };
    return strings[op];
}

const char * getTempReg(int i) {
    /* Se já tiver usado todos registradores temporários volta a usar do início sem fazer nenhuma
     * verificação adicional, pois os registradores temporários não garantem persistência dos dados
     */
    if(i > 9) {
        escopoHead->tempRegCount = 0;
        i = 0;
    }
    const char * strings[] = {
        "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9" /* Registradores Temporários */
    };
    return strings[i];
}

const char * getSavedReg(int i) {
    /* Se já tiver usado todos registradores temporários volta a usar do início, mas antes de usar o registrador
     * deve salvar seu valor antigo na memória
     */
    if(i > 7) {
        escopoHead->savedRegCount = 0;

        // TODO FIX IT!
    }
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

const char * getRZero() {
    return "$rz"; /* Registrador com valor Zero */
}

const char * getParamRefReg() {
    return "$ref";
}

const char * getVectorReg() {
    return "$vec"; /* Registrador auxiliar para vetores */
}

const char * getMemLocation(char * regName) {
    sprintf(str, "0(%s)", regName);
    return str;
}

const char * getMemIndexedLocation(char * regName, int offset) {
    sprintf(str, "%d(%s)", offset, regName);
    return str;
}

/* Busca a posição de memória do operando op, e retorna sua posição com offset na stack */
const char * getStackOperandLocation(Operand op) {
    int memloc = getMemoryLocation(op.contents.variable.name, op.contents.variable.scope);
    int offset = memloc - (escopoHead->tamanhoBlocoMemoria - 1);
    sprintf(str, "%d($sp)", offset);
    return str;
}

/* Retorna a posição da stack de acordo com o offset passado por parâmetro */
const char * getStackLocation(int offset) {
    sprintf(str, "%d($sp)", offset);
    return str;
}

void pushStackSpace(int n) {
    printCode(createObjectInstruction(toStringOpcode(_ADDI), getStackReg(), getStackReg(), itoa(n, str, DEC)));
}

void popStackSpace(int n) {
    printCode(createObjectInstruction(toStringOpcode(_SUBI), getStackReg(), getStackReg(), itoa(n, str, DEC)));
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
                printCode(createObjectInstruction(toStringOpcode(_LOAD), opRegName, getStackOperandLocation(op), NULL));
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

char * getVectorRegName(Operand op) {
    char * regName = getRegName(op.contents.variable.name);
    if(regName == NULL) {
        regName = (char *) getSavedReg(escopoHead->savedRegCount++);
        Registrador r = createRegistrador(op, regName);
        insertRegistrador(r);
        /* Lê o endereço de memória do início do vetor */
        printCode(createObjectInstruction(toStringOpcode(_LOADA), regName, getStackOperandLocation(op), NULL));
    }
    return regName;
}

char * getTempRegName(Operand op) {
    char * opRegName;
    // Cria registrador temporário para armazenar o resultado da expressão
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
    op3RegName = getTempRegName(q->op3);

    /* OPERANDO 2 */
    if(q->op2.kind == String) { /* Registrador */
        /* Busca ou atribui o registrador do operando 2 */
        op2RegName = getOperandRegName(q->op2);
        /* Imprime a instrução aritmética */
        printCode(createObjectInstruction(toStringOpcode(op), op3RegName, op1RegName, op2RegName));
    } else { /* Valor Imediato */
        // Valor imediato
        op2RegName = itoa(q->op2.contents.val, str, DEC);
        // nextInstruction é a versão imediato da instrução atual
        int nextInstruction = op + 1;
        /* Imprime a instrução aritmética, versão imediato */
        printCode(createObjectInstruction(toStringOpcode(opcode_map[nextInstruction]), op3RegName, op1RegName, op2RegName));
    }
}

void geraCodigoInstrucaoLogica(Quadruple q, Opcode op, Operand label) {
    char * op1RegName, * op2RegName, * op3RegName;

    /* Busca ou atribui o registrador do operando 1 */
    op1RegName = getOperandRegName(q->op1);
    /* Label para o qual irá ser feito o desvio */
    op3RegName = label.contents.variable.name;

    /* OPERANDO 2 */
    if(q->op2.kind == String) { /* Registrador */
        /* Busca ou atribui o registrador do operando 2 */
        op2RegName = getOperandRegName(q->op2);
        /* Imprime a instrução aritmética */
        printCode(createObjectInstruction(toStringOpcode(op), op1RegName, op2RegName, op3RegName));
    } else { /* Valor Imediato */
        // Valor imediato
        op2RegName = itoa(q->op2.contents.val, str, DEC);
        /* Imprime a instrução lógica */
        printCode(createObjectInstruction(toStringOpcode(op), op1RegName, op2RegName, op3RegName));
    }
}

void geraCodigoInstrucaoAtribuicao(Quadruple q) {
    char * regName = getOperandRegName(q->op2);
    if(q->op1.contents.variable.scope == NULL) {
        // Vetor com índice do acesso igual a uma variável
        printCode(createObjectInstruction(toStringOpcode(_STORE), regName, getMemLocation(getRegName(q->op1.contents.variable.name)), NULL));
    } else {
        if(q->op3.kind != Empty) {
            // Vetor com índice de acesso igual a uma constante
            printCode(createObjectInstruction(toStringOpcode(_STORE), regName, getMemIndexedLocation(getOperandRegName(q->op1), q->op3.contents.val), NULL));
        } else {
            // Variável comum
            printCode(createObjectInstruction(toStringOpcode(_STORE), regName, getStackOperandLocation(q->op1), NULL));
        }
    }
}

void geraCodigoFuncao(Quadruple q, Opcode op) {
    // Atribui fim de string para todas posições de temp, isso é feito pois o Procedimento
    // strcat só insere de forma correta strings inicializadas.
    memset(temp, '\0', sizeof(temp));
    strcat(temp, "\n");
    strcat(temp, q->op1.contents.variable.name);
    strcat(temp, ":");
    emitCode(temp);
}

void geraCodigoChamadaFuncao(Quadruple q) {
    int tamanhoBlocoMemoria;
    /* Verifica o nome da função/procedimento que está sendo chamada, se for input ou output imprime as
     * instruções específicas 'in' e 'out'. Depois verifica o escopo de onde vem a chamada, se for do
     * escopo da 'main' não guarda $ra na memória, caso contrário guarda $ra na memória.
     */
    if(!strcmp(q->op1.contents.variable.name, "input")) {
        printCode(createObjectInstruction(toStringOpcode(_IN), getTempRegName(q->op3), NULL, NULL));
    } else if(!strcmp(q->op1.contents.variable.name, "output")) {
        printCode(createObjectInstruction(toStringOpcode(_OUT), getOutputReg(), NULL, NULL));
    } else if(!strcmp(escopoHead->nome, "main")) {
        tamanhoBlocoMemoria = getTamanhoBlocoMemoriaEscopo(q->op1.contents.variable.name);
        /* Aloca o bloco de memória na stack */
        pushStackSpace(tamanhoBlocoMemoria);
        printCode(createObjectInstruction(toStringOpcode(_JUMPAL), q->op1.contents.variable.name, NULL, NULL));
        printCode(createObjectInstruction(toStringOpcode(_MOV), getTempRegName(q->op3), getReturnValueReg(), NULL));
        /* Desaloca o bloco de memória na stack */
        popStackSpace(tamanhoBlocoMemoria);
    } else {
        tamanhoBlocoMemoria = getTamanhoBlocoMemoriaEscopo(q->op1.contents.variable.name);
        /* Aloca espaço na stack para os parâmetros + 1 para o registrador de endereço de retorno */
        pushStackSpace(tamanhoBlocoMemoria + 1); // +1 devido ao registrador $ra
        printCode(createObjectInstruction(toStringOpcode(_STORE), getReturnAddressReg(), getStackLocation(-escopoHead->tamanhoBlocoMemoria), NULL));
        printCode(createObjectInstruction(toStringOpcode(_JUMPAL), q->op1.contents.variable.name, NULL, NULL));
        printCode(createObjectInstruction(toStringOpcode(_LOAD), getReturnAddressReg(), getStackLocation(-escopoHead->tamanhoBlocoMemoria), NULL));
        popStackSpace(tamanhoBlocoMemoria + 1); // +1 devido ao registrador $ra
        printCode(createObjectInstruction(toStringOpcode(_MOV), getTempRegName(q->op3), getReturnValueReg(), NULL));
    }
}

void geraCodigoSetParam(Quadruple q) {
    char * regName;
    BucketList var = NULL;
    /* Verifica se é uma variável para recuperar o BucketList correspondente */
    if(q->op1.kind == String && q->op1.contents.variable.scope != NULL) {
        // Recupera o BucketList da variável
        var = getVarFromSymtab(q->op1.contents.variable.name, q->op1.contents.variable.scope);
    }
    /* Se a chamada de função tiver até 4 parâmetros, utiliza os registradores $a0 - $a3
     * caso contrário, o excedente deve ser armazenado na stack
     */
    if(escopoHead->argRegCount < 4) {
        /* Verifica se é uma constante ou variável */
        if(q->op1.kind == String) { // Variável
            if(var != NULL && var->treeNode->kind.exp == VectorK) { // Vetor
                printCode(createObjectInstruction(toStringOpcode(_LOADA), getArgReg(escopoHead->savedRegCount), getStackOperandLocation(q->op1), NULL));
            } else { // Variável
                regName = getOperandRegName(q->op1);
                if(strcmp(getArgReg(escopoHead->argRegCount), regName)) { /* Só move se os registradores forem diferentes */
                    printCode(createObjectInstruction(toStringOpcode(_MOV), getArgReg(escopoHead->argRegCount), regName, NULL));
                    moveRegistrador((char *) getArgReg(escopoHead->argRegCount), regName);
                }
            }
        } else { // Constante
            printCode(createObjectInstruction(toStringOpcode(_LOADI), getArgReg(escopoHead->argRegCount), itoa(q->op1.contents.val, str, DEC), NULL));
        }
        escopoHead->argRegCount++;
    } else { // TODO fazer o resto
        /* Verifica se é uma constante ou variável */
        if(q->op1.kind == String) { // Variável
            if(var != NULL && var->treeNode->kind.exp == VectorK) { // Vetor
                printCode(createObjectInstruction(toStringOpcode(_LOADA), getSavedReg(escopoHead->savedRegCount), getStackOperandLocation(q->op1), NULL));
            } else { // Variável
                printCode(createObjectInstruction(toStringOpcode(_LOAD), getSavedReg(escopoHead->savedRegCount), getStackOperandLocation(q->op1), NULL));
            }
        } else { // Constante
            printCode(createObjectInstruction(toStringOpcode(_LOADI), getSavedReg(escopoHead->savedRegCount), itoa(q->op1.contents.val, str, DEC), NULL));
        }
        printCode(createObjectInstruction(toStringOpcode(_STORE), getSavedReg(escopoHead->savedRegCount), getStackOperandLocation(q->op1), NULL));
        escopoHead->savedRegCount++;
    }
}

void geraCodigoGetParam(Quadruple q) {
    /* Se a chamada de função tiver até 4 parâmetros, lê os registradores $a0 - $a3
     * caso contrário, o excedente deve ser lido da stack
     */
    if(escopoHead->argRegCount < 4) {
        insertRegistrador(createRegistrador(q->op1, (char *) getArgReg(escopoHead->argRegCount)));
        escopoHead->argRegCount++;
    } else if(escopoHead->argRegCount >= 4) {
        insertRegistrador(createRegistrador(q->op1, (char *) getSavedReg(escopoHead->savedRegCount)));
        printCode(createObjectInstruction(toStringOpcode(_LOAD), (char *) getSavedReg(escopoHead->savedRegCount), getStackOperandLocation(q->op1), NULL));
        escopoHead->savedRegCount++;
    }
}

void geraCodigoRetorno(Quadruple q) {
    char * regName;
    /* Só retorna valor se o escopo atual não for o escopo da main */
    if(strcmp(escopoHead->nome, "main")) {
        /* Verifica se há valor para ser retornado */
        if(q->op1.kind != Empty) {
            regName = getOperandRegName(q->op1);
            printCode(createObjectInstruction(toStringOpcode(_MOV), getReturnValueReg(), regName, NULL));
        }
        printCode(createObjectInstruction(toStringOpcode(_JUMPR), getReturnAddressReg(), NULL, NULL));
    }
}

void geraCodigoVetor(Quadruple q) {
    char * regName = getVectorRegName(q->op1);
    /* Verifica se o índice é constante ou variável */
    if(q->op2.kind == String) {
        /* Lê o valor da posição do vetor em um registrador temporário */
        printCode(createObjectInstruction(toStringOpcode(_LOAD), getTempRegName(q->op3), getMemLocation(regName), NULL));
        printCode(createObjectInstruction(toStringOpcode(_ADD), getTempRegName(q->op3), regName, getOperandRegName(q->op2)));
    } else {
        printCode(createObjectInstruction(toStringOpcode(_LOAD), getTempRegName(q->op3), getMemIndexedLocation(regName, q->op2.contents.val), NULL));
    }
}

void geraCodigoEnderecoVetor(Quadruple q) {
    /* Lê endereço do vetor */
    char * regName = getVectorRegName(q->op1);
    /* Verifica se o índice é constante ou variável */
    if(q->op2.kind == String) {
        /* Soma o endereço base do vetor com o valor da variável */
        printCode(createObjectInstruction(toStringOpcode(_ADD), getTempRegName(q->op3), regName, getOperandRegName(q->op2)));
    } else {
        //printCode(createObjectInstruction(toStringOpcode(_LOADA), getTempRegName(q->op3), getMemIndexedLocation(regName, q->op2.contents.val), NULL));
    }
}

void geraCodigoLabel(Quadruple q) {
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
                geraCodigoInstrucaoLogica(q, _BEQ, q->next->op2);
                break; /* BEQ */

            case NE:
                geraCodigoInstrucaoLogica(q, _BNE, q->next->op2);
                break; /* BNE */

            case LT:
                geraCodigoInstrucaoLogica(q, _BLT, q->next->op2);
                break; /* BLT */

            case LET:
                geraCodigoInstrucaoLogica(q, _BLET, q->next->op2);
                break; /* BLET */

            case GT:
                geraCodigoInstrucaoLogica(q, _BGT, q->next->op2);
                break; /* BGT */

            case GET:
                geraCodigoInstrucaoLogica(q, _BGET, q->next->op2);
                break; /* BGET */

            case ASN:
                geraCodigoInstrucaoAtribuicao(q);
                break; /* ASN */

            case VEC:
                geraCodigoVetor(q);
                break; /* VEC */

            case VEC_ADDR:
                geraCodigoEnderecoVetor(q);
                break; /* VEC_ADDR */

            case FUNC:
                geraCodigoFuncao(q, _FUNC);
                pushEscopoGerador(createEscopoGerador(q->op1.contents.variable.name));
                escopoHead->savedRegCount = 0;
                break; /* FUNC */

            case RTN:
                geraCodigoRetorno(q);
                break; /* RTN */

            case GET_PARAM:
                geraCodigoGetParam(q);
                break; /* GET_PARAM */

            case SET_PARAM:
                geraCodigoSetParam(q);
                break; /* SET_PARAM */

            case CALL:
                geraCodigoChamadaFuncao(q);
                escopoHead->argRegCount = 0;
                break; /* CALL */

            case GOTO:
                printCode(createObjectInstruction(toStringOpcode(_JUMP), q->op1.contents.variable.name, NULL, NULL));
                break; /* GOTO */

            case LBL:
                geraCodigoLabel(q);
                break; /* LBL */

            case HALT:
                printCode(createObjectInstruction(toStringOpcode(_HALT), NULL, NULL, NULL));
                break; /* HALT */

            case PARAM_LIST:
                //fprintf(listing, "params: %d\n", q->op1.contents.val);
                escopoHead->argRegCount = 0;
                break; /* PARAM_LIST */

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
    // Recupera o BucketList do escopo
    BucketList bl = st_bucket_func((char *) nome);
    /* Recupera o treeNode que representa o escopo para recuperar a quantidade de
     * parâmetros e variáveis
     */
    eg->quantidadeParametros = getQuantidadeParametros(bl->treeNode);
    eg->quantidadeVariaveis = getQuantidadeVariaveis(bl->treeNode);
    eg->tamanhoBlocoMemoria = getTamanhoBlocoMemoriaEscopo((char *) nome);
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

void moveRegistrador(char * dest, char * orig) {
    Registrador origem = getRegistrador(orig);
    origem->regName = dest;
    removeRegistrador(dest);
}

void removeRegistrador(char * name) {
    Registrador atual, anterior;
    if(escopoHead != NULL) {
        atual = escopoHead->regList;
    }
    /* Verifica se o primeiro deve ser removido */
    if(!strcmp(name, atual->regName)) {
        escopoHead->regList = atual->next;
        free(atual);
        atual = NULL;
        return;
    }

    anterior = atual;
    atual = atual->next;

    /* Verifica o restante */
    while(atual != NULL && anterior != NULL) {
        if(!strcmp(name, atual->regName)) {
            anterior->next = atual->next;
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->next;
    }
}

Registrador getRegistrador(char * name) {
    Registrador reg;
    if(escopoHead != NULL) {
        reg = escopoHead->regList;
    }
    while(reg != NULL) {
        if(!strcmp(name, reg->regName)) {
            return reg;
        }
        reg = reg->next;
    }
    return NULL;
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
