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

#define QTD_REG 32

#define QTD_ARG_REG 4
#define REG_ARG_INICIO 6
#define REG_ARG_FIM 9
#define SHIFT_COUNTER_REG_ARG 6

#define REG_SAVED_INICIO 10
#define REG_SAVED_FIM 19
#define SHIFT_COUNTER_REG_SAVED 10

#define REG_TEMP_INICIO 20
#define REG_TEMP_FIM 29
#define SHIFT_COUNTER_REG_TEMP 20

#define REG_ZERO 0
#define REG_RTN_ADDR 31
#define REG_RTN_VAL 1
#define REG_STACK 30
#define REG_OUTPUT 3
#define REG_GLOBAL 5
#define REG_INVALID 4

/* Cabeça da lista de instruções objeto */
Objeto objHead = NULL;

/* Cabeça da lista de labels */
Label labelHead = NULL;

/* Escopo atual */
Escopo escopo = NULL;

/* Variável usada para configurar a identação do código impresso */
static int indent = 0;

/* Variáveis auxiliares na geração de código objeto */
static char temp[100];
static int linha = 0;

 /* Operandos especiais */
TargetOperand rtnAddrOp;
TargetOperand rtnValOp;
TargetOperand stackOp;
TargetOperand outputOp;
TargetOperand rZeroOp;
TargetOperand globalOp;

Registrador registradores[QTD_REG];
RegisterName regNames[QTD_REG] = {
    $rz, $v0, $v1, $out, $inv, $gp, $a0, $a1,
    $a2, $a3, $s0, $s1, $s2, $s3, $s4, $s5,
    $s6, $s7, $s8, $s9, $t0, $t1, $t2, $t3,
    $t4, $t5, $t6, $t7, $t8, $t9, $sp, $ra
};

Opcode funcToOpcode(Function func) {
    switch (func) {
        case _ADD:  return _ADDI;
        case _SUB:  return _SUBI;
        case _MUL:  return _MULI;
        case _DIV:  return _DIVI;
        case _MOD:  return _MODI;
        case _AND:  return _ANDI;
        case _OR:   return _ORI;
        case _XOR:  return _XORI;
        case _LAND: return _LANDI;
        case _LOR:  return _LORI;
        case _SLL:  return _SLLI;
        case _SRL:  return _SRLI;
        /* Não deve chegar aqui nunca */
        default: return _HALT;
    }
}

TargetOperand getMemLocation(RegisterName registrador) {
    // Operando que representa o modo de endereçamento indexado
    TargetOperand operando = (TargetOperand) malloc(sizeof(struct targetOperand));
    operando->tipoEnderecamento = INDEXADO;
    operando->enderecamento.indexado.offset = 0;
    operando->enderecamento.indexado.registrador = registrador;
    return operando;
}

TargetOperand getMemIndexedLocation(RegisterName registrador, int offset) {
    // Operando que representa o modo de endereçamento indexado
    TargetOperand operando = (TargetOperand) malloc(sizeof(struct targetOperand));
    operando->tipoEnderecamento = INDEXADO;
    operando->enderecamento.indexado.offset = offset;
    operando->enderecamento.indexado.registrador = registrador;
    return operando;
}

/* Busca a posição de memória do operando op, e retorna sua posição com offset na stack */
TargetOperand getStackOperandLocation(Operand op) {
    int memloc = getMemoryLocation(op->contents.variable.name, op->contents.variable.scope);
    int offset = memloc - (escopo->tamanhoBlocoMemoria - 1);
    // Operando que representa o modo de endereçamento indexado
    TargetOperand operando = (TargetOperand) malloc(sizeof(struct targetOperand));
    operando->tipoEnderecamento = INDEXADO;
    operando->enderecamento.indexado.offset = offset;
    operando->enderecamento.indexado.registrador = regNames[REG_STACK];
    return operando;
}

TargetOperand getGlobalOperandLocation(Operand op) {
    int offset = getMemoryLocation(op->contents.variable.name, op->contents.variable.scope);
    // Operando que representa o modo de endereçamento indexado
    TargetOperand operando = (TargetOperand) malloc(sizeof(struct targetOperand));
    operando->tipoEnderecamento = INDEXADO;
    operando->enderecamento.indexado.offset = offset;
    operando->enderecamento.indexado.registrador = regNames[REG_GLOBAL];
    return operando;
}

