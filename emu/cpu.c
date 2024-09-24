#include "cpu.h"

static inline void registerUpdate(CPU* cpu, u8 value){
    for(int i = 0; i < 6; i++){
        if(cpu->regs[i].actLine && cpu->clock.cs == cpu->regs[i].update) cpu->regs[i].val = value;
    }
}

static inline void aluOperations(CPU* cpu){
    u8 result;
    
    if(cpu->control.add) result = cpu->regs[cpu->control.reg1Sel].val + cpu->regs[cpu->control.reg2Sel].val;
    else if(cpu->control.andOp) result = cpu->regs[cpu->control.reg1Sel].val & cpu->regs[cpu->control.reg2Sel].val;
    else if(cpu->control.orOp) result = cpu->regs[cpu->control.reg1Sel].val | cpu->regs[cpu->control.reg2Sel].val;
    else if(cpu->control.notOp) result = ~(cpu->regs[cpu->control.reg1Sel].val);
    else{
        return;
    }

    if(!(result & 0x00)) cpu->control.flags[2] = true;
    if((result & 0x80)) cpu->control.flags[1] = true;
    if((result < cpu->regs[cpu->control.reg1Sel].val) || (result < cpu->regs[cpu->control.reg2Sel].val)) cpu->control.flags[0] = true;
    
    cpu->control.valLines[0] = result;
}

static inline void memOps(CPU* cpu){
    if(cpu->ram.read) cpu->control.valLines[1] = cpu->ram.mem[((((u16)cpu->regs[5].val) << 8) + ((u16)cpu->regs[4].val))];
    if(cpu->ram.write) cpu->ram.mem[((((u16)cpu->regs[5].val) << 8) + ((u16)cpu->regs[4].val))] = cpu->regs[cpu->control.regSelect].val;
}

static inline void immOperation(CPU* cpu, u8 val){
    if(cpu->immMode) cpu->control.valLines[2] = val;
}

static inline void keyboardOperation(CPU* cpu){
    u8 input = 0x00;
    while(SDL_PollEvent(&cpu->screen.event) != 0){
        if(cpu->screen.event.key.keysym.sym == SDLK_UP) input += 0x10;
        else if(cpu->screen.event.key.keysym.sym == SDLK_DOWN) input += 0xf0;
        else{
            input += 0x80;
        }

        if(cpu->screen.event.key.keysym.sym == SDLK_W) input += 0x01;
        else if(cpu->screen.event.key.keysym.sym == SDLK_DOWN) input += 0x0f;
        else{
            input += 0x08;
        }
    }

    cpu->control.valLines[3] = input;
}

void instructProcess(CPU* cpu, u8 instruction){
    aluOperations(cpu);
    memOps(cpu);
    immOperation(cpu, instruction);
    inputOperation(cpu, instruction);
    registerUpdate(cpu, cpu->control.valLines[cpu->control.inSelect]);
}
