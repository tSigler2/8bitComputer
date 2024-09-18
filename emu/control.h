#ifndef CONTROL_H
#define CONTROL_H

#include <stdbool>

typedef struct Control{
    bool halt, jump, add, andOp, orOp, notOp, lda, 
    write, ldm, ldc, input, reset, screen;

    int reg1Sel, reg2Sel, depReg;
} Control;

#endif