/* Retorna a posição da stack de acordo com o offset passado por parâmetro */
TargetOperand getStackLocation(int offset) {
    // Operando que representa o modo de endereçamento indexado
    TargetOperand operando = (TargetOperand) malloc(sizeof(struct targetOperand));
    operando->tipoEnderecamento = INDEXADO;
    operando->enderecamento.indexado.offset = offset;
    operando->enderecamento.indexado.registrador = regNames[REG_STACK];
    return operando;
}

void pushStackSpace(int n) {
    printCode(insertObjInst(createObjInst(_ADDI, TYPE_I, stackOp, stackOp, getImediato(n))));
}

void popStackSpace(int n) {
    printCode(insertObjInst(createObjInst(_SUBI, TYPE_I, stackOp, stackOp, getImediato(n))));
}

TargetOperand getArgReg(int i) {
    TargetOperand operando = (TargetOperand) malloc(sizeof(struct targetOperand));
    operando->tipoEnderecamento = REGISTRADOR;

    if(i < QTD_ARG_REG) {
        // Calcula a posição do registrador no banco de registradores
        int position = SHIFT_COUNTER_REG_ARG + i;

        operando->enderecamento.registrador = regNames[position];

        Registrador reg = registradores[position];
        reg.op->opTarget = operando;
        return reg.op->opTarget;
    }

    operando->enderecamento.registrador = regNames[REG_INVALID];
    Registrador reg = registradores[REG_INVALID];
    reg.op->opTarget = operando;
    return reg.op->opTarget;
}

TargetOperand getTempReg(Operand op) {
    /* Se já tiver usado todos registradores temporários volta a usar do início
     * sem fazer nenhuma verificação adicional, pois os registradores temporários
     * não garantem persistência dos dados
     */
    if(escopo->tempRegCount > 9) {
        escopo->tempRegCount = 0;
    }

    // Calcula a posição do registrador no banco de registradores
    int position = SHIFT_COUNTER_REG_TEMP + escopo->savedRegCount;

    Registrador reg = registradores[position];
    reg.op = op;
    return reg.op->opTarget;
}

TargetOperand getSavedReg(Operand op) {
    /* Se já tiver usado todos registradores temporários volta a usar do início,
     * mas antes de usar o registrador deve salvar seu valor antigo na memória
     */
    if(escopo->savedRegCount > 9) {
        escopo->savedRegCount = 0;
    }

    // Calcula a posição do registrador no banco de registradores
    int position = SHIFT_COUNTER_REG_SAVED + escopo->savedRegCount;

    Registrador reg = registradores[position];
    reg.op = op;
    return reg.op->opTarget;
}

TargetOperand getOperandRegName(Operand op) {
    TargetOperand rs, rt;

    /* OPERANDO */
    if(op->kind == String) { /* Registrador */
        rs = getTargetOpByName(op->contents.variable.name);
        if(rs == NULL) {
            if(op->contents.variable.scope == NULL) { // Scope é nulo, então é um temporário
                rs = getTempReg(op);
                rt = getTargetOpByName(op->contents.variable.name);
            } else if(op->contents.variable.scope == globalScope) {
                rs = getSavedReg(op);
                rt = getGlobalOperandLocation(op);
            } else { // Scope não é nulo, então é uma variável e deve ser lida da memória
                rs = getSavedReg(op);
                rt = getStackOperandLocation(op);
            }
            printCode(insertObjInst(createObjInst(_LW, TYPE_I, rs, rt, NULL)));
        }
    } else { /* Valor Imediato */
        // Prepara o operando
        Operand operand = (Operand) malloc(sizeof(struct operand));
        operand->kind = String;
        operand->contents.variable.scope = NULL;
        // Obtém um registrador temporário
        rs = getTempReg(operand);
        // Aloca o nome do registrador no operando
        operand->contents.variable.name = (char *) toStringRegName(rs->enderecamento.registrador);
        // Lê um valor imediato em um registrador
        rt = getImediato(op->contents.val);
        printCode(insertObjInst(createObjInst(_LI, TYPE_I, rs, rt, NULL)));
    }
    return rs;
}

