/****************************************************/
/* File: util.c	                                    */
/* Diego Wendel de Oliveira Ferreira		    */
/* 86774                               		    */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 */
void printToken(TokenType token, const char* tokenString) {
    switch (token) {
        case IF: fprintf(listing,"if\n"); break;
        case ELSE: fprintf(listing, "else\n"); break;
        case WHILE: fprintf(listing,"while\n"); break;
        case RETURN: fprintf(listing,"return\n"); break;
        case INT: fprintf(listing,"int\n"); break;
        case VOID: fprintf(listing,"void\n"); break;
        case MAIS: fprintf(listing,"+\n"); break;
        case MENOS: fprintf(listing,"-\n"); break;
        case VEZES: fprintf(listing,"*\n"); break;
        case DIVISAO: fprintf(listing,"/\n"); break;
        case MODULO: fprintf(listing, "%%\n"); break;
        case SHIFT_LEFT: fprintf(listing, "<<\n"); break;
        case SHIFT_RIGHT: fprintf(listing, ">>\n"); break;
        case AND: fprintf(listing, "&\n"); break;
        case OR: fprintf(listing, "|\n"); break;
        case XOR: fprintf(listing, "^\n"); break;
        case NOT: fprintf(listing, "!\n"); break;
        case LOGICAL_AND: fprintf(listing, "&&\n"); break;
        case LOGICAL_OR: fprintf(listing, "||\n"); break;
        case ATRIBUICAO: fprintf(listing, "=\n"); break;
        case ATRIB_MAIS: fprintf(listing, "+=\n"); break;
        case ATRIB_MENOS: fprintf(listing, "-=\n"); break;
        case ATRIB_VEZES: fprintf(listing, "*=\n"); break;
        case ATRIB_DIVISAO: fprintf(listing, "/=\n"); break;
        case ATRIB_MODULO: fprintf(listing, "%%=\n"); break;
        case ATRIB_AND: fprintf(listing, "&=\n"); break;
        case ATRIB_OR: fprintf(listing, "|=\n"); break;
        case ATRIB_XOR: fprintf(listing, "^=\n"); break;
        case ATRIB_SHIFT_LEFT: fprintf(listing, "<<=\n"); break;
        case ATRIB_SHIFT_RIGHT: fprintf(listing, ">>=\n"); break;
        case MENOR: fprintf(listing, "<\n"); break;
        case MENORIGUAL: fprintf(listing, "<=\n"); break;
        case MAIOR: fprintf(listing, ">\n"); break;
        case MAIORIGUAL: fprintf(listing, ">=\n"); break;
        case IGUAL: fprintf(listing, "==\n"); break;
        case DIFERENTE: fprintf(listing, "!=\n"); break;
        case QUESTION: fprintf(listing, "?\n"); break;
        case COLON: fprintf(listing, ":\n"); break;
        case SEMI: fprintf(listing, ";\n"); break;
        case COMMA: fprintf(listing, ",\n"); break;
        case LPAREN: fprintf(listing, "(\n"); break;
        case RPAREN: fprintf(listing, ")\n"); break;
        case LBRACKET: fprintf(listing, "[\n"); break;
        case RBRACKET: fprintf(listing, "]\n"); break;
        case LKEY: fprintf(listing, "{\n"); break;
        case RKEY: fprintf(listing, "}\n"); break;
        case ENDFILE: fprintf(listing, "EOF\n"); break;
        case NUM: fprintf(listing, "NUM, val= %s\n", tokenString); break;
        case ID: fprintf(listing, "ID, name= %s\n", tokenString); break;
        case ERROR: fprintf(listing, "ERROR: %s\n", tokenString); break;
        default: /* should never happen */
        fprintf(listing, "Token desconhecido: %d\n", token);
    }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind kind) {
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t == NULL) {
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    } else {
        for (i = 0; i < MAXCHILDREN; i++) {
            t->child[i] = NULL;
        }
        t->sibling = NULL;
        t->node = STMTK;
        t->lineno = lineno;
        t->kind.stmt = kind;
    }
    return t;
}

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind) {
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t == NULL) {
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    } else {
        for (i = 0; i < MAXCHILDREN; i++) {
            t->child[i] = NULL;
        }
        t->sibling = NULL;
        t->node = EXPK;
        t->lineno = lineno;
        t->kind.exp = kind;
    }
    return t;
}

