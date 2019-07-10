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
#define REG_ARG_INICIO 1
#define REG_ARG_FIM 4
#define SHIFT_COUNTER_REG_ARG 1

#define REG_SAVED_INICIO 5
#define REG_SAVED_FIM 14
#define SHIFT_COUNTER_REG_SAVED 5

#define REG_TEMP_INICIO 15
#define REG_TEMP_FIM 23
#define SHIFT_COUNTER_REG_TEMP 15

#define REG_ZERO 0
#define REG_RTN_VAL 24
#define REG_KERNEL_0 25
#define REG_KERNEL_1 26
#define REG_GLOBAL_BAK 27
#define REG_STACK_BAK 28
#define REG_GLOBAL 29
#define REG_STACK 30
#define REG_RTN_ADDR 31

#define TAMANHO_PARTICAO 32
#define ULTIMA_PARTICAO_MEM_DADOS 127

/* Tipo de código em compilação */
CodeType codeType;

/* Cabeça da lista de instruções objeto */
Objeto objHead = NULL;

/* Cabeça da lista de labels */
Label labelHead = NULL;

/* Escopo atual */
Escopo escopo = NULL;

/* Variável usada para configurar a identação do código impresso */
int indent = 0;

/* Variáveis auxiliares na geração de código objeto */
char temp[100];
int linha = 0;

 /* Operandos especiais */
TargetOperand rtnAddrOp;
TargetOperand rtnValOp;
TargetOperand stackOp;
TargetOperand stackBakOp;
TargetOperand pcBakOp;
TargetOperand kernelOp0;
TargetOperand kernelOp1;
TargetOperand rZeroOp;
TargetOperand globalBakOp;
TargetOperand globalOp;

Registrador registradores[QTD_REG];
RegisterName regNames[QTD_REG] = {
    $rz, $a0, $a1, $a2, $a3, $s0, $s1, $s2,
    $s3, $s4, $s5, $s6, $s7, $s8, $s9, $t0,
    $t1, $t2, $t3, $t4, $t5, $t6, $t7, $t8,
    $v0, $k0, $k1, $gpb, $spb, $gp, $sp, $ra
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

/* Retorna as posições iniciais da memória de dados de acordo com o programa escolhido
 * Obs: Posições iniciais de memória devem ser reservadas
 */
TargetOperand getStackZeroOperandLocation(int offset) {
    // Operando que representa o modo de endereçamento indexado
    TargetOperand operando = (TargetOperand) malloc(sizeof(struct targetOperand));
    operando->tipoEnderecamento = INDEXADO;
    operando->enderecamento.indexado.offset = offset;
    operando->enderecamento.indexado.registrador = regNames[REG_ZERO];
    return operando;
}

TargetOperand getContextLocation(int offset) {
    // Operando que representa o modo de endereçamento indexado
    TargetOperand operando = (TargetOperand) malloc(sizeof(struct targetOperand));
    operando->tipoEnderecamento = INDEXADO;
    operando->enderecamento.indexado.offset = offset;
    operando->enderecamento.indexado.registrador = regNames[REG_ARG_INICIO];
    return operando;
}

TargetOperand getLocationToSaveContexto(void) {
    // Operando que representa o modo de endereçamento indexado
    TargetOperand operando = (TargetOperand) malloc(sizeof(struct targetOperand));
    operando->tipoEnderecamento = INDEXADO;
    operando->enderecamento.indexado.offset = 0;
    operando->enderecamento.indexado.registrador = regNames[REG_ARG_INICIO + 1];
    return operando;
}

TargetOperand getLocationToReadContexto(void) {
    // Operando que representa o modo de endereçamento indexado
    TargetOperand operando = (TargetOperand) malloc(sizeof(struct targetOperand));
    operando->tipoEnderecamento = INDEXADO;
    operando->enderecamento.indexado.offset = 0;
    operando->enderecamento.indexado.registrador = regNames[REG_ARG_INICIO];
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
        reg.targetOp = operando;
        return reg.targetOp;
    }

    // FIXME
    printf("FIXME - MAIS DE 4 PARAMETROS NA FUNÇÃO\n");
    exit(1);

    operando->enderecamento.registrador = regNames[REG_ZERO];
    Registrador reg = registradores[REG_ZERO];
    reg.targetOp = operando;
    return reg.targetOp;
}