TargetOperand getVectorRegName(Operand op) {
    TargetOperand reg = getTargetOpByName(op->contents.variable.name);
    TreeNode * treeNode;
    if(reg == NULL) {
        reg = getSavedReg(op);
        if(op->contents.variable.scope == globalScope) {
            /* Lê o endereço de memória do início do vetor */
            printCode(insertObjInst(createObjInst(_LA, TYPE_I, reg, getGlobalOperandLocation(op), NULL)));
        } else {
            // Verifica se o vetor foi declarado no mesmo escopo atual ou veio como parâmetro
            treeNode = getVarFromSymtab(op->contents.variable.name, op->contents.variable.scope)->treeNode;
            if(treeNode->kind.var.mem == PARAMK) {
                /* Parâmetro - Lê o ponteiro para o vetor */
                printCode(insertObjInst(createObjInst(_LW, TYPE_I, reg, getStackOperandLocation(op), NULL)));
            } else {
                /* Escopo atual - Lê o endereço de memória do início do vetor */
                printCode(insertObjInst(createObjInst(_LA, TYPE_I, reg, getStackOperandLocation(op), NULL)));
            }
        }
    }
    return reg;
}

void geraCodigoInstrucaoTipoI(Quadruple q, Function func) {
    TargetOperand op1, op2, op3;
    /* Busca ou atribui o registrador do operando 1 */
    op1 = getOperandRegName(q->op1);
    /* Atribui um registrador para o resultado da expressão */
    op3 = getTempReg(q->op3);
    // Valor imediato
    op2 = getImediato(q->op2->contents.val);
    /* Imprime a instrução aritmética versão imediato */
    printCode(insertObjInst(createObjInst(funcToOpcode(func), TYPE_I, op3, op1, op2)));
}

void geraCodigoInstrucaoTipoR(Quadruple q, Function func) {
    // Verifica se é uma instrução aritmética RTYPE
    if(q->op2->kind == String) { /* Registrador */
        TargetOperand op1, op2, op3;
        /* Busca ou atribui o registrador do operando 1 */
        op1 = getOperandRegName(q->op1);
        /* Atribui um registrador para o resultado da expressão */
        op3 = getTempReg(q->op3);
        /* Busca ou atribui o registrador do operando 2 */
        op2 = getOperandRegName(q->op2);
        /* Imprime a instrução aritmética */
        printCode(insertObjInst(createObjInstTypeR(_RTYPE, func, TYPE_R, op3, op1, op2)));
    } else { /* Imediato */
        geraCodigoInstrucaoTipoI(q, func);
    }
}

void geraCodigoInstrucaoRelacional(Quadruple q, Function func) {
    TargetOperand op1, op2, op3;
    /* Busca ou atribui o registrador do operando 1 */
    op1 = getOperandRegName(q->op1);
    /* Atribui um registrador para o resultado da expressão */
    op3 = getTempReg(q->op3);
    /* Busca ou atribui o registrador do operando 2 */
    op2 = getOperandRegName(q->op2);
    /* Imprime a instrução relacional */
    printCode(insertObjInst(createObjInstTypeR(_RTYPE, func, TYPE_R, op3, op1, op2)));
}

