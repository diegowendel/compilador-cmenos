/****************************************************/
/* File: cgen.c                                     */
/* The code generator implementation                */
/* for the TINY compiler                            */
/* (generates code for the TM machine)              */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"

/* number of arguments
 * of current function call to be inserted
 * in the intermediate code instruction list
 */
ParamStack paramHead = NULL;

/* Cabeça da Lista de representações quádruplas */
Quadruple head = NULL;

/* Cabeça da Lista de representações quádruplas */
LocationStack locationHead = NULL;

/* Numero para geração de nomes de variáveis temporárias */
static int temporario = 1;

/* Numero para geração de labels */
static int label = 1;

Operand operandoAtual;

InstructionKind instrucaoAtual;

/* Útil para saber quando deve-se retornar o endereço do vetor */
Quadruple ultimaQuadrupla;

static int linha = 0;

/* temporary string to help printing text */
static char tempString[20];

/* Protótipo para o gerador de código interno recursivo */
static void cGen (TreeNode * tree);

static char * createLabelName() {
    char * temp = (char *) malloc(5);
    sprintf(temp, "L%d", label);
    ++label;
    return temp;
}

static char * createTemporaryOperandName() {
    char * temp = (char *) malloc(5);
    sprintf(temp, "t%d", temporario);
    ++temporario;
    return temp;
}

Operand createOperand(void) {
    Operand op = (Operand) malloc(sizeof(struct operand));
    return op;
}

static Operand createTemporaryOperand() {
    Operand temp = createOperand();
    temp->kind = String;
    temp->contents.variable.scope = NULL;
    temp->contents.variable.name = createTemporaryOperandName();
    return temp;
}

/* Procedure genStmt generates code at a statement node */
static void genStmt(TreeNode * tree) {
    Quadruple * q;
    Operand op1, op2, op3;
    TreeNode * p1, * p2, * p3;
    switch (tree->kind.stmt) {
        case INTEGERK:
            p1 = tree->child[0];
            cGen(p1);
            break; /* INTEGERK */

        case VOIDK:
            p1 = tree->child[0];
            cGen(p1);
            break; /* VOIDK */

        case IFK:
            p1 = tree->child[0];
            p2 = tree->child[1];
            p3 = tree->child[2];
            /* Gera código para a expressão de teste */
            cGen(p1);
            /* Atribui como o primeiro operando */
            op1 = operandoAtual;
            /* Atribui o tipo de instrução */
            instrucaoAtual = JPF;
            /* Cria e insere uma nova representação em código intermediário */
            q = insertQuad(createQuad(instrucaoAtual, op1, NULL, NULL));
            /* Salva a IR do if para atualizar com o label que representa o fim do bloco then */
            pushLocation(q);
            /* Gera código para o bloco then */
            cGen(p2);
            /* set second operand */
            op2 = createOperand();
            op2->kind = String;
            op2->contents.variable.name = createLabelName();
            op2->contents.variable.scope = tree->kind.var.scope;
            /* update if intermediate code instruction */
            updateLocation(op2);
            popLocation();

            if(p3 != NULL) {
                q = insertQuad(createQuad(GOTO, NULL, NULL, NULL));
                pushLocation(q);
            }

            /* Label usado para marcar o fim do bloco then */
            insertQuad(createQuad(LBL, op2, NULL, NULL));
            cGen(p3);

            if(p3 != NULL) {
                op1 = createOperand();
                op1->kind = String;
                op1->contents.variable.name = createLabelName();
                op1->contents.variable.scope = tree->kind.var.scope;
                /* update if intermediate code instruction */
                updateLocation(op1);
                popLocation();

                /* Label usado para marcar o fim do bloco else */
                insertQuad(createQuad(LBL, op1, NULL, NULL));
            }
            break; /* IFK */

        case WHILEK:
            p1 = tree->child[0];
            p2 = tree->child[1];

            op1 = createOperand();
            op1->kind = String;
            op1->contents.variable.name = createLabelName();
            op1->contents.variable.scope = tree->kind.var.scope;
            insertQuad(createQuad(LBL, op1, NULL, NULL));
            /* build code for test expression */
            cGen(p1);
            /* set as first operand */
            op2 = operandoAtual;
            /* set intermediate code kind */
            instrucaoAtual = JPF;
            /* Cria e insere uma nova representação em código intermediário */
            q = insertQuad(createQuad(instrucaoAtual, op2, NULL, NULL));
            /* Salva a IR do if para atualizar com o label que representa o fim do bloco then */
            pushLocation(q);
            /* build code for while block */
            cGen(p2);
            /* go back to while test expression */
            /* set intermediate code kind */
            instrucaoAtual = GOTO;
            /* Cria e insere uma nova representação em código intermediário */
            insertQuad(createQuad(instrucaoAtual, op1, NULL, NULL));

            op3 = createOperand();
            op3->kind = String;
            op3->contents.variable.name = createLabelName();
            op3->contents.variable.scope = tree->kind.var.scope;
            insertQuad(createQuad(LBL, op3, NULL, NULL));
            updateLocation(op3);
            popLocation();
            break; /* WHILEK */

        case RETURNK:
            p1 = tree->child[0];
            /* Gera código para a expressão */
            cGen(p1);
            if(p1) {
                /* Atribui como o primeiro operando */
                op1 = operandoAtual;
            } else {
                /* Não retorna nada */
                op1 = NULL;
            }
            /* Atribui o tipo de instrução */
            instrucaoAtual = RTN;
            /* Cria e insere uma nova representação em código intermediário */
            insertQuad(createQuad(instrucaoAtual, op1, NULL, NULL));
            break; /* RETURNK */

        case COMPK:
            p1 = tree->child[0];
            if(p1) { /* Parâmetros */
                cGen(p1);
            }
            p2 = tree->child[1];
            if(p2) { /* Expressões */
                cGen(p2);
            }
            break; /* COMPK */
    }
} /* genStmt */

