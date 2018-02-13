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
        "in", "out",
        "jf",
        "j", "jal", "halt",
        "ldk", "sdk",
        "lim", "sim",
        "ckhd", "ckim", "ckdm",
        "mmuLowerIM", "mmuUpperIM", "mmuLowerDM", "mmuUpperDM", "mmuSelect",
        "syscall", "exec", // AO ALTERAR AQUI, LEMBRAR DE ALTERAR TAMÉM O CÓDIGO DO SO, QUE USA O OPCODE DO SYSCALL
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
        "$rz", "$v0", "$v1", "$out", "$inv", "$gp", "$a0", "$a1",
        "$a2", "$a3", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5",
        "$s6", "$s7", "$s8", "$s9", "$t0", "$t1", "$t2", "$t3",
        "$t4", "$t5", "$t6", "$t7", "$t8", "$t9", "$sp", "$ra"
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
        // in,    out,
        "010011", "010100",
        // jf,
        "010101",
        // j,     jal,      halt
        "010110", "010111", "011000",
        // ldk,   sdk
        "011001", "011010",
        // lim,   sim
        "011011", "011100",
        // ckhd,   ckim,    ckdm
        "011101", "011110", "011111",
        // mmuLowerIM, mmuUpperIM, mmuLowerDM, mmuUpperDM, mmuSelect,
        "100000", "100001", "100010", "100011", "100100",
        // syscall, exec,
        "100101", "100110",
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
        // "$rz", "$v0",  "$v1",   "$out",  "$inv",  "$gp",   "$a0",   "$a1",
        "00000", "00001", "00010", "00011", "00100", "00101", "00110", "00111",
        // "$a2", "$a3",  "$s0",   "$s1",   "$s2",   "$s3",   "$s4",   "$s5"
        "01000", "01001", "01010", "01011", "01100", "01101", "01110", "01111",
        // "$s6", "$s7",  "$s8",   "$s9",   "$t0",   "$t1",   "$t2",   "$t3"
        "10000", "10001", "10010", "10011", "10100", "10101", "10110", "10111",
        // "$t4", "$t5",  "$t6",   "$t7",   "$t8",   "$t9",   "$sp",   "$ra"
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