void geraCodigoInstrucaoAtribuicao(Quadruple q) {
    TargetOperand reg = getOperandRegName(q->op2);
    if(q->op1->contents.variable.scope == NULL) {
        // Vetor com índice do acesso igual a uma variável
        TargetOperand r = getTargetOpByName(q->op1->contents.variable.name);
        printCode(insertObjInst(createObjInst(_SW, TYPE_I, reg, getMemLocation(r->enderecamento.registrador), NULL)));
    } else if(q->op1->contents.variable.scope == globalScope) {
        if(q->op3 != NULL) {
            // Vetor com índice de acesso igual a uma constante
            TargetOperand r = getOperandRegName(q->op1);
            printCode(insertObjInst(createObjInst(_SW, TYPE_I, reg, getMemIndexedLocation(r->enderecamento.registrador, q->op3->contents.val), NULL)));
        } else {
            // Variável comum
            printCode(insertObjInst(createObjInst(_SW, TYPE_I, reg, getGlobalOperandLocation(q->op1), NULL)));
            /* Remove o registrador da lista, para forçar um novo LOAD ao usar a variável que foi recentemente alterada na memória */
            TargetOperand regAux = getTargetOpByName(q->op1->contents.variable.name);
            if(regAux != NULL) {
                //removeRegistrador(regAux->enderecamento.registrador);
            }
        }
    } else {
        if(q->op3 != NULL) {
            // Vetor com índice de acesso igual a uma constante
            TargetOperand r = getOperandRegName(q->op1);
            printCode(insertObjInst(createObjInst(_SW, TYPE_I, reg, getMemIndexedLocation(r->enderecamento.registrador, q->op3->contents.val), NULL)));
        } else {
            // Variável comum
            printCode(insertObjInst(createObjInst(_SW, TYPE_I, reg, getStackOperandLocation(q->op1), NULL)));
            /* Remove o registrador da lista, para forçar um novo LOAD ao usar a variável que foi recentemente alterada na memória */
            TargetOperand regAux = getTargetOpByName(q->op1->contents.variable.name);
            if(regAux != NULL) {
                //removeRegistrador(regAux->enderecamento.registrador);
            }
        }
    }
}

void geraCodigoChamadaFuncao(Quadruple q) {
    int tamanhoBlocoMemoria;
    /* Verifica o nome da função/procedimento que está sendo chamada, se for input ou output imprime as
     * instruções específicas 'in' e 'out'. Depois verifica o escopo de onde vem a chamada, se for do
     * escopo da 'main' não guarda $ra na memória, caso contrário guarda $ra na memória.
     */
    if(!strcmp(q->op1->contents.variable.name, "input")) {
        printCode(insertObjInst(createObjInst(_IN, TYPE_I, getTempReg(q->op3), NULL, NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "output")) {
        printCode(insertObjInst(createObjInst(_OUT, TYPE_I, getArgReg(0), NULL, getImediato(q->display))));
    } else if(!strcmp(escopo->nome, "main")) {
        tamanhoBlocoMemoria = getTamanhoBlocoMemoriaEscopo(q->op1->contents.variable.name);
        printCode(insertObjInst(createObjInst(_JAL, TYPE_J, getOperandLabel(q->op1->contents.variable.name), NULL, NULL)));
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, getSavedReg(q->op3), rtnValOp, NULL)));
        /* Desaloca o bloco de memória na stack */
        popStackSpace(tamanhoBlocoMemoria + 2); // +1 devido ao registrador $ra / +1 devido retorno de função
    } else {
        tamanhoBlocoMemoria = getTamanhoBlocoMemoriaEscopo(q->op1->contents.variable.name);
        saveRegistradores();
        printCode(insertObjInst(createObjInst(_JAL, TYPE_J, getOperandLabel(q->op1->contents.variable.name), NULL, NULL)));
        /* Desaloca o bloco de memória na stack */
        popStackSpace(tamanhoBlocoMemoria + 2); // +1 devido ao registrador $ra / +1 devido retorno de função
        recuperaRegistradores();
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, getSavedReg(q->op3), rtnValOp, NULL)));
    }
}

