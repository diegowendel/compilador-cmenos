/****************************************************/
/* Arquivo: target.c                                */
/* Implementação do Gerador de código objeto        */
/* Diego Wendel de Oliveira Ferreira                */
/****************************************************/

#include "target.h"
#include "globals.h"
#include "cgen.h"
#include "code.h"
#include "symtab.h"
#include "util.h"

/* Macros para aumentar e diminuir o espaçamento da identação */
#define INDENT indent+=4
#define UNINDENT indent-=4

/* Cabeça da lista de instruções objeto */
Objeto objHead = NULL;

/* Cabeça da lista de labels */
Label labelHead = NULL;

/* Escopo atual */
EscopoGerador escopoHead = NULL;

/* Variável usada para configurar a identação do código impresso */
static int indent = 0;

/* Variáveis auxiliares na geração de código objeto */
static char temp[100];
static int linha = 0;

 /* Registradores especiais */
InstOperand rtnAddrReg;
InstOperand rtnValReg;
InstOperand stackReg;
InstOperand outputReg;
InstOperand rzero;

RegisterName tempReg[10] = {
    $t0, $t1, $t2, $t3, $t4, $t5, $t6, $t7, $t8, $t9
};

RegisterName savedReg[10] = {
    $s0, $s1, $s2, $s3, $s4, $s5, $s6, $s7, $s8, $s9
};

RegisterName argReg[4] = {
    $a0, $a1, $a2, $a3
};

const char * toStringOpcode(Opcode op) {
    const char * strings[] = {
        "add", "addi", "sub", "subi", "mul", "muli", "div", "divi",
        "and", "andi", "or", "ori", "xor", "xori", "not",
        "sl", "sr",
        "mov", "lw", "li", "la", "sw",
        "beq", "bne", "blt", "blet", "bgt", "bget", "j", "jal", "jr",
        "nop", "halt", "reset",
        "in", "out"
    };
    return strings[op];
}

const char * toStringRegName(RegisterName rn) {
    const char * strings[] = {
        "$rz", "$v0", "$out", "$sp", "$ra", "$inv", "$ESPECIAL2", "$ESPECIAL3",
        "$a0", "$a1", "$a2", "$a3", "$s0", "$s1", "$s2", "$s3",
        "$s4", "$s5", "$s6", "$s7", "$s8", "$s9", "$t0", "$t1",
        "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9"
    };
    return strings[rn];
}

InstOperand getTempReg(int i) {
    /* Se já tiver usado todos registradores temporários volta a usar do início sem fazer nenhuma
     * verificação adicional, pois os registradores temporários não garantem persistência dos dados
     */
    if(i > 9) {
        escopoHead->tempRegCount = 0;
        i = 0;
    }
    InstOperand operando = (InstOperand) malloc(sizeof(struct instOperand));
    operando->tipoEnderecamento = REGISTRADOR;
    operando->enderecamento.registrador = tempReg[i];
    return operando;
}

InstOperand getSavedReg(int i) {
    /* Se já tiver usado todos registradores temporários volta a usar do início, mas antes de usar o registrador
     * deve salvar seu valor antigo na memória
     */
    if(i > 9) {
        escopoHead->savedRegCount = 0;
        i = 0;

        // TODO FIX IT!
    }
    InstOperand operando = (InstOperand) malloc(sizeof(struct instOperand));
    operando->tipoEnderecamento = REGISTRADOR;
    operando->enderecamento.registrador = savedReg[i];
    return operando;
}

InstOperand getArgReg(int i) {
    InstOperand operando = (InstOperand) malloc(sizeof(struct instOperand));
    operando->tipoEnderecamento = REGISTRADOR;
    operando->enderecamento.registrador = argReg[i];
    if(i > 3) {
        operando->enderecamento.registrador = $inv;
    }
    return operando;
}

