/****************************************************/
/* File: code.c                                     */
/* Author: Diego Ferreira                           */
/****************************************************/

#include "globals.h"
#include "code.h"

const char * toStringInstruction(enum instrucao i) {
    const char * strings[] = {
        "addition", "subtraction", "multiplication", "division", "modulo",
        "bitwise_and", "bitwise_or", "bitwise_xor", "not", "logical_and", "logical_or",
        "shift_left", "shift_right", "vector_value", "vector_address",
        "equal", "not_equal", "less_than", "less_than_equal_to",
        "greater_than", "greater_than_equal_to", "assign",
        "function", "return", "get_param", "set_param", "call", "param_list",
        "jump_if_false", "goto", "label", "syscall", "halt"
    };
    return strings[i];
}

const char * toStringOpcode(Opcode op) {
    const char * strings[] = {
        "addi", "subi", "muli", "divi", "modi",
        "andi", "ori", "xori", "not", "landi", "lori",
        "slli", "srli",
        "mov", "lw", "li", "la", "sw",
        "in", "out", "jf",        
        "ldk", "sdk", "lam", "sam", "sim",
        "mmuLowerIM", "mmuUpperIM", "mmuSelect",
        "lcd", "lcdPgms", "lcdCurr",
        "gic", "cic", "gip", "preIO",
        "syscall", "exec", "execAgain", // AO ALTERAR AQUI, LEMBRAR DE ALTERAR TAMBÉM O CÓDIGO DO SO, QUE USA O OPCODE DO SYSCALL
        "j", "jtm", "jal", "halt",
        "rtype"
    };
    return strings[op];
}

const char * toStringFunction(Function func) {
    const char * strings[] = {
        "add", "sub", "mul", "div", "mod",
        "and", "or", "xor", "land", "lor",
        "sll", "srl",
        "eq", "ne", "lt", "let", "gt", "get",
        "jr", 
        "dont_care"
    };
    return strings[func];
}

const char * toStringRegName(RegisterName rn) {
    const char * strings[] = {
        "$rz", "$a0", "$a1", "$a2", "$a3", "$s0", "$s1", "$s2",
        "$s3", "$s4", "$s5", "$s6", "$s7", "$s8", "$s9", "$t0",
        "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8",
        "$v0", "$k0", "$k1", "$gpb", "$spb", "$gp", "$sp", "$ra"
    };
    return strings[rn];
}

const char * toBinaryOpcode(Opcode op) {
    const char * strings[] = {
        // addi,  subi,     muli,     divi,     modi
        "000001", "000010", "000011", "000100", "000101",
        // andi,  ori,      xori,     not,      landi,    lori
        "000110", "000111", "001000", "001001", "001010", "001011",
        // slli,  srli,
        "001100", "001101",
        // mov,   lw,       li,       la,       sw
        "001110", "001111", "010000", "010001", "010010",
        // in,    out,      jf,
        "010011", "010100", "010101",
        // ldk,   sdk,      lam,      sam,      sim,
        "010110", "010111", "011000", "011001", "011010",
        // mmuLowerIM, mmuUpperIM, mmuSelect,
        "011011", "011100", "011101",
        // lcd,   lcdPgms,  lcdCurr,
        "011110", "011111", "100000",
        // gic,   cic,      gip,      preIO,
        "100001", "100010", "100011", "100100",
        // syscall, exec,   execAgain,
        "111001", "111010", "111011",
        // j,     jtm,      jal,      halt
        "111100", "111101", "111110", "111111",
        // rtype
        "000000"
    };
    return strings[op];
}

const char * toBinaryFunction(Function func) {
    const char * strings[] = {
        // add,   sub,      mul,      div,      mod
        "000000", "000001", "000010", "000011", "000100",
        // and,   or,       xor,      land,     lor,
        "000101", "000110", "000111", "001000", "001001",
        // sll,   srl
        "001010", "001011",
        // eq,    ne,       lt,       let,      gt,       get
        "001100", "001101", "001110", "001111", "010000", "010001",
        // jr,
        "010010",
        // dont_care,
        "XXXXXX"
    };
    return strings[func];
}

const char * toBinaryRegister(RegisterName rn) {
    const char * strings[] = {
        // "$rz", "$a0", "$a1"," $a2", "$a3", "$s0", "$s1", "$s2",
        "00000", "00001", "00010", "00011", "00100", "00101", "00110", "00111",
        // "$s3", "$s4", "$s5", "$s6", "$s7", "$s8", "$s9", "$t0",
        "01000", "01001", "01010", "01011", "01100", "01101", "01110", "01111",
        // "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8",
        "10000", "10001", "10010", "10011", "10100", "10101", "10110", "10111",
        // "$v0", "$k0", "$k1", "$gpb", "$spb", "$gp", "$sp", "$ra"
        "11000", "11001", "11010", "11011", "11100", "11101", "11110", "11111"
    };
    return strings[rn];
}


void emitSpaces(int indent){
    int i;
    for(i = 0; i < indent; ++i) {
        fprintf(code, " ");
    }
}

void emitCode(const char * c) {
    fprintf(code, "%s\n", c);
}

void emitBinary(const char * c) {
    fprintf(binary_file, "%s\n", c);
}

void emitComment(const char * c, int indent) {
    if (TraceCode) {
        emitSpaces(indent);
        fprintf(code, "# %s\n", c);
    }
}

void emitObjectCode(const char * c, int indent) {
    emitSpaces(indent);
    fprintf(code, "%s\n", c);
}