/* Procedure genExp generates code at an expression node */
static void genExp(TreeNode * tree) {
    TreeNode * p1, * p2;
    Operand op1, op2, op3;
    switch (tree->kind.exp) {
        case ATRIBK:
            p1 = tree->child[0];
            p2 = tree->child[1];
            /* Gera código para o operando da direita */
            cGen(p2);
            /* Atribui como o segundo operando */
            op2 = operandoAtual;
            /* Gera código para o operando da esquerda */
            cGen(p1);
            /* Atribui como o primeiro operando */
            op1 = operandoAtual;
            op3 = NULL;

            // ------------------- Caso especial ------------------- //
            /* Na atribuição, se o operando da esquerda for um vetor, deve-se
             * fazer um store na posição de memória dessa variável
             */
            if(p1->node == VARK && p1->kind.var.varKind == VECTORK) {
                ultimaQuadrupla->instruction = VEC_ADDR;
                // Se o índice for uma constante, adiciona o offset como o 3º operando (para código objeto)
                if(ultimaQuadrupla->op2->kind == IntConst) {
                    op3 = createOperand();
                    op3->kind = IntConst;
                    op3->contents.val = ultimaQuadrupla->op2->contents.val;
                    op1 = ultimaQuadrupla->op1;
                }
            }

            if(tree->op != ATRIBUICAO) {
                switch (tree->op) {
                    case ATRIB_MAIS:
                        instrucaoAtual = ADD;
                        break;
                    case ATRIB_MENOS:
                        instrucaoAtual = SUB;
                        break;
                    case ATRIB_VEZES:
                        instrucaoAtual = MULT;
                        break;
                    case ATRIB_DIVISAO:
                        instrucaoAtual = DIV;
                        break;
                    case ATRIB_MODULO:
                        instrucaoAtual = MOD;
                        break;
                    case ATRIB_AND:
                        instrucaoAtual = BITW_AND;
                        break;
                    case ATRIB_OR:
                        instrucaoAtual = BITW_OR;
                        break;
                    case ATRIB_XOR:
                        instrucaoAtual = BITW_XOR;
                        break;
                    case ATRIB_SHIFT_LEFT:
                        instrucaoAtual = SHFT_LF;
                        break;
                    case ATRIB_SHIFT_RIGHT:
                        instrucaoAtual = SHFT_RT;
                        break;
                }
                operandoAtual = createTemporaryOperand();
                insertQuad(createQuad(instrucaoAtual, op1, op2, operandoAtual));
                op2 = operandoAtual;
            }
            instrucaoAtual = ASN;
            insertQuad(createQuad(instrucaoAtual, op1, op2, op3));
            break; /* ATRIBK */

        case RELK:
            p1 = tree->child[0];
            p2 = tree->child[1];
            /* Gera código para o operando da esquerda */
            cGen(p1);
            /* Atribui como o primeiro operando */
            op1 = operandoAtual;
            /* Gera código para o operando da direita */
            cGen(p2);
            /* Atribui como o segundo operando */
            op2 = operandoAtual;
            switch (tree->op) {
                case IGUAL:
                    instrucaoAtual = EQ;
                    break;
                case DIFERENTE:
                    instrucaoAtual = NE;
                    break;
                case MENOR:
                    instrucaoAtual = LT;
                    break;
                case MENORIGUAL:
                    instrucaoAtual = LET;
                    break;
                case MAIOR:
                    instrucaoAtual = GT;
                    break;
                case MAIORIGUAL:
                    instrucaoAtual = GET;
                    break;
            }
            operandoAtual = createTemporaryOperand();
            insertQuad(createQuad(instrucaoAtual, op1, op2, operandoAtual));
            break; /* RELK */

        case ARITHK:
            p1 = tree->child[0];
            p2 = tree->child[1];
            /* Gera código para o operando da esquerda */
            cGen(p1);
            /* Atribui como o primeiro operando */
            op1 = operandoAtual;
            /* Gera código para o operando da direita */
            cGen(p2);
            /* Atribui como o segundo operando */
            op2 = operandoAtual;
            switch (tree->op) {
                case MAIS:
                    instrucaoAtual = ADD;
                    break;
                case MENOS:
                    instrucaoAtual = SUB;
                    break;
                case VEZES:
                    instrucaoAtual = MULT;
                    break;
                case DIVISAO:
                    instrucaoAtual = DIV;
                    break;
                case MODULO:
                    instrucaoAtual = MOD;
                    break;
                case SHIFT_LEFT:
                    instrucaoAtual = SHFT_LF;
                    break;
                case SHIFT_RIGHT:
                    instrucaoAtual = SHFT_RT;
                    break;
            }
            operandoAtual = createTemporaryOperand();
            insertQuad(createQuad(instrucaoAtual, op1, op2, operandoAtual));
            break; /* ARITHK */

        case LOGICK:
            p1 = tree->child[0];
            p2 = tree->child[1];
            /* Gera código para o operando da esquerda */
            cGen(p1);
            /* Atribui como o primeiro operando */
            op1 = operandoAtual;
            /* Gera código para o operando da direita */
            cGen(p2);
            /* Atribui como o segundo operando */
            op2 = operandoAtual;
            switch (tree->op) {
                case LOGICAL_AND:
                    instrucaoAtual = LOGIC_AND;
                    break;
                case LOGICAL_OR:
                    instrucaoAtual = LOGIC_OR;
                    break;
                case AND:
                    instrucaoAtual = BITW_AND;
                    break;
                case OR:
                    instrucaoAtual = BITW_OR;
                    break;
                case XOR:
                    instrucaoAtual = BITW_XOR;
                    break;
            }
            operandoAtual = createTemporaryOperand();
            insertQuad(createQuad(instrucaoAtual, op1, op2, operandoAtual));
            break; /* LOGICK */

        case UNARYK:
            p1 = tree->child[0];
            cGen(p1);
            op1 = operandoAtual;
            switch (tree->op) {
                case AND:
                    instrucaoAtual = BITW_AND;
                    break;
                case NOT:
                    instrucaoAtual = BITW_NOT;
                    break;
                case MENOS:
                    instrucaoAtual = SUB;
                    break;
            }
            operandoAtual = createTemporaryOperand();
            insertQuad(createQuad(instrucaoAtual, op1, operandoAtual, NULL));
            break; /* UNARYK */
    }
} /* genExp */