InstOperand getMemLocation(RegisterName registrador) {
    // Operando que representa o modo de endereçamento indexado
    InstOperand operando = (InstOperand) malloc(sizeof(struct instOperand));
    operando->tipoEnderecamento = INDEXADO;
    operando->enderecamento.indexado.offset = 0;
    operando->enderecamento.indexado.registrador = registrador;
    return operando;
}

InstOperand getMemIndexedLocation(RegisterName registrador, int offset) {
    // Operando que representa o modo de endereçamento indexado
    InstOperand operando = (InstOperand) malloc(sizeof(struct instOperand));
    operando->tipoEnderecamento = INDEXADO;
    operando->enderecamento.indexado.offset = offset;
    operando->enderecamento.indexado.registrador = registrador;
    return operando;
}

/* Busca a posição de memória do operando op, e retorna sua posição com offset na stack */
InstOperand getStackOperandLocation(Operand op) {
    int memloc = getMemoryLocation(op.contents.variable.name, op.contents.variable.scope);
    int offset = memloc - (escopoHead->tamanhoBlocoMemoria - 1);
    // Operando que representa o modo de endereçamento indexado
    InstOperand operando = (InstOperand) malloc(sizeof(struct instOperand));
    operando->tipoEnderecamento = INDEXADO;
    operando->enderecamento.indexado.offset = offset;
    operando->enderecamento.indexado.registrador = $sp;
    return operando;
}

/* Retorna a posição da stack de acordo com o offset passado por parâmetro */
InstOperand getStackLocation(int offset) {
    // Operando que representa o modo de endereçamento indexado
    InstOperand operando = (InstOperand) malloc(sizeof(struct instOperand));
    operando->tipoEnderecamento = INDEXADO;
    operando->enderecamento.indexado.offset = offset;
    operando->enderecamento.indexado.registrador = $sp;
    return operando;
}

void pushStackSpace(int n) {
    printCode(insertObjInst(createObjInst(_ADDI, TYPE_I, stackReg, stackReg, getImediato(n))));
}

void popStackSpace(int n) {
    printCode(insertObjInst(createObjInst(_SUBI, TYPE_I, stackReg, stackReg, getImediato(n))));
}

InstOperand getOperandRegName(Operand op) {
    InstOperand rs, rt;
    char * regName;
    Operand operand;

    /* OPERANDO */
    if(op.kind == String) { /* Registrador */
        rs = getRegByName(op.contents.variable.name);
        if(rs == NULL) {
            if(op.contents.variable.scope == NULL) { // Scope é nulo, então é um temporário
                rs = getTempReg(escopoHead->tempRegCount++);
                insertRegistrador(createRegistrador(op, rs->enderecamento.registrador));
                printCode(insertObjInst(createObjInst(_LOAD, TYPE_I, rs, getRegByName(op.contents.variable.name), NULL)));
            } else { // Scope não é nulo, então é uma variável e deve ser lida da memória
                rs = getSavedReg(escopoHead->savedRegCount++);
                insertRegistrador(createRegistrador(op, rs->enderecamento.registrador));
                printCode(insertObjInst(createObjInst(_LOAD, TYPE_I, rs, getStackOperandLocation(op), NULL)));
            }
        }
    } else { /* Valor Imediato */
        // Lê um valor imediato em um registrador
        rt = getImediato(op.contents.val);
        rs = getTempReg(escopoHead->tempRegCount++);
        regName = (char *) toStringRegName(rs->enderecamento.registrador);
        operand.kind = String;
        operand.contents.variable.name = regName;
        operand.contents.variable.scope = NULL;
        insertRegistrador(createRegistrador(operand, rs->enderecamento.registrador));
        printCode(insertObjInst(createObjInst(_LOADI, TYPE_I, rs, rt, NULL)));
    }
    return rs;
}

InstOperand getVectorRegName(Operand op) {
    InstOperand reg = getRegByName(op.contents.variable.name);
    if(reg == NULL) {
        reg = getSavedReg(escopoHead->savedRegCount++);
        insertRegistrador(createRegistrador(op, reg->enderecamento.registrador));
        /* Lê o endereço de memória do início do vetor */
        printCode(insertObjInst(createObjInst(_LOADA, TYPE_I, reg, getStackOperandLocation(op), NULL)));
    }
    return reg;
}

