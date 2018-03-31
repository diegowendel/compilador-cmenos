/****************************************************/
/* Arquivo: cli.c                                   */
/* Command Line Interface                           */
/* Diego Wendel de Oliveira Ferreira                */
/****************************************************/

#ifndef _CLI_H_
#define _CLI_H_

#include "globals.h"

/*
 * Mostra erro de passagem de argumentos
 */
void mostrarErroArgumentos(char * argv);

/* 
 * Interpreta os comandos passados ao compilador
 */
CodeInfo interpretar(int argc, char * argv[]);

#endif
