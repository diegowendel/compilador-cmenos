/****************************************************/
/* Arquivo: main.c                                  */
/* Main para o compilador da máquina iZero          */
/* Diego Wendel de Oliveira Ferreira                */
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

/* NO_TARGET_CODE igual a TRUE não gera código objeto,
 * caso contrário gera código objeto
 */
#define NO_TARGET_CODE FALSE

/* NO_TARGET_CODE igual a TRUE não gera código binário,
 * caso contrário gera código binário
 */
#define NO_BINARY_CODE FALSE

#include "util.h"
#include "cli.h"
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
#if !NO_BINARY_CODE
#include "binary.h"
#endif
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
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode = TRUE;
int TraceTarget = TRUE;
int TraceBinary = TRUE;

int Error = FALSE;
int codigoIntermediarioGerarado = FALSE;
int codigoObjetoGerado = FALSE;

int main(int argc, char * argv[]) {
    TreeNode * syntaxTree;

    if (argc == 1) {
        mostrarErroArgumentos(argv[0]);
    }

    CodeInfo codeInfo = interpretar(argc, argv);    
    listing = stdout; /* send listing to screen */
    fprintf(listing, "\nCOMPILAÇÃO C MENOS: %s\n", codeInfo.pgm);
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
        int fnlen = strcspn(codeInfo.pgm, ".");
        printf("\n\n\nalo: %s\n\n\n", codeInfo.pgm);
        codefile = (char *) calloc(fnlen + 4, sizeof(char));
        strncpy(codefile, codeInfo.pgm, fnlen);
        strcat(codefile, ".txt");
        code = fopen(codefile, "w");
        if (code == NULL) {
            printf("Incapaz de abrir %s\n", codefile);
            exit(1);
        }
        if (TraceCode) fprintf(listing, "\nGerando código intermediário...\n");
        codeGen(syntaxTree, codefile);
        fclose(code);
        if (TraceCode) fprintf(listing, "\nGeração de código intermediário concluída!\n");
        // Código intermediário gerado com sucesso
        codigoIntermediarioGerarado = TRUE;
    }
#if !NO_TARGET_CODE
    if(codigoIntermediarioGerarado) {
        char * codefile;
        int fnlen = strcspn(codeInfo.pgm, ".");
        codefile = (char *) calloc(fnlen + 4, sizeof(char));
        strncpy(codefile, codeInfo.pgm, fnlen);
        strcat(codefile, ".txt");
        code = fopen(codefile, "a+");
        Quadruple codigoIntermediario = getCodigoIntermediario();
        if (TraceTarget) fprintf(listing, "\nGerando código objeto...\n");
        geraCodigoObjeto(codigoIntermediario, codeInfo.codeType);
        fclose(code);
        if (TraceTarget) fprintf(listing, "\nGeração de código objeto concluída!\n");
        // Código objeto gerado com sucesso
        codigoObjetoGerado = TRUE;
    }
#if !NO_BINARY_CODE
    if(codigoObjetoGerado) {
        char * codefile;
        int fnlen = strcspn(codeInfo.pgm, ".");
        codefile = (char *) calloc(fnlen + 4, sizeof(char));
        strncpy(codefile, codeInfo.pgm, fnlen);
        strcat(codefile, ".txt");
        code = fopen(codefile, "a+");
        Objeto codigoObjeto = getCodigoObjeto();
        if (TraceBinary) fprintf(listing, "\nGerando código binário...\n");
        geraCodigoBinarioComDeslocamento(codigoObjeto, codeInfo.codeType, codeInfo.offset);
        fclose(code);
        if (TraceBinary) fprintf(listing, "\nGeração de código binário concluída!\n\n");
    }
#endif
#endif
#endif
#endif
#endif
    fclose(source);
    return 0;
}
