#ifndef CPU_H
#define CPU_H

#include "clock.h"
#include "register.h"
#include "mem.h"
#include "control.h"
#include "screen.h"

typedef struct CPU{
    Register regs[6];
    Clock clock;
    RAM ram;
    Control control;
    Screen screen;
    bool immMode;
} CPU;

static inline void registerUpdate(CPU* cpu, u8 value);
static inline void aluOperations(CPU* cpu);
static inline void memOps(CPU* cpu);

#endif