TargetOperand getTempReg(Operand op) {
    /* Se já tiver usado todos registradores temporários volta a usar do início
     * sem fazer nenhuma verificação adicional, pois os registradores temporários
     * não garantem persistência dos dados
     */
    if(escopo->tempRegCount > 8) {
        escopo->tempRegCount = 0;
    }

    // Calcula a posição do registrador no banco de registradores
    int position = SHIFT_COUNTER_REG_TEMP + escopo->tempRegCount++;
    Registrador reg = registradores[position];
    return getAndUpdateTargetOperand(reg, op);
}

TargetOperand getSavedReg(Operand op) {
    /* Se já tiver usado todos registradores temporários volta a usar do início,
     * mas antes de usar o registrador deve salvar seu valor antigo na memória
     */
    if(escopo->savedRegCount > 9) {
        escopo->savedRegCount = 0;
    }

    // Calcula a posição do registrador no banco de registradores
    int position = SHIFT_COUNTER_REG_SAVED + escopo->savedRegCount++;
    Registrador reg = registradores[position];
    return getAndUpdateTargetOperand(reg, op);
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
            rs->deslocamento = rt->enderecamento.indexado.offset;
            rs->regName = rt->enderecamento.indexado.registrador;
        }
    } else { /* Valor Imediato */
        // Prepara o operando
        Operand operand = createOperand();
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
    TargetOperand rs, rt;
    TreeNode * treeNode;

    rs = getTargetOpByName(op->contents.variable.name);
    if(rs == NULL) {
        rs = getSavedReg(op);
        if(op->contents.variable.scope == globalScope) {
            /* Lê o endereço de memória do início do vetor */
            rt = getGlobalOperandLocation(op);
            printCode(insertObjInst(createObjInst(_LA, TYPE_I, rs, rt, NULL)));
            registradores[(int) rs->enderecamento.registrador].isAddress = TRUE;
        } else {
            // Verifica se o vetor foi declarado no mesmo escopo atual ou veio como parâmetro
            treeNode = getVarFromSymtab(op->contents.variable.name, op->contents.variable.scope)->treeNode;
            rt = getStackOperandLocation(op);
            if(treeNode->kind.var.mem == PARAMK) {
                /* Parâmetro - Lê o ponteiro para o vetor */
                printCode(insertObjInst(createObjInst(_LW, TYPE_I, rs, rt, NULL)));
            } else {
                /* Escopo atual - Lê o endereço de memória do início do vetor */
                printCode(insertObjInst(createObjInst(_LA, TYPE_I, rs, rt, NULL)));
                registradores[(int) rs->enderecamento.registrador].isAddress = TRUE;
            }
        }
    }
    rs->deslocamento = rt->enderecamento.indexado.offset;
    rs->regName = rt->enderecamento.indexado.registrador;
    return rs;
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
        TargetOperand r = getOperandRegName(q->op1);
        printCode(insertObjInst(createObjInst(_SW, TYPE_I, reg, getMemLocation(r->enderecamento.registrador), NULL)));
    } else if(q->op1->contents.variable.scope == globalScope) {
        if(q->op3 != NULL) {
            // Vetor com índice de acesso igual a uma constante
            TargetOperand r = getOperandRegName(q->op1);
            printCode(insertObjInst(createObjInst(_SW, TYPE_I, reg, getMemIndexedLocation(r->enderecamento.registrador, q->op3->contents.val), NULL)));
        } else {
            // Variável comum
            TargetOperand op = getGlobalOperandLocation(q->op1);
            printCode(insertObjInst(createObjInst(_SW, TYPE_I, reg, op, NULL)));
            updateRegisterContent(op);
        }
    } else {
        if(q->op3 != NULL) {
            // Vetor com índice de acesso igual a uma constante
            TargetOperand r = getOperandRegName(q->op1);
            printCode(insertObjInst(createObjInst(_SW, TYPE_I, reg, getMemIndexedLocation(r->enderecamento.registrador, q->op3->contents.val), NULL)));
        } else {
            // Variável comum
            TargetOperand op = getStackOperandLocation(q->op1);
            printCode(insertObjInst(createObjInst(_SW, TYPE_I, reg, op, NULL)));
            updateRegisterContent(op);
        }
    }
}

/**
 * Salva todos os registradores para se preparar para uma possível troca de contexto.
 * Salva sempre na última partição da memória de dados.
 */