InstOperand getTempRegName(Operand op) {
    InstOperand reg;
    // Cria registrador temporário para armazenar o resultado da expressão
    reg = getTempReg(escopoHead->tempRegCount++);
    insertRegistrador(createRegistrador(op, reg->enderecamento.registrador));
    return reg;
}

void geraCodigoInstrucaoAritmetica(Quadruple q, Opcode op) {
    InstOperand op1, op2, op3;

    /* Busca ou atribui o registrador do operando 1 */
    op1 = getOperandRegName(q->op1);
    /* Atribui um registrador para o resultado da expressão */
    op3 = getTempRegName(q->op3);

    /* OPERANDO 2 */
    if(q->op2.kind == String) { /* Registrador */
        /* Busca ou atribui o registrador do operando 2 */
        op2 = getOperandRegName(q->op2);
        /* Imprime a instrução aritmética */
        printCode(insertObjInst(createObjInst(op, TYPE_R, op3, op1, op2)));
    } else { /* Valor Imediato */
        // Valor imediato
        op2 = getImediato(q->op2.contents.val);
        // nextInstruction é a versão imediato da instrução atual
        int nextInstruction = op + 1;
        /* Imprime a instrução aritmética, versão imediato */
        printCode(insertObjInst(createObjInst(opcode_map[nextInstruction], TYPE_I, op3, op1, op2)));
    }
}

void geraCodigoInstrucaoLogica(Quadruple q, Opcode op, Operand label) {
    InstOperand op1, op2, op3;

    /* Busca ou atribui o registrador do operando 1 */
    op1 = getOperandRegName(q->op1);
    /* Label para o qual irá ser feito o desvio */
    op3 = getOperandLabel(label.contents.variable.name);

    /* OPERANDO 2 */
    if(q->op2.kind == String) { /* Registrador */
        /* Busca ou atribui o registrador do operando 2 */
        op2 = getOperandRegName(q->op2);
        /* Imprime a instrução aritmética */
        printCode(insertObjInst(createObjInst(op, TYPE_R, op1, op2, op3)));
    } else { /* Valor Imediato */
        // Lê o valor imediato
        op2 = getOperandRegName(q->op2);
        /* Imprime a instrução lógica */
        printCode(insertObjInst(createObjInst(op, TYPE_I, op1, op2, op3)));
    }
}

void geraCodigoInstrucaoAtribuicao(Quadruple q) {
    InstOperand reg = getOperandRegName(q->op2);
    if(q->op1.contents.variable.scope == NULL) {
        // Vetor com índice do acesso igual a uma variável
        InstOperand r = getRegByName(q->op1.contents.variable.name);
        printCode(insertObjInst(createObjInst(_STORE, TYPE_I, reg, getMemLocation(r->enderecamento.registrador), NULL)));
    } else {
        if(q->op3.kind != Empty) {
            // Vetor com índice de acesso igual a uma constante
            InstOperand r = getOperandRegName(q->op1);
            printCode(insertObjInst(createObjInst(_STORE, TYPE_I, reg, getMemIndexedLocation(r->enderecamento.registrador, q->op3.contents.val), NULL)));
        } else {
            // Variável comum
            printCode(insertObjInst(createObjInst(_STORE, TYPE_I, reg, getStackOperandLocation(q->op1), NULL)));
        }
    }
}