void geraCodigoSetParam(Quadruple q) {
    TargetOperand reg;
    BucketList var = NULL;
    /* Verifica se é uma variável para recuperar o BucketList correspondente */
    if(q->op1->kind == String && q->op1->contents.variable.scope != NULL) {
        // Recupera o BucketList da variável
        var = getVarFromSymtab(q->op1->contents.variable.name, q->op1->contents.variable.scope);
    }
    /* Se a chamada de função tiver até 4 parâmetros, utiliza os registradores $a0 - $a3
     * caso contrário, o excedente deve ser armazenado na stack
     */
    if(escopo->argRegCount < 4) {
        /* Verifica se é uma constante ou variável */
        if(q->op1->kind == String) { // Variável
            if(var != NULL && var->treeNode->node == VARK && var->treeNode->kind.var.varKind == VECTORK) { // Vetor
                printCode(insertObjInst(createObjInst(_LA, TYPE_I, getArgReg(escopo->argRegCount), getStackOperandLocation(q->op1), NULL)));
            } else { // Variável
                reg = getOperandRegName(q->op1);
                if(getArgReg(escopo->argRegCount)->enderecamento.registrador != reg->enderecamento.registrador) { /* Só move se os registradores forem diferentes */
                    printCode(insertObjInst(createObjInst(_MOV, TYPE_I, getArgReg(escopo->argRegCount), reg, NULL)));
                    //removeRegistrador(getArgReg(escopo->argRegCount)->enderecamento.registrador);
                }
            }
        } else { // Constante
            printCode(insertObjInst(createObjInst(_LI, TYPE_I, getArgReg(escopo->argRegCount), getImediato(q->op1->contents.val), NULL)));
        }
        escopo->argRegCount++;
    } else { // TODO fazer o resto
        TargetOperand rs = getSavedReg(q->op1);
        TargetOperand rt;
        Opcode opcode;
        /* Verifica se é uma constante ou variável */
        if(q->op1->kind == String) { // Variável
            if(var != NULL && var->treeNode->node == VARK && var->treeNode->kind.var.varKind == VECTORK) { // Vetor
                opcode = _LA;
            } else { // Variável
                opcode = _LW;
            }
            rt = getStackOperandLocation(q->op1);
        } else { // Constante
            opcode = _LI;
            rt = getImediato(q->op1->contents.val);
        }
        printCode(insertObjInst(createObjInst(opcode, TYPE_I, rs, rt, NULL)));
        printCode(insertObjInst(createObjInst(_SW, TYPE_I, rs, getStackOperandLocation(q->op1), NULL)));
    }
}

void geraCodigoGetParam(Quadruple q) {
    /* Se a chamada de função tiver até 4 parâmetros, lê os registradores $a0 - $a3
     * caso contrário, o excedente deve ser lido da stack
     */
    if(escopo->argRegCount < 4) {
        TargetOperand arg = getArgReg(escopo->argRegCount);
        printCode(insertObjInst(createObjInst(_SW, TYPE_I, arg, getStackOperandLocation(q->op1), NULL)));
        //removeRegistrador(arg->enderecamento.registrador);
        escopo->argRegCount++;
    } else if(escopo->argRegCount >= 4) {
        TargetOperand savedReg = getSavedReg(q->op1);
        printCode(insertObjInst(createObjInst(_LW, TYPE_I, savedReg, getStackOperandLocation(q->op1), NULL)));
    }
}

void geraCodigoRetorno(Quadruple q) {
    TargetOperand reg;
    /* Só retorna valor se o escopo atual não for o escopo da main */
    if(strcmp(escopo->nome, "main")) {
        /* Verifica se há valor para ser retornado */
        if(q->op1 != NULL) {
            reg = getOperandRegName(q->op1);
            printCode(insertObjInst(createObjInst(_MOV, TYPE_I, rtnValOp, reg, NULL)));
        }
        printCode(insertObjInst(createObjInstTypeR(_RTYPE, _JR, TYPE_R, rtnAddrOp, NULL, NULL)));
    }
}

void geraCodigoVetor(Quadruple q) {
    TargetOperand reg = getVectorRegName(q->op1);
    /* Verifica se o índice é constante ou variável */
    if(q->op2->kind == String) {
        /* Lê o valor da posição do vetor em um registrador temporário */
        TargetOperand temp = getTempReg(q->op3);
        printCode(insertObjInst(createObjInstTypeR(_RTYPE, _ADD, TYPE_R, temp, reg, getOperandRegName(q->op2))));
        printCode(insertObjInst(createObjInst(_LW, TYPE_I, temp, getMemIndexedLocation(temp->enderecamento.registrador, 0), NULL)));
    } else {
        printCode(insertObjInst(createObjInst(_LW, TYPE_I, getTempReg(q->op3), getMemIndexedLocation(reg->enderecamento.registrador, q->op2->contents.val), NULL)));
    }
}

void geraCodigoEnderecoVetor(Quadruple q) {
    /* Lê endereço do vetor */
    TargetOperand reg = getVectorRegName(q->op1);
    /* Verifica se o índice é constante ou variável */
    if(q->op2->kind == String) {
        /* Soma o endereço base do vetor com o valor da variável */
        printCode(insertObjInst(createObjInstTypeR(_RTYPE, _ADD, TYPE_R, getTempReg(q->op3), reg, getOperandRegName(q->op2))));
    }
}

