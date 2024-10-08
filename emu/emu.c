#include "emu.h"
#include "cpu.h"
#include "decoder.h"

int main(int argc, char** argv){
    if(argc < 2){
        perror("No Input File");
        exit(1);
    }
    inFile = NULL;
    verbose = false;

    inPerSec = 0;

    for(int i = 1; i < argc; i++){
        if(!strcmp(argv[i], "-t")){
            inPerSec = atoi(argv[i+1]);
            i++;
        }
        else if(!strcmp(argv[i], "-v")) verbose = true;
        else if(inFile == NULL){
            inFile = fopen(argv[i], "rb");

            if(inFile == NULL){
                perror("Failed to Open File");
                exit(1);
            }
        }
    }

    inFile = fopen("bin/fibonacchi.bin", "rb");


    if(inFile == NULL){
        perror("Failed to Open File");
        exit(1);
    }

    if(!inPerSec) inPerSec = 100;
    
    CPU cpu = {
        .immMode = false,
        .regs = {
            [0] = {.name = 'A', .val = 0x00, .actLine = false, .update = FALLING},
            [1] = {.name = 'B', .val = 0x00, .actLine = false, .update = FALLING},
            [2] = {.name = 'C', .val = 0x00, .actLine = false, .update = FALLING},
            [3] = {.name = 'D', .val = 0x00, .actLine = false, .update = FALLING},
            [4] = {.name = 'L', .val = 0x00, .actLine = false, .update = FALLING},
            [5] = {.name = 'H', .val = 0x00, .actLine = false, .update = FALLING}
        },
        .clock = {
            .cs = LOW,
            .speed = SETSPEED(inPerSec)
        },
        .ram = {
            .mem = {0x00},
            .memPointer = 0x0000,
            .read = false,
            .write = false
        },
        .control = {
            .halt = false,
            .jump = false,
            .add = false,
            .andOp = false,
            .orOp = false,
            .notOp = false,
            .lda = false,
            .write = false,
            .ldm = false,
            .ldc = false,
            .input = false,
            .reset = false,
            .screen = false,

            .reg1Sel = 0,
            .reg2Sel = 0,
            .depReg = 0,
            .regSelect = 0,
            .inSelect = 0,

            .reg = {
                [0] = {.name = 'a', .actLine = false, .update = FALLING},
                [1] = {.name = 'b', .actLine = false, .update = FALLING},
                [2] = {.name = 'c', .actLine = false, .update = FALLING},
                [3] = {.name = 'd', .actLine = false, .update = FALLING},
                [4] = {.name = 'l', .actLine = false, .update = FALLING},
                [5] = {.name = 'h', .actLine = false, .update = FALLING}
            },

            .flags = {false},

            .valLines = {0x00}
        },
        .screen = {
            .update = RISING,
            .actLine = false,
            .colorRegister = 0x00
        }
    };
    
    initScreen(&cpu.screen);

    u8 instruction;

    struct timespec ts;
    while(!(cpu.control.halt)){
        clock_gettime(CLOCK_REALTIME, &ts);
        if(cpu.clock.cs == FALLING) fread(&instruction, sizeof(u8), 1, inFile);
        intDecode(instruction, &cpu);
        if(cpu.clock.cs == FALLING && verbose) printf("Instruction: %x File Position: %ld ImmMode: %d\n", instruction, ftell(inFile), cpu.immMode);
        if(cpu.clock.cs == FALLING && verbose) printf("A: %x B: %x C: %x D: %x L: %x H: %x ZF: %d\n", cpu.regs[0].val, cpu.regs[1].val, cpu.regs[2].val, cpu.regs[3].val, cpu.regs[5].val, cpu.regs[4].val, cpu.control.flags[2]);
        instructProcess(&cpu, instruction);
        cpu.clock.cs = clockForward(cpu.clock.cs);

        if(cpu.control.jump && cpu.control.flags[2] && cpu.clock.cs == FALLING) fseek(inFile, (long)((((u16)cpu.regs[4].val) << 8) + ((u16) cpu.regs[5].val)), SEEK_SET);
        while(ts.tv_nsec % cpu.clock.speed != (long)0) clock_gettime(CLOCK_REALTIME, &ts);
    }

    fclose(inFile);
    SDL_DestroyWindow(cpu.screen.window);
    SDL_DestroyRenderer(cpu.screen.renderer);

    return 0;
}