void geraCodigoChamadaFuncao(Quadruple q) {
    int tamanhoBlocoMemoria;
    /* Verifica o nome da função/procedimento que está sendo chamada, se for input ou output imprime as
     * instruções específicas 'in' e 'out'. Depois verifica o escopo de onde vem a chamada, se for do
     * escopo da 'main' não guarda $ra na memória, caso contrário guarda $ra na memória.
     */
    if(!strcmp(q->op1.contents.variable.name, "input")) {
        printCode(insertObjInst(createObjInst(_IN, TYPE_I, getTempRegName(q->op3), NULL, NULL)));
    } else if(!strcmp(q->op1.contents.variable.name, "output")) {
        printCode(insertObjInst(createObjInst(_OUT, TYPE_I, outputReg, NULL, NULL)));
    } else if(!strcmp(escopoHead->nome, "main")) {
        tamanhoBlocoMemoria = getTamanhoBlocoMemoriaEscopo(q->op1.contents.variable.name);
        /* Aloca o bloco de memória na stack */
        pushStackSpace(tamanhoBlocoMemoria);
        printCode(insertObjInst(createObjInst(_JUMPAL, TYPE_J, getOperandLabel(q->op1.contents.variable.name), NULL, NULL)));
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, getTempRegName(q->op3), rtnValReg, NULL)));
        /* Desaloca o bloco de memória na stack */
        popStackSpace(tamanhoBlocoMemoria);
    } else {
        tamanhoBlocoMemoria = getTamanhoBlocoMemoriaEscopo(q->op1.contents.variable.name);
        printCode(insertObjInst(createObjInst(_STORE, TYPE_I, rtnAddrReg, getStackLocation(1), NULL))); // sw $ra
        /* Aloca espaço na stack para os parâmetros + 1 para o registrador de endereço de retorno */
        pushStackSpace(tamanhoBlocoMemoria + 1); // +1 devido ao registrador $ra
        printCode(insertObjInst(createObjInst(_JUMPAL, TYPE_J, getOperandLabel(q->op1.contents.variable.name), NULL, NULL)));
        popStackSpace(tamanhoBlocoMemoria + 1); // +1 devido ao registrador $ra
        printCode(insertObjInst(createObjInst(_LOAD, TYPE_I, rtnAddrReg, getStackLocation(1), NULL))); // lw $ra
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, getTempRegName(q->op3), rtnValReg, NULL)));
    }
}

void geraCodigoSetParam(Quadruple q) {
    InstOperand reg;
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
                printCode(insertObjInst(createObjInst(_LOADA, TYPE_I, getArgReg(escopoHead->savedRegCount), getStackOperandLocation(q->op1), NULL)));
            } else { // Variável
                reg = getOperandRegName(q->op1);
                if(getArgReg(escopoHead->argRegCount)->enderecamento.registrador != reg->enderecamento.registrador) { /* Só move se os registradores forem diferentes */
                    printCode(insertObjInst(createObjInst(_MOV, TYPE_I, getArgReg(escopoHead->argRegCount), reg, NULL)));
                    moveRegistrador(getArgReg(escopoHead->argRegCount)->enderecamento.registrador, reg->enderecamento.registrador);
                }
            }
        } else { // Constante
            printCode(insertObjInst(createObjInst(_LOADI, TYPE_I, getArgReg(escopoHead->argRegCount), getImediato(q->op1.contents.val), NULL)));
        }
        escopoHead->argRegCount++;
    } else { // TODO fazer o resto
        /* Verifica se é uma constante ou variável */
        if(q->op1.kind == String) { // Variável
            if(var != NULL && var->treeNode->kind.exp == VectorK) { // Vetor
                printCode(insertObjInst(createObjInst(_LOADA, TYPE_I, getSavedReg(escopoHead->savedRegCount), getStackOperandLocation(q->op1), NULL)));
            } else { // Variável
                printCode(insertObjInst(createObjInst(_LOAD, TYPE_I, getSavedReg(escopoHead->savedRegCount), getStackOperandLocation(q->op1), NULL)));
            }
        } else { // Constante
            printCode(insertObjInst(createObjInst(_LOADI, TYPE_I, getSavedReg(escopoHead->savedRegCount), getImediato(q->op1.contents.val), NULL)));
        }
        printCode(insertObjInst(createObjInst(_STORE, TYPE_I, getSavedReg(escopoHead->savedRegCount), getStackOperandLocation(q->op1), NULL)));
        escopoHead->savedRegCount++;
    }
}

