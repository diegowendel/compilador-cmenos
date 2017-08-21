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
        case ELSE: fprintf(listing,"else\n"); break;
        case WHILE: fprintf(listing,"while\n"); break;
        case RETURN: fprintf(listing,"return\n"); break;
        case INT: fprintf(listing,"int\n"); break;
        case VOID: fprintf(listing,"void\n"); break;
        case MAIS: fprintf(listing,"+\n"); break;
        case MENOS: fprintf(listing,"-\n"); break;
        case VEZES: fprintf(listing,"*\n"); break;
        case DIVISAO: fprintf(listing,"/\n"); break;
        case MODULO: fprintf(listing, "%%\n"); break;
        case MENOR: fprintf(listing, "<\n"); break;
        case MENORIGUAL: fprintf(listing, "<=\n"); break;
        case MAIOR: fprintf(listing, ">\n"); break;
        case MAIORIGUAL: fprintf(listing, ">=\n"); break;
        case IGUAL: fprintf(listing, "==\n"); break;
        case DIFERENTE: fprintf(listing, "!=\n"); break;
        case ATRIBUICAO: fprintf(listing, "=\n"); break;
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
        t->nodekind = StmtK;
        t->kind.stmt = kind;
        t->lineno = lineno;
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
        t->nodekind = ExpK;
        t->kind.exp = kind;
        t->lineno = lineno;
        t->type = Void;
    }
    return t;
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
    t = malloc(n);

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
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno += 2
#define UNINDENT indentno -= 2

/* printSpaces indents by printing spaces */
static void printSpaces(void) {
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
        if (tree->nodekind == StmtK) {
            switch (tree->kind.stmt) {
                case IfK: fprintf(listing, "if\n"); break;
                case ElseK:	fprintf(listing, "else\n");	break;
                case WhileK: fprintf(listing, "while\n"); break;
                case ReturnK: fprintf(listing, "return\n");	break;
                case IntegerK: fprintf(listing, "int\n"); break;
                case VoidK: fprintf(listing, "void\n"); break;
                case CompK: fprintf(listing, "Declaração Composta:\n");	break;
            }
        } else if (tree->nodekind == ExpK) {
            switch (tree->kind.exp) {
                case OpK: fprintf(listing, "Op: "); printToken(tree->attr.op,"\0"); break;
                case ConstK: fprintf(listing, "Const: %d\n", tree->attr.val); break;
                case CallK: fprintf(listing, "Chamada de função: %s\n", tree->attr.name); break;
                case IdK: fprintf(listing, "Id: %s\n", tree->attr.name); break;
                case VectorK: fprintf(listing, "Vector: %s\n", tree->attr.name); break;
                case FunctionK: fprintf(listing, "Function: %s\n", tree->attr.name); break;
                default: fprintf(listing,"ExpNode desconhecido\n"); break;
            }
        } else {
            fprintf(listing, "Tipo desconhecido\n");
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
