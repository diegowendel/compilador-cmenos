/****************************************************/
/* Arquivo: cli.c                                   */
/* Implementação da Command Line Interface          */
/* Diego Wendel de Oliveira Ferreira                */
/****************************************************/

#include "globals.h"
#include "cli.h"

void mostrarErroArgumentos(char * argv) {
    fprintf(stderr, "Uso: %s <arquivo> <opção>\n", argv);
    exit(1);
}

void mostrarOpcoes(void) {
    printf("Compilador C- para a máquina iZero...\n");
    printf("Opções:\n");
    printf("\t-b, --bios\tCompila código para a bios\n");
    printf("\t-k, --kernel\tCompila código para o kernel\n");
    printf("\t-h, --help\tExibe este menu\n");
    printf("\toffset (int)\tOffset na compilação do código\n");
    printf("O padrão é compilar o código de um programa normal sem offset.\n");
}

int temOpcaoAjuda(int argc, char * argv[]) {
    int i;
    for (i = 0; i < argc; i++) {
        char * opcao = argv[i];
        if (!strcmp(opcao, "-h") || !strcmp(opcao, "--help")) {
            return TRUE;
        }
    }
    return FALSE;
}

CodeInfo interpretar(int argc, char * argv[]) {
    if (temOpcaoAjuda(argc, argv)) {
        mostrarOpcoes();
        exit(0);
    }
    
    char pgm[120]; /* Nome do código fonte */
    strcpy(pgm, argv[1]) ;
    if (strchr (pgm, '.') == NULL) {
        strcat(pgm, ".c");
    }

    source = fopen(pgm, "r");
    if (source == NULL) {
        fprintf(stderr, "Arquivo %s não encontrado!\n", pgm);
        exit(1);
    }

    CodeInfo codeInfo;
    strcpy(codeInfo.pgm, pgm);
    codeInfo.offset = 0;
    if (argc == 2) {
        codeInfo.codeType = PROGRAMA;
    } else if (argc == 3) {
        char * opcao = argv[2];
        if (!strcmp(opcao, "-k") || !strcmp(opcao, "--kernel")) {
            codeInfo.codeType = KERNEL;
        } else if (!strcmp(opcao, "-b") || !strcmp(opcao, "--bios")) {
            codeInfo.codeType = BIOS;
        } else {
            codeInfo.codeType = PROGRAMA;
            codeInfo.offset = atoi(opcao);
        }
    } else {
        mostrarErroArgumentos(argv[0]);
    }
    return codeInfo;
}