static void genVar(TreeNode * tree) {
    Quadruple q;
    TreeNode * p1, * p2;
    Operand op1, op2, op3;
    int qtdParams, offset, display = -1;
    switch (tree->kind.var.varKind) {
        case CONSTK:
            /* Atribui o operando atual */
            operandoAtual = createOperand();
            operandoAtual->kind = IntConst;
            operandoAtual->contents.val = tree->kind.var.attr.val;
            break; /* CONSTK */

        case IDK:
            /* Atribui o operando atual */
            operandoAtual = createOperand();
            operandoAtual->kind = String;
            operandoAtual->contents.variable.name = tree->kind.var.attr.name;
            operandoAtual->contents.variable.scope = tree->kind.var.scope;
            break; /* IDK */

        case VECTORK:
            p1 = tree->child[0];
            /* Atualiza o operando atual como o id do vetor e seta como op1 */
            operandoAtual = createOperand();
            operandoAtual->kind = String;
            operandoAtual->contents.variable.name = tree->kind.var.attr.name;
            operandoAtual->contents.variable.scope = tree->kind.var.scope;
            op1 = operandoAtual;
            /* Gera código para a posição do vetor */
            cGen(p1);
            /* Indice do vetor */
            op2 = operandoAtual;
            /* Atribui a instrução atual */
            instrucaoAtual = VEC;
            /* Temporário */
            op3 = createTemporaryOperand();
            /* Atualiza o operando atual */
            operandoAtual = op3;
            /* Cria e insere uma nova representação em código intermediário, mas
             * somente se estiver acessando o vetor
             */
            if(tree->kind.var.acesso == ACCESSK) {
                // Guarda a última instrução para manipular em caso de acesso ao endereço do vetor
                ultimaQuadrupla = createQuad(instrucaoAtual, op1, op2, op3);
                insertQuad(ultimaQuadrupla);
            }
            break; /* VectorK */

        case FUNCTIONK:
            verificaFimInstrucaoAnterior();
            op1 = createOperand();
            op1->kind = String;
            op1->contents.variable.name = tree->kind.var.attr.name;
            op1->contents.variable.scope = tree->kind.var.scope;
            insertQuad(createQuad(FUNC, op1, NULL, NULL));

            /* list of parameters */
            p1 = tree->child[0];
            while(p1 != NULL) {
                op2 = createOperand();
                op2->kind = String;
                op2->contents.variable.name = p1->child[0]->kind.var.attr.name;
                op2->contents.variable.scope = p1->child[0]->kind.var.scope;
                insertQuad(createQuad(GET_PARAM, op2, NULL, NULL));
                p1 = p1->sibling;
            }

            /* build code for function block */
            p2 = tree->child[1];
            cGen(p2);
            break;

        case CALLK:
            /* Argumentos */
            p1 = tree->child[0];
            /* Atribui o primeiro operando */
            op1 = createOperand();
            op1->kind = String;
            op1->contents.variable.name = tree->kind.var.attr.name;
            op1->contents.variable.scope = tree->kind.var.scope;
            /* Atribui o segundo operando */
            qtdParams = getQuantidadeParametros(tree);
            pushParam(&qtdParams);
            op2 = createOperand();
            if(qtdParams > 0) {
                op2->kind = IntConst;
                op2->contents.val = qtdParams;
            } else {
                op2->kind = IntConst;
                op2->contents.val = 0;
            }
            /* build code for function call */
            instrucaoAtual = PARAM_LIST;
            op3 = createOperand();
            op3->kind = IntConst;
            op3->contents.val = getQuantidadeParametros(tree);
            insertQuad(createQuad(instrucaoAtual, op3, NULL, NULL));
            while(p1 != NULL) {
                cGen(p1);
                /* Atribui o tipo de instrução */
                instrucaoAtual = SET_PARAM;
                /* Cria e insere uma nova representação em código intermediário */
                insertQuad(createQuad(instrucaoAtual, operandoAtual, NULL, NULL));
                /* Decrementa qtdParams */
                --qtdParams;
                /* Se for um chamado de OUTPUT, verifica o display de exibição */
                if(!strcmp(tree->kind.var.attr.name, "output") && p1->sibling == NULL) {
                    display = p1->kind.var.attr.val;
                }
                p1 = p1->sibling;
            }
            popParam();
            /* Atribui o tipo de instrução */
            instrucaoAtual = CALL;
            /* Armazena possível offset para manipulação da memória reservada de índice de programas */
            if (op1->kind == IntConst) {
                offset = operandoAtual->contents.val;
            }
            /* Atualiza o operando atual */
            operandoAtual = createTemporaryOperand();
            /* Cria e insere uma nova representação em código intermediário */
            q = createQuad(instrucaoAtual, op1, op2, operandoAtual);
            if(display != -1) {
                q->display = display;
            }
            insertQuad(q);
            break;
    }
}