void saveRegistradores(void) {
    int i;
    int index = ULTIMA_PARTICAO_MEM_DADOS * TAMANHO_PARTICAO;

    for(i = 0; i < REG_KERNEL_0; i++) {
        Registrador reg = registradores[i];
        printCode(insertObjInst(createObjInst(_SW, TYPE_I, reg.targetOp, getStackZeroOperandLocation(index), NULL)));
        index++;
    }
    // Salva também o registrador de endereço de retorno
    printCode(insertObjInst(createObjInst(_SW, TYPE_I, registradores[REG_RTN_ADDR].targetOp, getStackZeroOperandLocation(index), NULL)));
}

/**
 * Lê todos os registradores para realizar troca de contexto.
 * Lê sempre na última partição da memória de dados.
 */
void loadRegistradores(void) {
    int i;
    int index = ULTIMA_PARTICAO_MEM_DADOS * TAMANHO_PARTICAO;

    for(i = 0; i < REG_KERNEL_0; i++) {
        Registrador reg = registradores[i];
        printCode(insertObjInst(createObjInst(_LW, TYPE_I, reg.targetOp, getStackZeroOperandLocation(index), NULL)));
        index++;
    }
    // Carrega também o registrador de endereço de retorno
    printCode(insertObjInst(createObjInst(_LW, TYPE_I, registradores[REG_RTN_ADDR].targetOp, getStackZeroOperandLocation(index), NULL)));
}

