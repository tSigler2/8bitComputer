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
    int immMode;
} CPU;

static inline void registerUpdate(CPU* cpu, u8 value);
static inline void aluOperations(CPU* cpu);
static inline void memOps(CPU* cpu);
static inline void immOperation(CPU* cpu, u8 val);
static inline void inputOperation(CPU* cpu, u8 val);
static inline void keyboardOperation(CPU* cpu);
void instructProcess(CPU* cpu, u8 instruction);

#endif
