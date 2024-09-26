#include "decoder.h"

void intDecode(u8 instruction, CPU* cpu){
    u8 intHead = (instruction & 0xf0);
    u8 intTail = (instruction & 0x0f);

    if(!(intHead == 0x20) && !(intHead == 0x30) && !(intHead == 0x40) && !(intHead == 0x50)) resetALU(&cpu->control);
    if(cpu->immMode) return;

    switch(intHead){
        case 0x00:
            cpu->control.halt = true;
            break;
        case 0x10:
            cpu->control.jump = true;
            break;
        case 0x20:
            cpu->control.add = true;
            aluOps(intTail, &cpu->control);
            break;
        case 0x30:
            cpu->control.andOp = true;
            aluOps(intTail, &cpu->control);
            break;
        case 0x40:
            cpu->control.orOp = true;
            aluOps(intTail, &cpu->control);
            break;
        case 0x50:
            cpu->control.notOp = true;
            aluOps(intTail, &cpu->control);
            break;
        case 0x60:
            cpu->control.lda = true;
            controlRegOps(intTail, &cpu->control);
            cpu->immMode = 2;
            break;
        case 0x70:
            cpu->control.write = true;
            break;
        case 0x80:
            cpu->control.ldm = true;
            break;
        case 0x90:
            cpu->control.ldc = true;
            break;
        case 0xa0:
            cpu->control.input = true;
            break;
        case 0xb0:
            cpu->control.reset = true;
            break;
        case 0xf0:
            cpu->control.screen = true;
            break;
        default:
            perror("Unrecognized Instruction");
            exit(1);
    }
    reset(cpu, intHead);
}