void geraCodigoGetParam(Quadruple q) {
    /* Se a chamada de função tiver até 4 parâmetros, lê os registradores $a0 - $a3
     * caso contrário, o excedente deve ser lido da stack
     */
    if(escopoHead->argRegCount < 4) {
        insertRegistrador(createRegistrador(q->op1, getArgReg(escopoHead->argRegCount)->enderecamento.registrador));
        escopoHead->argRegCount++;
    } else if(escopoHead->argRegCount >= 4) {
        insertRegistrador(createRegistrador(q->op1, getSavedReg(escopoHead->savedRegCount)->enderecamento.registrador));
        printCode(insertObjInst(createObjInst(_LOAD, TYPE_I, getSavedReg(escopoHead->savedRegCount), getStackOperandLocation(q->op1), NULL)));
        escopoHead->savedRegCount++;
    }
}

void geraCodigoRetorno(Quadruple q) {
    InstOperand reg;
    /* Só retorna valor se o escopo atual não for o escopo da main */
    if(strcmp(escopoHead->nome, "main")) {
        /* Verifica se há valor para ser retornado */
        if(q->op1.kind != Empty) {
            reg = getOperandRegName(q->op1);
            printCode(insertObjInst(createObjInst(_MOV, TYPE_I, rtnValReg, reg, NULL)));
        }
        printCode(insertObjInst(createObjInst(_JUMPR, TYPE_J, rtnAddrReg, NULL, NULL)));
    }
}

void geraCodigoVetor(Quadruple q) {
    InstOperand reg = getVectorRegName(q->op1);
    /* Verifica se o índice é constante ou variável */
    if(q->op2.kind == String) {
        /* Lê o valor da posição do vetor em um registrador temporário */
        printCode(insertObjInst(createObjInst(_LOAD, TYPE_I, getTempRegName(q->op3), getMemLocation(reg->enderecamento.registrador), NULL)));
        printCode(insertObjInst(createObjInst(_ADD, TYPE_R, getTempRegName(q->op3), reg, getOperandRegName(q->op2))));
    } else {
        printCode(insertObjInst(createObjInst(_LOAD, TYPE_I, getTempRegName(q->op3), getMemIndexedLocation(reg->enderecamento.registrador, q->op2.contents.val), NULL)));
    }
}

void geraCodigoEnderecoVetor(Quadruple q) {
    /* Lê endereço do vetor */
    InstOperand reg = getVectorRegName(q->op1);
    /* Verifica se o índice é constante ou variável */
    if(q->op2.kind == String) {
        /* Soma o endereço base do vetor com o valor da variável */
        printCode(insertObjInst(createObjInst(_ADD, TYPE_R, getTempRegName(q->op3), reg, getOperandRegName(q->op2))));
    }
}

void geraCodigoFuncao(Quadruple q) {
    // Atribui fim de string para todas posições de temp, isso é feito pois o Procedimento
    // strcat só insere de forma correta strings inicializadas.
    memset(temp, '\0', sizeof(temp));
    strcat(temp, "\n");
    strcat(temp, q->op1.contents.variable.name);
    strcat(temp, ":");
    // Adiciona o label a próxima linha de instrução
    insertLabel(q->op1.contents.variable.name, linha);
    emitCode(temp);
}

void geraCodigoLabel(Quadruple q) {
    // Atribui fim de string para todas posições de temp, isso é feito pois o Procedimento
    // strcat só insere de forma correta strings inicializadas.
    memset(temp, '\0', sizeof(temp));
    strcat(temp, ".");
    strcat(temp, q->op1.contents.variable.name);
    strcat(temp, "\t");
    // Adiciona o label a próxima linha de instrução
    insertLabel(q->op1.contents.variable.name, linha);
    emitCode(temp);
}

