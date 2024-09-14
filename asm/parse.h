#ifndef PARSE_H
#define PARSE_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#else
#include <sys/stat.h>
#endif

typedef uint8_t u8;
typedef uint16_t u16;

enum Op{
    HALT = (u8)0x00,
    JUMP = (u8)0x10,
    ADD = (u8)0x20,
    AND = (u8)0x30,
    OR = (u8)0x40,
    NOT = (u8)0x50,
    LOADI = (u8)0x60,
    WRITE = (u8)0x70,
    LOADC = (u8)0x80,
    LOADM = (u8)0x90,
    INPUT = (u8)0xa0,
    RESET = (u8)0xb0,
    SCREEN = (u8)0xf0
};

enum TokenType{
    TK_PERIOD = '.',
    TK_SEMICOLON = ';',
    TK_COLON = ':',
    TK_PLUS = '+',
    TK_MINUS = '-',
    TK_STAR = '*',
    TK_SLASK = '/',
    TK_OPENP = '(',
    TK_CLOSEP = ')',
    TK_CONST,
    TK_NUM,
    TK_EQ,
    TK_FUNC,
    TK_REG,
    TK_VAR,
    TK_ROOT,
    TK_DATA,
    TK_OP
};

enum Register{
    A = (u8)0x00,
    B = (u8)0x01,
    C = (u8)0x02,
    D = (u8)0x03,
    L = (u8)0x04,
    H = (u8)0x05
};

struct Token{
    int number;
    enum TokenType type;
    struct Token** children;
    int childNum;
    u8 data;

    int src;
    char* srcData;
} program;

void parse(FILE* asmFile, struct Token* program);

char* parseFolder(char* path);

u8 getRegister(char c);

enum TokenType getType(char* keyword);

void readArgs(struct Token** tok, int tokCount, int* count, int*lineCount);

#endif