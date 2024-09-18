#ifndef CPU_H
#define CPU_H

#include "clock.h"
#include "register.h"
#include "mem.h"
#include "control.h"

typedef struct CPU{
    Register regs[6];
    Clock clock;
    RAM ram;
    Control control;
    bool immMode;
} CPU;

#endif