void printCode(Objeto instrucao) {
    char aux[20];
    memset(temp, '\0', sizeof(temp));
    sprintf(temp, "%d:\t", linha++);
    strcat(temp, toStringOpcode(instrucao->opcode));
    strcat(temp, "\t");
    if(instrucao->op1 != NULL) {
        if(instrucao->op1->tipoEnderecamento == IMEDIATO) {
            sprintf(aux, "%d", instrucao->op1->enderecamento.imediato);
            strcat(temp, aux);
        } else if(instrucao->op1->tipoEnderecamento == REGISTRADOR) {
            strcat(temp, toStringRegName(instrucao->op1->enderecamento.registrador));
        } else if(instrucao->op1->tipoEnderecamento == INDEXADO) {
            sprintf(aux, "%d(%s)", instrucao->op1->enderecamento.indexado.offset, toStringRegName(instrucao->op1->enderecamento.indexado.registrador));
            strcat(temp, aux);
        } else { // Label
            strcat(temp, instrucao->op1->enderecamento.label);
        }
    }
    if(instrucao->op2 != NULL) {
        strcat(temp, ", ");
        if(instrucao->op2->tipoEnderecamento == IMEDIATO) {
            sprintf(aux, "%d", instrucao->op2->enderecamento.imediato);
            strcat(temp, aux);
        } else if(instrucao->op2->tipoEnderecamento == REGISTRADOR) {
            strcat(temp, toStringRegName(instrucao->op2->enderecamento.registrador));
        } else if(instrucao->op2->tipoEnderecamento == INDEXADO) {
            sprintf(aux, "%d(%s)", instrucao->op2->enderecamento.indexado.offset, toStringRegName(instrucao->op2->enderecamento.indexado.registrador));
            strcat(temp, aux);
        } else { // Label
            strcat(temp, instrucao->op2->enderecamento.label);
        }
    }
    if(instrucao->op3 != NULL) {
        strcat(temp, ", ");
        if(instrucao->op3->tipoEnderecamento == IMEDIATO) {
            sprintf(aux, "%d", instrucao->op3->enderecamento.imediato);
            strcat(temp, aux);
        } else if(instrucao->op3->tipoEnderecamento == REGISTRADOR) {
            strcat(temp, toStringRegName(instrucao->op3->enderecamento.registrador));
        } else if(instrucao->op3->tipoEnderecamento == INDEXADO) {
            sprintf(aux, "%d(%s)", instrucao->op3->enderecamento.indexado.offset, toStringRegName(instrucao->op3->enderecamento.indexado.registrador));
            strcat(temp, aux);
        } else { // Label
            strcat(temp, instrucao->op3->enderecamento.label);
        }
    }
    emitObjectCode(temp, indent);
}

