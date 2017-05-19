/****************************************************/
/* File: code.h                                     */
/* Code emitting utilities for the TINY compiler    */
/* and interface to the TM machine                  */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _CODE_H_
#define _CODE_H_

#include "cgen.h"

/* code emitting utilities */

/* Procedimento emitComment imprime uma linha de comentario
 * com a string c no aquivo
 */
void emitSpaces(int indent);

void emitComment(const char * c, int indent);

void emitCode(const char * c);

/* Procedimento toString retorna o mnemônico do enum da instrução
 * passado como parâmetro
 */
const char * toString(enum instrucao i);

#endif
