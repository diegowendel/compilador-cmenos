/****************************************************/
/* File: main.c                                     */
/* Main program for TINY compiler                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"

/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE FALSE
/* set NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE FALSE

/* set NO_CODE to TRUE to get a compiler that does not
 * generate code
 */
#define NO_CODE FALSE

/* NO_TARGET_CODE igual a FALSE não gera código objeto,
 * caso contrário gera código objeto
 */
#define NO_TARGET_CODE FALSE

#include "util.h"
#if NO_PARSE
#include "scan.h"
#else
#include "parse.h"
#if !NO_ANALYZE
#include "analyze.h"
#if !NO_CODE
#include "cgen.h"
#if !NO_TARGET_CODE
#include "target.h"
#endif
#endif
#endif
#endif

/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;

/* allocate and set tracing flags */
int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode = TRUE;
int TraceTarget = TRUE;

int Error = FALSE;
int CodigoIntermediarioGerarado = FALSE;

int main( int argc, char * argv[] ) {
    TreeNode * syntaxTree;
    char pgm[120]; /* source code file name */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    strcpy(pgm,argv[1]) ;
    if (strchr (pgm, '.') == NULL) {
        strcat(pgm, ".tny");
    }
    source = fopen(pgm, "r");
    if (source == NULL) {
        fprintf(stderr, "Arquivo %s não encontrado\n", pgm);
        exit(1);
    }
    listing = stdout; /* send listing to screen */
    fprintf(listing, "\nCOMPILAÇÃO C MENOS: %s\n", pgm);
#if NO_PARSE
    while (getToken() != ENDFILE);
#else
    syntaxTree = parse();
    if (TraceParse) {
        fprintf(listing, "\nÁrvore sintática:\n\n");
        printTree(syntaxTree);
    }
#if !NO_ANALYZE
    if (! Error) {
        if (TraceAnalyze) fprintf(listing, "\nConstruindo tabela de símbolos...\n");
        buildSymtab(syntaxTree);
        if (TraceAnalyze) fprintf(listing, "\nChecando tipos...\n");
        typeCheck(syntaxTree);
        if (TraceAnalyze) fprintf(listing, "\nChecagem de tipos concluída!\n");
    }
#if !NO_CODE
    if (! Error) {
        char * codefile;
        int fnlen = strcspn(pgm, ".");
        codefile = (char *) calloc(fnlen + 4, sizeof(char));
        strncpy(codefile, pgm, fnlen);
        strcat(codefile, ".cm");
        code = fopen(codefile, "w");
        if (code == NULL) {
            printf("Incapaz de abrir %s\n", codefile);
            exit(1);
        }
        if (TraceCode) fprintf(listing, "\nGerando código intermediário...\n");
        codeGen(syntaxTree, codefile);
        fclose(code);
        if (TraceCode) fprintf(listing, "\nGeração de código intermediário concluída!\n\n");
        // Código intermediário gerado com sucesso
        CodigoIntermediarioGerarado = TRUE;
    }
#if !NO_TARGET_CODE
    if(CodigoIntermediarioGerarado) {
        char * codefile;
        int fnlen = strcspn(pgm, ".");
        codefile = (char *) calloc(fnlen + 4, sizeof(char));
        strncpy(codefile, pgm, fnlen);
        strcat(codefile, ".cm");
        code = fopen(codefile, "a+");
        Quadruple codigoIntermediario = getCodigoIntermediario();
        if (TraceTarget) fprintf(listing, "\nGerando código objeto...\n");
        geraCodigoObjeto(codigoIntermediario);
        fclose(code);
        if (TraceTarget) fprintf(listing, "\nGeração de código objeto concluída!\n\n");
    }
#endif
#endif
#endif
#endif
    fclose(source);
    return 0;
}