void geraCodigoFuncao(Quadruple q) {
    // Atribui fim de string para todas posições de temp, isso é feito pois o Procedimento
    // strcat só insere de forma correta strings inicializadas.
    memset(temp, '\0', sizeof(temp));
    strcat(temp, "\n");
    strcat(temp, q->op1->contents.variable.name);
    strcat(temp, ":");
    // Adiciona o label a próxima linha de instrução
    insertLabel(q->op1->contents.variable.name, linha);
    emitCode(temp);
    pushEscopo(createEscopo(q->op1->contents.variable.name));

    if(!strcmp(escopo->nome, "main")) {
        int tamanho = getTamanhoBlocoMemoriaEscopoGlobal();
        /* Aloca o bloco de memória na stack */
        pushStackSpace(escopo->tamanhoBlocoMemoria + tamanho);
    } else {
        /* Aloca espaço na stack para os parâmetros + 1 para o registrador de endereço de retorno
         * + 1 para armazenar valor de retorno de função dentro do escopo
         */
        pushStackSpace(escopo->tamanhoBlocoMemoria + 2); // +1 devido ao registrador $ra
    }
}

void geraCodigoDesvio(Quadruple q) {
    TargetOperand op1 = getOperandRegName(q->op1);
    TargetOperand op2 = getOperandLabel(q->op2->contents.variable.name);
    printCode(insertObjInst(createObjInst(_JF, TYPE_I, op1, op2, NULL)));
}

void geraCodigoLabel(Quadruple q) {
    // Atribui fim de string para todas posições de temp, isso é feito pois o Procedimento
    // strcat só insere de forma correta strings inicializadas.
    memset(temp, '\0', sizeof(temp));
    strcat(temp, ".");
    strcat(temp, q->op1->contents.variable.name);
    strcat(temp, "\t");
    // Adiciona o label a próxima linha de instrução
    insertLabel(q->op1->contents.variable.name, linha);
    emitCode(temp);
}

void geraCodigoObjeto(Quadruple q) {
    INDENT;
    emitCode("\n********** Código objeto **********");
    // Antes de começar gerar código objeto, prepara os registradores especiais
    prepararRegistradores();
    prepararOperandosEspeciais();
    while(q != NULL) {
        switch (q->instruction) {
            /* Aritméticas */
            case ADD:
                geraCodigoInstrucaoTipoR(q, _ADD);
                break;
            case SUB:
                geraCodigoInstrucaoTipoR(q, _SUB);
                break;
            case MULT:
                geraCodigoInstrucaoTipoR(q, _MUL);
                break;
            case DIV:
                geraCodigoInstrucaoTipoR(q, _DIV);
                break;
            case MOD:
                geraCodigoInstrucaoTipoR(q, _MOD);
                break;
            case SHFT_LF:
                geraCodigoInstrucaoTipoR(q, _SLL);
                break;
            case SHFT_RT:
                geraCodigoInstrucaoTipoR(q, _SRL);
                break;

            /* Lógicas */
            case BITW_AND:
                geraCodigoInstrucaoTipoR(q, _AND);
                break;
            case BITW_OR:
                geraCodigoInstrucaoTipoR(q, _OR);
                break;
            case BITW_XOR:
                geraCodigoInstrucaoTipoR(q, _XOR);
                break;
            case BITW_NOT:
                // FIXME
                //geraCodigoInstrucaoAritmeticaLogica(q, _NOT);
                break;
            case LOGIC_AND:
                geraCodigoInstrucaoTipoR(q, _LAND);
                break;
            case LOGIC_OR:
                geraCodigoInstrucaoTipoR(q, _LOR);
                break;

            default: break;

            /* Relacionais */
            case EQ:
                geraCodigoInstrucaoRelacional(q, _EQ);
                break;
            case NE:
                geraCodigoInstrucaoRelacional(q, _NE);
                break;
            case LT:
                geraCodigoInstrucaoRelacional(q, _LT);
                break;
            case LET:
                geraCodigoInstrucaoRelacional(q, _LET);
                break;
            case GT:
                geraCodigoInstrucaoRelacional(q, _GT);
                break;
            case GET:
                geraCodigoInstrucaoRelacional(q, _GET);
                break;

            /* Atribuição */
            case ASN:
                geraCodigoInstrucaoAtribuicao(q);
                break;

            /* Vetores */
            case VEC:
                geraCodigoVetor(q);
                break;
            case VEC_ADDR:
                geraCodigoEnderecoVetor(q);
                break;

            /* Funções */
            case FUNC:
                geraCodigoFuncao(q);
                escopo->savedRegCount = 0;
                break;
            case RTN:
                geraCodigoRetorno(q);
                break;
            case GET_PARAM:
                geraCodigoGetParam(q);
                break;
            case SET_PARAM:
                geraCodigoSetParam(q);
                break;
            case CALL:
                geraCodigoChamadaFuncao(q);
                escopo->argRegCount = 0;
                break;
            case PARAM_LIST:
                escopo->argRegCount = 0;
                break;

            /* Saltos */
            case GOTO:
                printCode(insertObjInst(createObjInst(_J, TYPE_J, getOperandLabel(q->op1->contents.variable.name), NULL, NULL)));
                break;
            case JPF:
                geraCodigoDesvio(q);
                break;

            /* Label */
            case LBL:
                geraCodigoLabel(q);
                break;

            /* Halt */
            case HALT:
                printCode(insertObjInst(createObjInst(_HALT, TYPE_J, NULL, NULL, NULL)));
                break;
        }
        q = q->next;
    }
}

