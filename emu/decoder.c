#include "decoder.h"

inline void aluOps(u8 lower, Control* control){
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

inline void controlRegOps(u8 lower, Control* control){
    control->reg[lower].content = CONTROL(control->reg[lower]->content, true);
}

inline void exitImmMode(CPU* cpu){
    cpu->immMode = false;

    for(int i = 0; i < 6; i++) cpu->control.reg[i].content = CONTROL(cpu->control.reg[i].content, false);
}

inline void resetALU(Control* control){
    control->reg1Sel = 0;
    control->reg2Sel = 0;
    control->depReg = 0;
}

inline void reset(CPU* cpu){
    for(int i = 0; i < 6; i++){
        cpu->regs[i].val = 0x00;
        cpu->control.reg[i].val = false;
    }
}

void intDecode(u8 instruction, CPU* cpu){
    u8 intHead = (instruction & 0xf0);
    u8 intTail = (instruction & 0x0f);

    if(!(intHead == 0x20) && !(intHead == 0x30) && !(intHead == 0x40) && !(intHead == 0x50)) resetALU(&cpu.control);
    if(cpu->immMode) exitImmMode(cpu);

    switch(intHead){
        case 0x00:
            cpu->control->halt = true;
            break;
        case 0x10:
            cpu->control->jump = true;
            break;
        case 0x20:
            cpu->control->add = true;
            aluOps(intTail, &cpu->control);
            break;
        case 0x30:
            cpu->control->andOp = true;
            aluOps(intTail, &cpu->control);
            break;
        case 0x40:
            cpu->control->orOp = true;
            aluOps(intTail, &cpu->control);
            break;
        case 0x50:
            cpu->control->notOp = true;
            aluOps(intTail, &cpu->control);
            break;
        case 0x60:
            cpu->control->lda = true;
            controlRegOps(lower, &cpu->control);
            cpu->immMode = true;
            break;
        case 0x70:
            cpu->control->write = true;
            break;
        case 0x80:
            cpu->control->ldm = true;
            break;
        case 0x90:
            cpu->control->ldc = true;
            break;
        case 0xa0:
            cpu->control->input = true;
            break;
        case 0xb0:
            cpu->control->reset = true;
            reset(cpu);
            break;
        case 0xf0:
            cpu->control->screen = true;
            break;
        default:
            perror("Unrecognized Instruction");
            exit(1);
    }
}