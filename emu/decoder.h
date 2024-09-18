#ifndef DECODE_H
#define DECODE_H

#include "cpu.h"
#include <stdbool.h>

#define CONTROL(_r, _c) (((_r) || ((_c) && !(_r))) ? false : true)

static inline void controlRegOps(u8 lower, Control* control){
    control->reg[lower].content = CONTROL(control->reg[lower].content, true);
}

static inline void exitImmMode(CPU* cpu){
    cpu->immMode = false;

    for(int i = 0; i < 6; i++) cpu->control.reg[i].content = CONTROL(cpu->control.reg[i].content, false);
}

static inline void aluOps(u8 lower, Control* control){
    switch((lower & 0x07)){
        case 0x00:
            control->reg1Sel = 0;
            control->reg2Sel = 1;
            break;
        case 0x01:
            control->reg1Sel = 0;
            control->reg2Sel = 2;
            break;
        case 0x02:
            control->reg1Sel = 0;
            control->reg2Sel = 3;
            break;
        case 0x03:
            control->reg1Sel = 1;
            control->reg2Sel = 2;
            break;
        case 0x04:
            control->reg1Sel = 1;
            control->reg2Sel = 3;
            break;
        case 0x05:
            control->reg1Sel = 2;
            control->reg2Sel = 3;
            break;
    }

    if((lower & 0x08)){
        control->depReg = 1;
        return;
    }
    control->depReg = 2;
    return;
}

static inline void resetALU(Control* control){
    control->reg1Sel = 0;
    control->reg2Sel = 0;
    control->depReg = 0;
}

static inline void reset(CPU* cpu, u8 inst){
    if(inst != 0x00) cpu->control.halt = false;
    if(inst != 0x10) cpu->control.jump = false;
    if(inst != 0x20) cpu->control.add = false;
    if(inst != 0x30) cpu->control.andOp = false;
    if(inst != 0x40) cpu->control.orOp = false;
    if(inst != 0x50) cpu->control.notOp = false;
    if(inst != 0x60) cpu->control.lda = false;
    if(inst != 0x70) cpu->control.write = false;
    if(inst != 0x80) cpu->control.ldm = false;
    if(inst != 0x90) cpu->control.ldc = false;
    if(inst != 0xa0) cpu->control.input = false;
    if(inst != 0xb0) cpu->control.reset = false;
    if(inst != 0xf0) cpu->control.screen = false;
}

void intDecode(u8 instruction, CPU* cpu);

#endif