/* Procedimento cGen gera o código recursivamente
 * pela árvore sintática
 */
static void cGen(TreeNode * tree) {
    if (tree != NULL) {
        switch (tree->node) {
            case STMTK:
                genStmt(tree);
                break;
            case EXPK:
                genExp(tree);
                break;
            case VARK:
                genVar(tree);
                break;
            default:
                break;
        }
        /* Se a quantidade de parâmetros for maior que 0, cGen() será chamado automaticamente */
        if(paramHead == NULL) {
            cGen(tree->sibling);
        } else {
            if(paramHead->count == 0) {
                cGen(tree->sibling);
            }
        }
    }
}

void verificaFimInstrucaoAnterior(void) {
    if(head != NULL) {
        Quadruple temp = head;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        /* Insere um return forçadamente caso não haja no código de alto nível */
        if(temp->instruction != RTN) {
            insertQuad(createQuad(RTN, NULL, NULL, NULL));
        }
    }
}

/**********************************************/
/* the primary function of the code generator */
/**********************************************/
/* Procedure codeGen generates code to a code
 * file by traversal of the syntax tree. The
 * second parameter (codefile) is the file name
 * of the code file, and is used to print the
 * file name as a comment in the code file
 */
void codeGen(TreeNode * syntaxTree, char * codefile, CodeInfo codeInfo) {
    cGen(syntaxTree);

    if (codeInfo.codeType != PROGRAMA) {
        // Se for código do Kernel ou Bios, adiciona o HALT no fim do código
        insertQuad(createQuad(HALT, NULL, NULL, NULL));
    } else {
        // Se for código de um Programa comum, adiciona o SYSCALL no fim do código
        insertQuad(createQuad(SYSCALL, NULL, NULL, NULL));
    }

    emitCode("********** Código intermediário **********\n");
    printIntermediateCode();
}