void geraCodigoObjeto(Quadruple q) {
    INDENT;
    emitCode("\n********** Código objeto **********");
    // Antes de começar gerar código objeto, prepara os registradores especiais
    preparaRegistradoresEspeciais();
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
                geraCodigoFuncao(q);
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
                printCode(insertObjInst(createObjInst(_JUMP, TYPE_J, getOperandLabel(q->op1.contents.variable.name), NULL, NULL)));
                break; /* GOTO */

            case LBL:
                geraCodigoLabel(q);
                break; /* LBL */

            case HALT:
                printCode(insertObjInst(createObjInst(_HALT, TYPE_J, NULL, NULL, NULL)));
                break; /* HALT */

            case PARAM_LIST:
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

Registrador createRegistrador(Operand op, RegisterName regName) {
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

void moveRegistrador(RegisterName dest, RegisterName orig) {
    Registrador origem = getRegistrador(orig);
    origem->regName = dest;
    removeRegistrador(dest);
}

void removeRegistrador(RegisterName name) {
    Registrador atual, anterior;
    if(escopoHead != NULL) {
        atual = escopoHead->regList;
    }
    /* Verifica se o primeiro deve ser removido */
    if(name == atual->regName) {
        escopoHead->regList = atual->next;
        free(atual);
        atual = NULL;
        return;
    }

    anterior = atual;
    atual = atual->next;

    /* Verifica o restante */
    while(atual != NULL && anterior != NULL) {
        if(name == atual->regName) {
            anterior->next = atual->next;
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->next;
    }
}

Registrador getRegistrador(RegisterName name) {
    Registrador reg;
    if(escopoHead != NULL) {
        reg = escopoHead->regList;
    }
    while(reg != NULL) {
        if(name == reg->regName) {
            return reg;
        }
        reg = reg->next;
    }
    return NULL;
}

InstOperand getRegByName(char * name) {
    Registrador reg;
    if(escopoHead != NULL) {
        reg = escopoHead->regList;
    }
    while(reg != NULL) {
        if(!strcmp(name, reg->op.contents.variable.name)) {
            InstOperand operando = (InstOperand) malloc(sizeof(struct instOperand));
            operando->tipoEnderecamento = REGISTRADOR;
            operando->enderecamento.registrador = reg->regName;
            return operando;
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

Objeto createObjInst(Opcode opcode, Type type, InstOperand op1, InstOperand op2, InstOperand op3) {
    Objeto obj = (Objeto) malloc(sizeof(struct objeto));
    obj->opcode = opcode;
    obj->type = type;
    obj->op1 = op1;
    obj->op2 = op2;
    obj->op3 = op3;
    obj->next = NULL;
    return obj;
}

Objeto insertObjInst(Objeto obj) {
    if(objHead == NULL) {
        objHead = obj;
    } else {
        Objeto temp = objHead;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = obj;
    }
    return obj;
}

void preparaRegistradoresEspeciais(void) {
    // Registrador de endereço de retorno de função
    rtnAddrReg = (InstOperand) malloc(sizeof(struct instOperand));
    rtnAddrReg->tipoEnderecamento = REGISTRADOR;
    rtnAddrReg->enderecamento.registrador = $ra;
    // Registrador de valor de retorno
    rtnValReg = (InstOperand) malloc(sizeof(struct instOperand));
    rtnValReg->tipoEnderecamento = REGISTRADOR;
    rtnValReg->enderecamento.registrador = $v0;
    // Registrador da stack
    stackReg = (InstOperand) malloc(sizeof(struct instOperand));
    stackReg->tipoEnderecamento = REGISTRADOR;
    stackReg->enderecamento.registrador = $sp;
    // Registrador para output
    outputReg = (InstOperand) malloc(sizeof(struct instOperand));
    outputReg->tipoEnderecamento = REGISTRADOR;
    outputReg->enderecamento.registrador = $out;
    // Registrador zero
    rzero = (InstOperand) malloc(sizeof(struct instOperand));
    rzero->tipoEnderecamento = REGISTRADOR;
    rzero->enderecamento.registrador = $rz;
}

InstOperand getImediato(int valor) {
    InstOperand imediato = (InstOperand) malloc(sizeof(struct instOperand));
    imediato->tipoEnderecamento = IMEDIATO;
    imediato->enderecamento.imediato = valor;
    return imediato;
}

InstOperand getOperandLabel(char * name) {
    InstOperand label = (InstOperand) malloc(sizeof(struct instOperand));
    label->tipoEnderecamento = LABEL;
    label->enderecamento.label = name;
    return label;
}

Label createLabel(char * nome, int linha) {
    Label l = (Label) malloc(sizeof(struct label));
    l->nome = nome;
    l->linha = linha;
    l->next = NULL;
    return l;
}

void insertLabel(char * nome, int linha) {
    Label l = createLabel(nome, linha);
    if(labelHead == NULL) {
        labelHead = l;
    } else {
        Label temp = labelHead;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = l;
    }
}

int getLinhaLabel(char * nome) {
    Label l;
    if(labelHead != NULL) {
        l = labelHead;
    }
    while(l != NULL) {
        if(!strcmp(nome, l->nome)) {
            return linha;
        }
        l = l->next;
    }
    return -1;
}

Objeto getCodigoObjeto(void) {
    return objHead;
}