void geraCodigoChamadaFuncao(Quadruple q) {
    /* Verifica o nome da função/procedimento que está sendo chamada, se for input ou output imprime as
     * instruções específicas 'in' e 'out'. Depois verifica o escopo de onde vem a chamada, se for do
     * escopo da 'main' não guarda $ra na memória, caso contrário guarda $ra na memória.
     */
    if(!strcmp(q->op1->contents.variable.name, "input")) {
        if (codeType == PROGRAMA) {
            printCode(insertObjInst(createObjInst(_PRE_IO, TYPE_J, NULL, NULL, NULL)));
        }
        printCode(insertObjInst(createObjInst(_IN, TYPE_I, getTempReg(q->op3), NULL, NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "output")) {
        printCode(insertObjInst(createObjInst(_OUT, TYPE_I, getArgReg(0), NULL, getImediato(q->display))));
    } else if(!strcmp(q->op1->contents.variable.name, "ldk")) {
        printCode(insertObjInst(createObjInst(_LW_DISK, TYPE_I, getTempReg(q->op3), getArgReg(0), NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "sdk")) {
        printCode(insertObjInst(createObjInst(_SW_DISK, TYPE_I, getArgReg(0), getArgReg(1), NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "lam")) {
        printCode(insertObjInst(createObjInst(_LW_AM, TYPE_I, getTempReg(q->op3), NULL, NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "sam")) {
        printCode(insertObjInst(createObjInst(_SW_AM, TYPE_I, getArgReg(0), NULL, NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "sim")) {
        printCode(insertObjInst(createObjInst(_SW_IM, TYPE_I, getArgReg(0), getArgReg(1), NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "mmuLowerIM")) {
        printCode(insertObjInst(createObjInst(_MMU_LOWER_IM, TYPE_I, getArgReg(0), NULL, NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "mmuUpperIM")) {
        printCode(insertObjInst(createObjInst(_MMU_UPPER_IM, TYPE_I, getArgReg(0), NULL, NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "mmuSelect")) {
        // getArgReg(0) é o seletor da MMU que será alterado para o offset do programa que será executado
        printCode(insertObjInst(createObjInst(_MMU_SELECT, TYPE_I, NULL, getArgReg(0), NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "exec")) {
        // getArgReg(0) é o seletor da MMU que será alterado para o offset do programa que será executado
        printCode(insertObjInst(createObjInst(_MMU_SELECT, TYPE_I, NULL, getArgReg(0), NULL)));

        removeAllSavedOperands();
        // Executa um programa carregado em memória
        printCode(insertObjInst(createObjInst(_SW, TYPE_I, rtnAddrOp, getStackLocation(-escopo->tamanhoBlocoMemoria), NULL))); // sw $ra
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, getArgReg(2), stackOp, NULL)));         // Backup stack pointer
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, getArgReg(3), globalOp, NULL)));        // Backup global pointer
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, stackOp, stackBakOp, NULL)));
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, globalOp, globalBakOp, NULL)));
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, stackBakOp, getArgReg(2), NULL)));      // Backup stack pointer
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, globalBakOp, getArgReg(3), NULL)));     // Backup global pointer
        printCode(insertObjInst(createObjInst(_EXEC, TYPE_J, NULL, NULL, NULL)));
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, stackOp, stackBakOp, NULL)));           // Restaura stack pointer
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, globalOp, globalBakOp, NULL)));         // Restaura global pointer
        printCode(insertObjInst(createObjInst(_LW, TYPE_I, rtnAddrOp, getStackLocation(-escopo->tamanhoBlocoMemoria), NULL))); // lw $ra
    } else if(!strcmp(q->op1->contents.variable.name, "execAgain")) {
        // getArgReg(0) é o seletor da MMU que será alterado para o offset do programa que será executado
        printCode(insertObjInst(createObjInst(_MMU_SELECT, TYPE_I, NULL, getArgReg(0), NULL)));

        removeAllSavedOperands();
        // Executa um programa carregado em memória + offset
        printCode(insertObjInst(createObjInst(_SW, TYPE_I, rtnAddrOp, getStackLocation(-escopo->tamanhoBlocoMemoria), NULL))); // sw $ra
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, getArgReg(2), stackOp, NULL)));         // Backup stack pointer
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, getArgReg(3), globalOp, NULL)));        // Backup global pointer
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, stackOp, stackBakOp, NULL)));
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, globalOp, globalBakOp, NULL)));
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, stackBakOp, getArgReg(2), NULL)));      // Backup stack pointer
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, globalBakOp, getArgReg(3), NULL)));     // Backup global pointer
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, kernelOp1, getArgReg(1), NULL)));

        // Recupera registradores do novo contexto
        loadRegistradores();

        // kernelOp1 é o endereço arbitrário do qual se retomará a execução do programa
        printCode(insertObjInst(createObjInst(_EXEC_AGAIN, TYPE_I, NULL, kernelOp1, NULL)));
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, stackOp, stackBakOp, NULL)));           // Restaura stack pointer
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, globalOp, globalBakOp, NULL)));         // Restaura global pointer
        printCode(insertObjInst(createObjInst(_LW, TYPE_I, rtnAddrOp, getStackLocation(-escopo->tamanhoBlocoMemoria), NULL))); // lw $ra
    } else if(!strcmp(q->op1->contents.variable.name, "lcd")) {
        printCode(insertObjInst(createObjInst(_LCD, TYPE_I, getArgReg(0), NULL, NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "lcdPgms")) {
        printCode(insertObjInst(createObjInst(_LCD_PGMS, TYPE_I, getArgReg(0), NULL, NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "lcdCurr")) {
        printCode(insertObjInst(createObjInst(_LCD_CURR, TYPE_I, getArgReg(0), NULL, NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "gic")) {
        printCode(insertObjInst(createObjInst(_GIC, TYPE_I, getTempReg(q->op3), NULL, NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "cic")) {
        printCode(insertObjInst(createObjInst(_CIC, TYPE_I, NULL, NULL, NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "gip")) {
        printCode(insertObjInst(createObjInst(_GIP, TYPE_I, getTempReg(q->op3), NULL, NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "gsp")) { // getStackPointer
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, getTempReg(q->op3), stackOp, NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "gspb")) { // getStackPointerBackup
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, getTempReg(q->op3), stackBakOp, NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "ggpb")) { // getGlobalPointerBackup
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, getTempReg(q->op3), globalBakOp, NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "sspb")) { // setStackPointerBackup
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, stackBakOp, getArgReg(0), NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "sgpb")) { // setStackPointerBackup
        printCode(insertObjInst(createObjInst(_MOV, TYPE_I, globalBakOp, getArgReg(0), NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "rgnsp")) { // restoreGlobalN'StackPointers
        printCode(insertObjInst(createObjInst(_LI, TYPE_I, globalOp, getImediato(0), NULL)));
        printCode(insertObjInst(createObjInst(_LI, TYPE_I, stackOp, getImediato(escopo->tamanhoBlocoMemoria + getTamanhoBlocoMemoriaEscopoGlobal() + 1), NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "saveRegs")) {
        saveRegistradores();
    } else if(!strcmp(q->op1->contents.variable.name, "loadRegs")) {
        loadRegistradores();
    } else if(!strcmp(q->op1->contents.variable.name, "ldm")) {
        printCode(insertObjInst(createObjInst(_LW, TYPE_I, getTempReg(q->op3), getLocationToReadContexto(), NULL)));
    } else if(!strcmp(q->op1->contents.variable.name, "sdm")) {
        printCode(insertObjInst(createObjInst(_SW, TYPE_I, getArgReg(0), getLocationToSaveContexto(), NULL)));
    } else {
        int tamanhoBlocoMemoriaFuncaoChamada = getTamanhoBlocoMemoriaEscopo(q->op1->contents.variable.name);
        int tamanhoBlocoMemoriaEscopoAtual = escopo->tamanhoBlocoMemoria;
        int regRetornoFuncao = -(tamanhoBlocoMemoriaEscopoAtual); // Negativo pois é indexado ao ponteiro de pilha

        removeAllSavedOperands();
        if(codeType != KERNEL || strcmp(escopo->nome, "main")) {
            printCode(insertObjInst(createObjInst(_SW, TYPE_I, rtnAddrOp, getStackLocation(regRetornoFuncao), NULL))); // sw $ra
        }
        printCode(insertObjInst(createObjInst(_JAL, TYPE_J, getOperandLabel(q->op1->contents.variable.name), NULL, NULL)));

        /* Desaloca o bloco de memória na stack */
        popStackSpace(tamanhoBlocoMemoriaFuncaoChamada + 2); // +1 devido ao registrador $ra / +1 devido retorno de função ($v0)
        if(codeType != KERNEL || strcmp(escopo->nome, "main")) {
            printCode(insertObjInst(createObjInst(_LW, TYPE_I, rtnAddrOp, getStackLocation(regRetornoFuncao), NULL))); // lw $ra
        }
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
            TargetOperand arg = getArgReg(escopo->argRegCount);
            if(var != NULL && var->treeNode->node == VARK && var->treeNode->kind.var.varKind == VECTORK) { // Vetor
                printCode(insertObjInst(createObjInst(_LA, TYPE_I, arg, getStackOperandLocation(q->op1), NULL)));
                registradores[(int) arg->enderecamento.registrador].isAddress = TRUE;
            } else { // Variável
                reg = getOperandRegName(q->op1);
                if(getArgReg(escopo->argRegCount)->enderecamento.registrador != reg->enderecamento.registrador) { /* Só move se os registradores forem diferentes */
                    printCode(insertObjInst(createObjInst(_MOV, TYPE_I, arg, reg, NULL)));
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
                registradores[(int) rs->enderecamento.registrador].isAddress = TRUE;
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

        if (codeType == KERNEL) {
            // Backup do registrador ponteiro para a stack
            printCode(insertObjInst(createObjInst(_MOV, TYPE_I, stackBakOp, stackOp, NULL)));
            // Backup do registrador ponteiro para área de variáveis globais
            printCode(insertObjInst(createObjInst(_MOV, TYPE_I, globalBakOp, globalOp, NULL)));
            // Zera o registrador $rz
            printCode(insertObjInst(createObjInst(_LI, TYPE_I, rZeroOp, getImediato(0), NULL)));
            // Apontador para o registrador da stack ($sp) inicia em Zero
            printCode(insertObjInst(createObjInst(_LI, TYPE_I, stackOp, getImediato(0), NULL)));
            // Apontador para o registrador de escopo global ($gp) inicia em Zero
            printCode(insertObjInst(createObjInst(_LI, TYPE_I, globalOp, getImediato(0), NULL)));
        } else if (tamanho > 0) { // Se tamanho do escopo global for maior que Zero, quer dizer que existem variávies globais declaradas
            // Apontador para o registrador de escopo global ($gp)
            printCode(insertObjInst(createObjInst(_ADDI, TYPE_I, stackOp, globalOp, getImediato(1))));
        }

        // Aloca o bloco de memória na stack
        pushStackSpace(escopo->tamanhoBlocoMemoria + tamanho + 1); // +1 devido ao registrador $ra
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

void geraCodigoObjeto(Quadruple q, CodeInfo codeInfo) {
    INDENT;
    emitCode("\n********** Código objeto **********");
    // Atribui o tipo do código em compilação
    codeType = codeInfo.codeType;
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
                removeAllSavedOperands();
                break;

            /* Halt */
            case HALT:
                printCode(insertObjInst(createObjInst(_HALT, TYPE_J, NULL, NULL, NULL)));
                break;
            case SYSCALL:
                /* Desaloca o bloco de memória da função main() da stack, isso é necessário para o SO ter o stackPointer alinhado com sua área de memória */
                popStackSpace(escopo->tamanhoBlocoMemoria + getTamanhoBlocoMemoriaEscopoGlobal() + 1); // +1 devido ao registrador $ra
                // Retorna o controle para o sistema operacional
                printCode(insertObjInst(createObjInst(_SYSCALL, TYPE_I, NULL, kernelOp0, NULL)));
                break;
        }
        q = q->next;
    }

    // Libera memória do código intermediário
    freeCodigoIntermediario();
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

    int i;
    for(i = 0; i < 10; i++) {
        e->registradoresEmUso[i] = FALSE;
    }

    e->next = NULL;
    return e;
}

void pushEscopo(Escopo e) {
    if(escopo == NULL) {
        escopo = e;
    } else {
        e->next = escopo;
        escopo = e;
        removeAllSavedOperands();
    }
}

TargetOperand getTargetOpByName(char * name) {
    int i;

    for(i = 0; i < QTD_REG; i++) {
        Registrador reg = registradores[i];
        char * varName = reg.op->contents.variable.name;
        if(varName != NULL && !strcmp(name, varName)) {
            return reg.targetOp;
        }
    }
    return NULL;
}

TargetOperand getAndUpdateTargetOperand(Registrador reg, Operand op) {
    reg.isAddress = FALSE;
    reg.op->kind = op->kind;

    if(op->kind == String) {
        reg.op->contents.variable.name = op->contents.variable.name;
        reg.op->contents.variable.scope = op->contents.variable.scope;
    } else {
        reg.op->contents.val = op->contents.val;
    }

    reg.op = op;
    return reg.targetOp;
}

void updateRegisterContent(TargetOperand operand) {
    int i;
    for(i = REG_SAVED_INICIO; i <= REG_SAVED_FIM; i++) {
        Registrador reg = registradores[i];
        if (reg.op->contents.variable.name != NULL &&
            reg.op->contents.variable.scope != NULL &&
            reg.targetOp->deslocamento == operand->enderecamento.indexado.offset &&
            reg.targetOp->regName == operand->enderecamento.indexado.registrador) {
            printCode(insertObjInst(createObjInst(_LW, TYPE_I, reg.targetOp, operand, NULL)));
        }
    }
}

void removeOperand(TargetOperand opTarget) {
    Registrador reg = registradores[(int) opTarget->enderecamento.registrador]; // Cast enum to int
    reg.op->contents.variable.name = NULL;
    reg.op->contents.variable.scope = NULL;
    reg.isAddress = FALSE;
}

void removeAllSavedOperands(void) {
    int i;
    for(i = REG_SAVED_INICIO; i <= REG_SAVED_FIM; i++) {
        Registrador reg = registradores[i];
        reg.op->contents.variable.name = NULL;
        reg.op->contents.variable.scope = NULL;
        reg.isAddress = FALSE;
    }
    escopo->savedRegCount = 0;
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
    for(i = 0; i < QTD_REG; i++) {
        Registrador reg;
        reg.op = createOperand();
        reg.op->contents.variable.name = NULL;
        reg.op->contents.variable.scope = NULL;
        reg.targetOp = (TargetOperand) malloc(sizeof(struct targetOperand));
        reg.targetOp->tipoEnderecamento = REGISTRADOR;
        reg.targetOp->enderecamento.registrador = regNames[i];
        reg.regName = regNames[i];
        reg.isAddress = FALSE;
        registradores[i] = reg;
    }
}

void prepararOperandosEspeciais(void) {
    rtnAddrOp = registradores[REG_RTN_ADDR].targetOp;
    rtnValOp = registradores[REG_RTN_VAL].targetOp;
    stackOp = registradores[REG_STACK].targetOp;
    stackBakOp = registradores[REG_STACK_BAK].targetOp;
    kernelOp0 = registradores[REG_KERNEL_0].targetOp;
    kernelOp1 = registradores[REG_KERNEL_1].targetOp;
    rZeroOp = registradores[REG_ZERO].targetOp;
    globalOp = registradores[REG_GLOBAL].targetOp;
    globalBakOp = registradores[REG_GLOBAL_BAK].targetOp;
}

Objeto getCodigoObjeto(void) {
    return objHead;
}