void printIntermediateCode() {
    Quadruple q = head;
    char quad[100];

    while(q != NULL) {
        sprintf(quad, "%d: (", q->linha);
        strcat(quad, toStringInstruction(q->instruction));

        if(q->op1 == NULL) {
            strcat(quad, ", _");
        } else if(q->op1->kind == String) {
            strcat(quad, ", ");
            strcat(quad, q->op1->contents.variable.name);
        } else {
            sprintf(tempString, ", %d", q->op1->contents.val);
            strcat(quad,tempString);
        }

        if(q->op2 == NULL) {
            strcat(quad, ", _");
        } else if(q->op2->kind == String) {
            strcat(quad, ", ");
            strcat(quad, q->op2->contents.variable.name);
        } else {
            sprintf(tempString, ", %d", q->op2->contents.val);
            strcat(quad,tempString);
        }

        if(q->op3 == NULL) {
            strcat(quad, ", _)");
        } else if(q->op3->kind == String) {
            strcat(quad, ", ");
            strcat(quad, q->op3->contents.variable.name);
            strcat(quad, ")");
        } else {
            sprintf(tempString, ", %d", q->op3->contents.val);
            strcat(quad,tempString);
            strcat(quad, ")");
        }
        emitCode(quad);
        q = q->next;
    }
}

void pushLocation(Quadruple * quad) {
    // createLocation
    LocationStack ls = (LocationStack) malloc(sizeof(struct Location));
    ls->quad = quad;
    ls->next = NULL;

    if(locationHead == NULL) {
        locationHead = ls;
        locationHead->next = NULL;
    } else {
        ls->next = locationHead;
        locationHead = ls;
    }
}

void popLocation() {
    if(locationHead != NULL) {
        LocationStack ls = locationHead;
        locationHead = locationHead->next;
        free(ls);
        ls = NULL;
    }
}

void updateLocation(Operand op) {
    Quadruple q = *locationHead->quad;
    if(q->instruction != JPF) {
        q->op1 = op;
    } else {
        q->op2 = op;
    }
    *locationHead->quad = q;
}

void pushParam(int * count) {
    ParamStack ps = (ParamStack) malloc(sizeof(struct Param));
    ps->count = count;
    ps->next = NULL;
    if(paramHead == NULL) {
        paramHead = ps;
    } else {
        ps->next = paramHead;
        paramHead = ps;
    }
}

void popParam(void) {
    if(paramHead != NULL) {
        ParamStack ps = paramHead;
        paramHead = paramHead->next;
        free(ps);
        ps = NULL;
    }
}

Quadruple createQuad(InstructionKind instruction, Operand op1, Operand op2, Operand op3) {
    Quadruple q = (Quadruple) malloc(sizeof(struct Quad));
    q->instruction = instruction;
    q->op1 = op1;
    q->op2 = op2;
    q->op3 = op3;
    q->linha = ++linha;
    q->next = NULL;
    return q;
}

Quadruple * insertQuad(Quadruple q) {
    Quadruple * ptr = (Quadruple *) malloc(sizeof(struct Quad));
    if(head == NULL) {
        head = q;
        head->next = NULL;
        ptr = &head;
    } else {
        Quadruple temp = head;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = q;
        temp->next->next = NULL;
        ptr = &temp->next;
    }
    return ptr;
}

Quadruple getCodigoIntermediario(void) {
    return head;
}

void freeCodigoIntermediario(void) {
    if (head == NULL) {
        return;
    }

    Quadruple temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