void printCode(Objeto instrucao) {
    char aux[20];
    memset(temp, '\0', sizeof(temp));
    sprintf(temp, "%d:\t", linha++);
    if(instrucao->func == _DONT_CARE) {
        strcat(temp, toStringOpcode(instrucao->opcode));
    } else {
        strcat(temp, toStringFunction(instrucao->func));
    }
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

Escopo createEscopo(const char * nome) {
    Escopo e = (Escopo) malloc(sizeof(struct escopo));
    e->argRegCount = 0;
    e->savedRegCount = 0;
    e->tempRegCount = 0;
    // Recupera o BucketList do escopo
    BucketList bl = st_bucket_func((char *) nome);
    /* Recupera o treeNode que representa o escopo para recuperar a quantidade de
     * parâmetros e variáveis
     */
    e->quantidadeParametros = getQuantidadeParametros(bl->treeNode);
    e->quantidadeVariaveis = getQuantidadeVariaveis(bl->treeNode);
    e->tamanhoBlocoMemoria = getTamanhoBlocoMemoriaEscopo((char *) nome);
    e->nome = nome;
    e->next = NULL;
    return e;
}

void pushEscopo(Escopo e) {
    if(escopo == NULL) {
        escopo = e;
    } else {
        e->next = escopo;
        escopo = e;
    }
}

TargetOperand getTargetOpByName(char * name) {
    int i;

    for(i = 0; i < QTD_REG; i++) {
        Registrador reg = registradores[i];

        if(reg.op != NULL) {
            if(reg.op->kind == String) {
                char * varName = reg.op->contents.variable.name;
                if(varName != NULL && !strcmp(name, varName)) {
                    return reg.op->opTarget;
                }
            }
        }
    }
    return NULL;
}

void saveRegistradores(void) {
    printCode(insertObjInst(createObjInst(_SW, TYPE_I, rtnAddrOp, getStackLocation(1), NULL))); // sw $ra

    /*Registrador reg;
    if(escopo != NULL) {
        reg = escopo->regList;
    }

    while(reg != NULL) {
        if(toStringRegName(reg->TargetOperand->enderecamento.registrador)[1] == 115) {
            TargetOperand rt;
            if(reg->op->contents.variable.scope == NULL) {
                rt = getStackLocation(2); // sw function return
            } else {
                rt = getStackOperandLocation(&reg->op);
            }
            printCode(insertObjInst(createObjInst(_SW, TYPE_I, reg->TargetOperand, rt, NULL)));
        }
        reg = reg->next;
    }*/
}

void recuperaRegistradores(void) {
    printCode(insertObjInst(createObjInst(_LW, TYPE_I, rtnAddrOp, getStackLocation(1), NULL))); // lw $ra

    /*Registrador reg;
    if(escopo != NULL) {
        reg = escopo->regList;
    }

    while(reg != NULL) {
        if(toStringRegName(reg->TargetOperand->enderecamento.registrador)[1] == 115) {
            TargetOperand rt;
            if(reg->op->contents.variable.scope == NULL) {
                rt = getStackLocation(2); // sw function return
            } else {
                rt = getStackOperandLocation(&reg->op);
            }
            printCode(insertObjInst(createObjInst(_LW, TYPE_I, reg->TargetOperand, rt, NULL)));
        }
        reg = reg->next;
    }*/
}

Objeto createObjInstTypeR(Opcode opcode, Function func, Type type, TargetOperand op1, TargetOperand op2, TargetOperand op3) {
    Objeto obj = (Objeto) malloc(sizeof(struct objeto));
    obj->opcode = opcode;
    obj->func = func;
    obj->type = type;
    obj->op1 = op1;
    obj->op2 = op2;
    obj->op3 = op3;
    obj->next = NULL;
    return obj;
}

Objeto createObjInst(Opcode opcode, Type type, TargetOperand op1, TargetOperand op2, TargetOperand op3) {
    Objeto obj = (Objeto) malloc(sizeof(struct objeto));
    obj->opcode = opcode;
    obj->func = _DONT_CARE;
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

TargetOperand getImediato(int val) {
    TargetOperand imediato = (TargetOperand) malloc(sizeof(struct targetOperand));
    imediato->tipoEnderecamento = IMEDIATO;
    imediato->enderecamento.imediato = val;
    return imediato;
}

TargetOperand getOperandLabel(char * name) {
    TargetOperand label = (TargetOperand) malloc(sizeof(struct targetOperand));
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
            return l->linha + 1; // +1 útil no código binário
        }
        l = l->next;
    }
    return -1;
}

