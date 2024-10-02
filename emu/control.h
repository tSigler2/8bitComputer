#ifndef CONTROL_H
#define CONTROL_H

#include <stdbool.h>
#include "clock.h"

typedef struct ControlReg{
    char name;
    bool actLine;

    ClockState update;
} ControlReg;

typedef struct Control{
    bool halt, jump, add, andOp, orOp, notOp, lda, 
    write, ldm, ldc, input, reset, screen, present;

    int reg1Sel, reg2Sel, depReg, regSelect, inSelect;

    ControlReg reg[6];
    bool flags[3];

    u8 valLines[4];
} Control;

#endif
