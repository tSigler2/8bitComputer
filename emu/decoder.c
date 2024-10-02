#include "decoder.h"

void intDecode(u8 instruction, CPU* cpu){
    u8 intHead = (instruction & 0xf0);
    u8 intTail = (instruction & 0x0f);

    reset(cpu, intHead);
    if(!(intHead == 0x20) && !(intHead == 0x30) && !(intHead == 0x40) && !(intHead == 0x50)) resetALU(&cpu->control);
    if(cpu->immMode > 0) return;
    if(cpu->clock.cs != FALLING) return;

    switch(intHead){
        case 0x00:
            cpu->control.halt = true;
            break;
        case 0x10:
            cpu->control.jump = true;
            break;
        case 0x20:
            cpu->control.add = true;
            cpu->control.inSelect = 0;
            aluOps(intTail, &cpu->control);
            break;
        case 0x30:
            cpu->control.andOp = true;
            cpu->control.inSelect = 0;
            aluOps(intTail, &cpu->control);
            break;
        case 0x40:
            cpu->control.orOp = true;
            cpu->control.inSelect = 0;
            aluOps(intTail, &cpu->control);
            break;
        case 0x50:
            cpu->control.notOp = true;
            cpu->control.inSelect = 0;
            aluOps(intTail, &cpu->control);
            break;
        case 0x60:
            cpu->control.lda = true;
            cpu->control.inSelect = 2;
            controlRegOps(intTail, &cpu->control);
            cpu->regs[intTail].actLine = cpu->control.reg[intTail].actLine;
            cpu->immMode = 2;
            break;
        case 0x70:
            cpu->control.write = true;
            cpu->control.regSelect = intTail;
            break;
        case 0x80:
            cpu->control.ldm = true;
            cpu->control.inSelect = 1;
            controlRegOps(intTail, &cpu->control);
            cpu->regs[intTail].actLine = cpu->control.reg[intTail].actLine;
            break;
        case 0x90:
            cpu->control.ldc = true;
            break;
        case 0xa0:
            cpu->control.input = true;
            cpu->control.inSelect = 3;
            break;
        case 0xb0:
            cpu->control.reset = true;
            break;
        case 0xe0:
            cpu->control.present = true;
            break;
        case 0xf0:
            cpu->control.screen = true;
            break;
        default:
            perror("Unrecognized Instruction");
            exit(1);
    }
}