void prepararRegistradores(void) {
    int i;

    for(i = REG_ARG_INICIO; i < REG_ARG_FIM; i++) {
        Registrador reg;
        reg.op = NULL;
        reg.regName = regNames[i];
        registradores[i] = reg;
    }

    for(i = REG_SAVED_INICIO; i < REG_SAVED_FIM; i++) {
        Registrador reg;
        reg.op = NULL;
        reg.regName = regNames[i];
        registradores[i] = reg;
    }

    for(i = REG_TEMP_INICIO; i < REG_TEMP_FIM; i++) {
        Registrador reg;
        reg.op = NULL;
        reg.regName = regNames[i];
        registradores[i] = reg;
    }

    // Registrador de endereço de retorno de função
    Registrador rtnAddrReg;
    rtnAddrReg.op = NULL;
    rtnAddrReg.regName = regNames[REG_RTN_ADDR];
    registradores[REG_RTN_ADDR] = rtnAddrReg;

    // Registrador de valor de retorno
    Registrador rtnValReg;
    rtnValReg.op = NULL;
    rtnValReg.regName = regNames[REG_RTN_VAL];
    registradores[REG_RTN_VAL] = rtnValReg;

    // Registrador da stack
    Registrador stackReg;
    stackReg.op = NULL;
    stackReg.regName = regNames[REG_STACK];
    registradores[REG_STACK] = stackReg;

    // Registrador para output
    Registrador outputReg;
    outputReg.op = NULL;
    outputReg.regName = regNames[REG_OUTPUT];
    registradores[REG_OUTPUT] = outputReg;

    // Registrador zero
    Registrador rzero;
    rzero.op = NULL;
    rzero.regName = regNames[REG_ZERO];
    registradores[REG_ZERO] = rzero;

    // Registrador para valores globais
    Registrador globalReg;
    globalReg.op = NULL;
    globalReg.regName = regNames[REG_GLOBAL];
    registradores[REG_GLOBAL] = globalReg;
}

void prepararOperandosEspeciais(void) {
    rtnAddrOp = registradores[REG_RTN_ADDR].op->opTarget;
    rtnValOp = registradores[REG_RTN_VAL].op->opTarget;
    stackOp = registradores[REG_STACK].op->opTarget;
    outputOp = registradores[REG_OUTPUT].op->opTarget;
    rZeroOp = registradores[REG_ZERO].op->opTarget;
    globalOp = registradores[REG_GLOBAL].op->opTarget;
}

Objeto getCodigoObjeto(void) {
    return objHead;
}
