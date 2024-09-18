#include "decoder.h"

void intDecode(u8 instruction, CPU* cpu){
    u8 intHead = (instruction & 0xf0);
    u8 intTail = (instruction & 0x0f);
    switch(intHead){
        case 0x00:
            cpu->control->halt = true;
            break;
        case 0x10:
            cpu->control->jump = true;
        case 0x20:
            cpu->control->add = true;
            break;
        case 0x30:
            cpu->control->andOp = true;
            break;
        case 0x40:
            cpu->control->orOp = true;
            break;
        case 0x50:
            cpu->control->notOp = true;
            break;
        case 0x60:
            cpu->control->lda = true;
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
            break;
        case 0xf0:
            cpu->control->reset = true;
            break;
        default:
            perror("Unrecognized Instruction");
            exit(1);
    }
}