TreeNode * newVarNode(VarKind kind) {
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t == NULL) {
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    } else {
        for (i = 0; i < MAXCHILDREN; i++) {
            t->child[i] = NULL;
        }
        t->sibling = NULL;
        t->node = VARK;
        t->lineno = lineno;
        t->kind.var.varKind = kind;
        t->kind.var.acesso = DECLK;
        t->kind.var.scope = NULL;
    }
    return t;
}

TreeNode * newSysNode(SysCallKind kind) {
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t == NULL) {
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    } else {
        for (i = 0; i < MAXCHILDREN; i++) {
            t->child[i] = NULL;
        }
        t->sibling = NULL;
        t->node = SYSK;
        t->lineno = lineno;
        t->kind.sys = kind;
    }
    return t;
}

char * toStringSysCall(SysCallKind syscall) {
    char * strings[] = {
        "input", "output", "ldk", "sdk", "lam", "sam", "sim",
        "mmuLowerIM", "mmuUpperIM",
        "mmuSelect", "exec", "lcd", "lcdPgms", "lcdCurr",
        "gic", "cic", "gip", "execAgain",
        "saveRegs", "loadRegs", "ldm", "sdm",
        "gsp", "gspb", "ggpb", "sspb", "sgpb", "rgnsp"
    };
    return strings[syscall];
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s) {
    int n;
    char * t;

    if (s == NULL) {
        return NULL;
    }

    n = strlen(s)+1;
    t = (char *) malloc(n);

    if (t == NULL) {
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    } else {
        strcpy(t,s);
    }
    return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno += 2
#define UNINDENT indentno -= 2

/* printSpaces indents by printing spaces */
void printSpaces(void) {
    int i;
    for (i = 0; i < indentno; i++) {
        fprintf(listing," ");
    }
}

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * tree ) {
    int i;
    INDENT;
    while (tree != NULL) {
        printSpaces();
        if (tree->node == STMTK) {
            switch (tree->kind.stmt) {
                case INTEGERK: fprintf(listing, "int\n"); break;
                case VOIDK: fprintf(listing, "void\n"); break;
                case IFK: fprintf(listing, "if\n"); break;
                case WHILEK: fprintf(listing, "while\n"); break;
                case RETURNK: fprintf(listing, "return\n");	break;
                case COMPK: fprintf(listing, "Declaração Composta:\n");	break;
            }
        } else if (tree->node == EXPK) {
            switch (tree->kind.exp) {
                case ATRIBK: fprintf(listing, "Atrib Op: "); printToken(tree->op, "\0"); break;
                case RELK: fprintf(listing, "Rel Op: "); printToken(tree->op, "\0"); break;
                case ARITHK: fprintf(listing, "Arith Op: "); printToken(tree->op, "\0"); break;
                case LOGICK: fprintf(listing, "Logic Op: "); printToken(tree->op, "\0"); break;
                case UNARYK: fprintf(listing, "Unary Op: "); printToken(tree->op, "\0"); break;
            }
        } else if (tree->node == VARK) {
            switch (tree->kind.var.varKind) {
                case IDK: fprintf(listing, "Id: %s\n", tree->kind.var.attr.name); break;
                case VECTORK: fprintf(listing, "Vector: %s\n", tree->kind.var.attr.name); break;
                case CONSTK: fprintf(listing, "Const: %d\n", tree->kind.var.attr.val); break;
                case FUNCTIONK: fprintf(listing, "Function: %s\n", tree->kind.var.attr.name); break;
                case CALLK: fprintf(listing, "Function call: %s\n", tree->kind.var.attr.name); break;
            }
        } else if (tree->node == SYSK) {
            fprintf(listing, "System call: %s\n", toStringSysCall(tree->kind.sys));
        }
        for (i = 0; i < MAXCHILDREN; i++) {
            printTree(tree->child[i]);
        }
        tree = tree->sibling;
    }
    UNINDENT;
}

/*
 * Procedimento que reverte uma string só utilizando o operador XOR,
 * é conhecido como XOR SWAP
 */
void reverse(char * str, int length) {
    char *p1, *p2;
    for (p1 = str, p2 = str + length - 1; p2 > p1; ++p1, --p2) {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
}

// Implementation of itoa()
char * itoa(int num, char * str, int base) {
    int i = 0;
    int isNegative = FALSE;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = TRUE;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative) {
        str[i++] = '-';
    }
    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);
    return str;
}
