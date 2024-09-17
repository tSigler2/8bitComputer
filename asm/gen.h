#ifndef GEN_H
#define GEN_H
#include "parse.h"
#include <stdbool.h>

#define ARRLEN(a) (sizeof(a)/sizeof((a)[0]))+1

u16* funcTracker;
bool loadImm;

u8 checkRegCombo(struct Token* reg1, struct Token* reg2);
u8 checkCommand(struct Token* token);
int findFuncAddress(char** funcAddressList, char* func, int childAmt);
void generateBinary(struct Token* program, FILE* outFile);
#endif