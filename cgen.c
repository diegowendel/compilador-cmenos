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

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indent = 0;

/* macros to increase/decrease indentation */
#define INDENT indent+=4
#define UNINDENT indent-=4

/* Numero para geração de nomes de variáveis temporárias */
static int temporario = 1;

/* Numero para geração de labels */
static int label = 1;

Operand operandoAtual;

/* Operando para representar vazio */
Operand vazio;

InstructionKind instrucaoAtual;

/* Útil para saber quando deve-se retornar o endereço do vetor */
Quadruple ultimaQuadrupla;

static int linha = 0;

/* temporary string to help printing text */
static char tempString[20];

/* Protótipo para o gerador de código interno recursivo */
static void cGen (TreeNode * tree);

static char * createLabel() {
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

static Operand createTemporaryOperand() {
    Operand temp;
    char * tempName =  createTemporaryOperandName();
    temp.kind = String;
    temp.contents.variable.scope = NULL;
    temp.contents.variable.name = tempName;
    return temp;
}

/* Procedure genStmt generates code at a statement node */
static void genStmt(TreeNode * tree) {
    INDENT;
    TreeNode * p1, * p2, * p3;
    Operand op1, op2, op3;
    Quadruple * q;
    switch (tree->kind.stmt) {

        case VoidK:
            emitComment("-> type void", indent);
            p1 = tree->child[0];
            cGen(p1);
            emitComment("<- type void\n", indent);
            break;

        case IntegerK:
            emitComment("-> type int", indent);
            p1 = tree->child[0];
            cGen(p1);
            emitComment("<- type int\n", indent);
            break;

        case CompK:
            p1 = tree->child[0];
            if(p1) { /* Parâmetros */
                cGen(p1);
            }
            p2 = tree->child[1];
            if(p2) { /* Expressões */
                cGen(p2);
            }
            break;

        case IfK:
            emitComment("-> if", indent);
            p1 = tree->child[0];
            p2 = tree->child[1];
            p3 = tree->child[2];

            emitComment("-> if: test expression", indent);
            /* Gera código para a expressão de teste */
            cGen(p1);
            /* Atribui como o primeiro operando */
            op1 = operandoAtual;
            /* Atribui o tipo de instrução */
            instrucaoAtual = JPF;
            /* Cria e insere uma nova representação em código intermediário */
            q = insertQuad(createQuad(instrucaoAtual, op1, vazio, vazio));
            /* Salva a IR do if para atualizar com o label que representa o fim do bloco then */
            pushLocation(createLocation(q));
            emitComment("<- if: test expression", indent);

            emitComment("-> if: if block", indent);
            /* Gera código para o bloco then */
            cGen(p2);
            emitComment("<- if: if block", indent);

            /* set second operand */
            op2.kind = String;
            op2.contents.variable.name = createLabel();
            op2.contents.variable.scope = tree->scope;
            /* update if intermediate code instruction */
            updateLocation(op2);
            popLocation();

            if(p3 != NULL) {
                q = insertQuad(createQuad(GOTO, vazio, vazio, vazio));
                pushLocation(createLocation(q));
            }

            /* Label usado para marcar o fim do bloco then */
            insertQuad(createQuad(LBL, op2, vazio, vazio));
            emitComment("-> if: else/end block", indent);

            cGen(p3);

            if(p3 != NULL) {
                op1.kind = String;
                op1.contents.variable.name = createLabel();
                op1.contents.variable.scope = tree->scope;
                /* update if intermediate code instruction */
                updateLocation(op1);
                popLocation();

                /* Label usado para marcar o fim do bloco else */
                insertQuad(createQuad(LBL, op1, vazio, vazio));
            }
            emitComment("<- if: else/end block", indent);
            emitComment("<- if", indent);
            break;

        case ElseK:
            break;

        case WhileK:
            emitComment("-> while", indent);
            p1 = tree->child[0];
            p2 = tree->child[1];

            emitComment("-> while: test expression", indent);

            op1.kind = String;
            op1.contents.variable.name = createLabel();
            op1.contents.variable.scope = tree->scope;
            insertQuad(createQuad(LBL, op1, vazio, vazio));
            /* build code for test expression */
            cGen(p1);
            /* set as first operand */
            op2 = operandoAtual;
            /* set intermediate code kind */
            instrucaoAtual = JPF;
            /* Cria e insere uma nova representação em código intermediário */
            q = insertQuad(createQuad(instrucaoAtual, op2, vazio, vazio));
            /* Salva a IR do if para atualizar com o label que representa o fim do bloco then */
            pushLocation(createLocation(q));

            emitComment("<- while: test expression", indent);

            emitComment("-> while: while block", indent);
            /* build code for while block */
            cGen(p2);
            /* go back to while test expression */
            /* set intermediate code kind */
            instrucaoAtual = GOTO;
            /* Cria e insere uma nova representação em código intermediário */
            insertQuad(createQuad(instrucaoAtual, op1, vazio, vazio));
            emitComment("<- while: while block", indent);

            op3.kind = String;
            op3.contents.variable.name = createLabel();
            op3.contents.variable.scope = tree->scope;
            insertQuad(createQuad(LBL, op3, vazio, vazio));
            updateLocation(op3);
            popLocation();
            emitComment("<- while", indent);
            break;

        case ReturnK:
            emitComment("-> return", indent);
            p1 = tree->child[0];
            emitComment("-> return: expression", indent);
            /* Gera código para a expressão */
            cGen(p1);
            if(p1) {
                /* Atribui como o primeiro operando */
                op1 = operandoAtual;
            } else {
                /* Não retorna nada */
                op1 = vazio;
            }
            /* Atribui o tipo de instrução */
            instrucaoAtual = RTN;
            /* Cria e insere uma nova representação em código intermediário */
            insertQuad(createQuad(instrucaoAtual, op1, vazio, vazio));
            emitComment("<- return: expression", indent);
            emitComment("<- return", indent);
            break;
    }
    UNINDENT;
} /* genStmt */

/* Procedure genExp generates code at an expression node */
static void genExp(TreeNode * tree) {
    INDENT;
    Quadruple q;
    TreeNode * p1, * p2;
    Operand op1, op2, op3;
    int qtdParams, display = -1;

    switch (tree->kind.exp) {
        case ConstK:
            emitComment("-> constant", indent);
            sprintf(tempString, "%d", tree->attr.val);
            emitComment(tempString, indent);
            /* Atribui o operando atual */
            operandoAtual.kind = IntConst;
            operandoAtual.contents.val = tree->attr.val;
            emitComment("<- constant", indent);
            break; /* ConstK */

        case IdK:
            emitComment("-> identifier", indent);
            emitComment(tree->attr.name, indent);
            /* Atribui o operando atual */
            operandoAtual.kind = String;
            operandoAtual.contents.variable.name = tree->attr.name;
            operandoAtual.contents.variable.scope = tree->scope;
            emitComment("<- identifier", indent);
            break; /* IdK */

        case VectorK:
            emitComment("-> vector", indent);
            emitComment(tree->attr.name, indent);
            p1 = tree->child[0];
            /* Atualiza o operando atual como o id do vetor e seta como op1 */
            operandoAtual.kind = String;
            operandoAtual.contents.variable.name = tree->attr.name;
            operandoAtual.contents.variable.scope = tree->scope;
            op1 = operandoAtual;
            /* Gera código para a posição do vetor */
            emitComment("-> vector: position", indent);
            cGen(p1);
            /* Indice do vetor */
            op2 = operandoAtual;
            emitComment("<- vector: position", indent);
            /* Atribui a instrução atual */
            instrucaoAtual = VEC;
            /* Temporário */
            op3 = createTemporaryOperand();
            /* Atualiza o operando atual */
            operandoAtual = op3;
            /* Cria e insere uma nova representação em código intermediário, mas
             * somente se estiver acessando o vetor
             */
            if(tree->varAccess == ACESSANDO) {
                // Guarda a última instrução para manipular em caso de acesso ao endereço do vetor
                ultimaQuadrupla = createQuad(instrucaoAtual, op1, op2, op3);
                insertQuad(ultimaQuadrupla);
            }
            emitComment("<- vector", indent);
            break; /* VectorK */

        case FunctionK:
            verificaFimInstrucaoAnterior();
            emitComment("-> function declaration", indent);
            emitComment(tree->attr.name, indent);

            /* Se for função de input ou output não gera código intermediário */
            if(strcmp(tree->attr.name, "input") && strcmp(tree->attr.name, "output")) {
                op1.kind = String;
                op1.contents.variable.name = tree->attr.name;
                op1.contents.variable.scope = tree->scope;
                insertQuad(createQuad(FUNC, op1, vazio, vazio));

                /* list of parameters */
                p1 = tree->child[0];
                while(p1 != NULL) {
                    op2.kind = String;
                    op2.contents.variable.name = p1->child[0]->attr.name;
                    op2.contents.variable.scope = p1->child[0]->scope;
                    insertQuad(createQuad(GET_PARAM, op2, vazio, vazio));
                    p1 = p1->sibling;
                }

                /* build code for function block */
                p2 = tree->child[1];
                cGen(p2);
            }

            emitComment("<- function declaration", indent);
            break;

        case CallK:
            emitComment("-> function call", indent);
            emitComment(tree->attr.name, indent);
            /* Argumentos */
            p1 = tree->child[0];
            /* Atribui o primeiro operando */
            op1.kind = String;
            op1.contents.variable.name = tree->attr.name;
            op1.contents.variable.scope = tree->scope;
            /* Atribui o segundo operando */
            qtdParams = getQuantidadeParametros(tree);
            pushParam(&qtdParams);
            if(qtdParams > 0) {
                op2.kind = IntConst;
                op2.contents.val = qtdParams;
            } else {
                op2.kind = IntConst;
                op2.contents.val = 0;
            }
            /* build code for function call */
            instrucaoAtual = PARAM_LIST;
            op3.kind = IntConst;
            op3.contents.val = getQuantidadeParametros(tree);
            insertQuad(createQuad(instrucaoAtual, op3, vazio, vazio));
            emitComment("-> function call: arguments", indent);
            while(p1 != NULL) {
                cGen(p1);
                /* Atribui o tipo de instrução */
                instrucaoAtual = SET_PARAM;
                /* Cria e insere uma nova representação em código intermediário */
                insertQuad(createQuad(instrucaoAtual, operandoAtual, vazio, vazio));
                /* Decrementa qtdParams */
                --qtdParams;
                /* Se for um chamado de OUTPUT, verifica o display de exibição */
                if(!strcmp(tree->attr.name, "output") && p1->sibling == NULL) {
                    display = p1->attr.val;
                }
                p1 = p1->sibling;
            }
            popParam();
            emitComment("<- function call: arguments", indent);
            /* Atribui o tipo de instrução */
            instrucaoAtual = CALL;
            /* Atualiza o operando atual */
            operandoAtual = createTemporaryOperand();
            /* Cria e insere uma nova representação em código intermediário */
            q = createQuad(instrucaoAtual, op1, op2, operandoAtual);
            if(display != -1) {
                q->display = display;
            }
            insertQuad(q);
            emitComment("<- function call", indent);
            break;

        case OpK:
            if(tree->attr.op == ATRIBUICAO) {

                emitComment("-> assign", indent);
                p1 = tree->child[0];
                p2 = tree->child[1];
                op3 = vazio;

                /* Gera código para o operando da direita */
                emitComment("-> assign: right argument", indent);
                cGen(p2);
                /* Atribui como o segundo operando */
                op2 = operandoAtual;
                emitComment("<- assign: right argument", indent);

                /* Gera código para o operando da esquerda */
                emitComment("-> assign: left argument", indent);
                cGen(p1);
                /* Atribui como o primeiro operando */
                op1 = operandoAtual;
                emitComment("<- assign: left argument", indent);

                // ------------------- Caso especial ------------------- //
                /* Na atribuição, se o operando da esquerda for um vetor, deve-se
                 * fazer um store na posição de memória dessa variável
                 */
                if(p1->kind.exp == VectorK) {
                     ultimaQuadrupla->instruction = VEC_ADDR;
                     // Se o índice for uma constante, adiciona o offset como o 3º operando (para código objeto)
                     if(ultimaQuadrupla->op2.kind == IntConst) {
                         op3.kind = IntConst;
                         op3.contents.val = ultimaQuadrupla->op2.contents.val;

                         op1 = ultimaQuadrupla->op1;
                     }
                }

                /* Atribui o tipo de instrução */
                instrucaoAtual = ASN;
                /* Cria e insere uma nova representação em código intermediário */
                insertQuad(createQuad(instrucaoAtual, op1, op2, op3));
                emitComment("<- assign", indent);

            } else {

                emitComment("-> arithmetic operator", indent);
                p1 = tree->child[0];
                p2 = tree->child[1];
                /* Gera código para o operando da esquerda */
                emitComment("-> arithmetic operator: left argument", indent);
                cGen(p1);
                /* Atribui como o primeiro operando */
                op1 = operandoAtual;
                emitComment("<- arithmetic operator: left argument", indent);
                /* Gera código para o operando da direita */
                emitComment("-> arithmetic operator: right argument", indent);
                cGen(p2);
                /* Atribui como o segundo operando */
                op2 = operandoAtual;
                emitComment("<- arithmetic operator: right argument", indent);

                switch (tree->attr.op) {
                    case MAIS:
                        emitComment("arithmetic operator: +", indent);
                        /* Atribui o tipo de instrução */
                        instrucaoAtual = ADD;
                        break;
                    case MENOS:
                        emitComment("arithmetic operator: -", indent);
                        /* Atribui o tipo de instrução */
                        instrucaoAtual = SUB;
                        break;
                    case VEZES:
                        emitComment("arithmetic operator: *", indent);
                        /* Atribui o tipo de instrução */
                        instrucaoAtual = MULT;
                        break;
                    case DIVISAO:
                        emitComment("arithmetic operator: /", indent);
                        /* Atribui o tipo de instrução */
                        instrucaoAtual = DIV;
                        break;
                    case MODULO:
                        emitComment("modulo operator: %", indent);
                        /* Atribui o tipo de instrução */
                        instrucaoAtual = MOD;
                        break;
                    case MENOR:
                        emitComment("relational operator: <", indent);
                        /* Atribui o tipo de instrução */
                        instrucaoAtual = LT;
                        break;
                    case MENORIGUAL:
                        emitComment("relational operator: <=", indent);
                        /* Atribui o tipo de instrução */
                        instrucaoAtual = LET;
                        break;
                    case MAIOR:
                        emitComment("relational operator: >", indent);
                        /* Atribui o tipo de instrução */
                        instrucaoAtual = GT;
                        break;
                    case MAIORIGUAL:
                        emitComment("relational operator: >=", indent);
                        /* Atribui o tipo de instrução */
                        instrucaoAtual = GET;
                        break;
                    case IGUAL:
                        emitComment("relational operator: ==", indent);
                        /* Atribui o tipo de instrução */
                        instrucaoAtual = EQ;
                        break;
                    case DIFERENTE:
                        emitComment("relational operator: !=", indent);
                        /* Atribui o tipo de instrução */
                        instrucaoAtual = NE;
                        break;
                    default:
                        emitComment("BUG: Unknown operator", indent);
                        break;
                } /* case op */

                /* Atualiza o operando atual */
                operandoAtual = createTemporaryOperand();
                insertQuad(createQuad(instrucaoAtual, op1, op2, operandoAtual));
                emitComment("<- Operator", indent);

            }
            break; /* OpK */
    default:
        break;
    }
    UNINDENT;
} /* genExp */

/* Procedimento cGen gera o código recursivamente
 * pela árvore sintática
 */
static void cGen(TreeNode * tree) {
    if (tree != NULL) {
        switch (tree->nodekind) {
            case StmtK:
                genStmt(tree);
                break;
            case ExpK:
                genExp(tree);
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
            insertQuad(createQuad(RTN, vazio, vazio, vazio));
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
void codeGen(TreeNode * syntaxTree, char * codefile) {
    char * s = (char *) malloc(strlen(codefile) + 7);
    strcpy(s,"Arquivo: ");
    strcat(s,codefile);
    emitComment("Compilação C- para código intermediário", indent);
    emitComment(s, indent);
    /* Antes de iniciar, prepara o operando vazio */
    preparaVazio();
    cGen(syntaxTree);
    /* finish */
    emitComment("Fim da execução.", indent);
    insertQuad(createQuad(HALT, vazio, vazio, vazio));

    emitCode("\n********** Código intermediário **********\n");
    printIntermediateCode();
}

void printIntermediateCode() {
    Quadruple q = head;
    char quad[100];

    while(q != NULL) {
        sprintf(quad, "%d: (", q->linha);
        strcat(quad, toString(q->instruction));

        if(q->op1.kind == String) {
            strcat(quad, ", ");
            strcat(quad, q->op1.contents.variable.name);
        } else if(q->op1.kind == IntConst) {
            sprintf(tempString, ", %d", q->op1.contents.val);
            strcat(quad,tempString);
        } else {
            strcat(quad, ", _");
        }

        if(q->op2.kind == String) {
            strcat(quad, ", ");
            strcat(quad, q->op2.contents.variable.name);
        } else if(q->op2.kind == IntConst) {
            sprintf(tempString, ", %d", q->op2.contents.val);
            strcat(quad,tempString);
        } else {
            strcat(quad, ", _");
        }

        if(q->op3.kind == String) {
            strcat(quad, ", ");
            strcat(quad, q->op3.contents.variable.name);
            strcat(quad, ")");
        } else if(q->op3.kind == IntConst) {
            sprintf(tempString, ", %d", q->op3.contents.val);
            strcat(quad,tempString);
            strcat(quad, ")");
        } else {
            strcat(quad, ", _)");
        }
        emitCode(quad);
        q = q->next;
    }
}

void pushLocation(LocationStack ls) {
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

LocationStack createLocation(Quadruple * quad) {
    LocationStack ls = (LocationStack) malloc(sizeof(struct Location));
    ls->quad = quad;
    ls->next = NULL;
    return ls;
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

void preparaVazio(void) {
    vazio.kind = Empty;
    vazio.contents.variable.name = NULL;
    vazio.contents.variable.scope = NULL;
}

Quadruple getCodigoIntermediario(void) {
    return head;
}
