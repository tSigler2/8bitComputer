#ifndef CONTROL_H
#define CONTROL_H

#include <stdbool.h>
#include "clock.h"

typedef struct ControlReg{
    char name;
    bool content, actLine;

    Clockstate update;
} ControlReg;

typedef struct Control{
    bool halt, jump, add, andOp, orOp, notOp, lda, 
    write, ldm, ldc, input, reset, screen;

    int reg1Sel, reg2Sel, depReg;

    ControlReg reg[6];
} Control;

#endif