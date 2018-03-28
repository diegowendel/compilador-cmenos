/****************************************************/
/* File: code.h                                     */
/* Author: Diego Ferreira                           */
/****************************************************/

#ifndef _CODE_H_
#define _CODE_H_

#include "cgen.h"
#include "target.h"

const char * toStringInstruction(enum instrucao i);

const char * toStringOpcode(Opcode op);

const char * toStringFunction(Function func);

const char * toStringRegName(RegisterName rn);

const char * toBinaryOpcode(Opcode op);

const char * toBinaryFunction(Function func);

const char * toBinaryRegister(RegisterName rn);

void emitCode(const char * c);

void emitBinary(const char * c);

void emitComment(const char * c, int indent);

void emitObjectCode(const char * c, int indent);

#endif
