#include "emu.h"
#include "cpu.h"
#include "decoder.h"

int main(int argc, char** argv){
    if(argc < 2){
        perror("No Input File");
        exit(1);
    }
    inFile = NULL;

    inPerSec = 0;

    for(int i = 1; i < argc; i++){
        if(!strcmp(argv[i], "-t")){
            inPerSec = atoi(argv[i+1]);
            i++;
        }
        else if(inFile == NULL){
            inFile = fopen(argv[i], "rb");

            if(inFile == NULL){
                perror("Failed to Open File");
                exit(1);
            }
        }
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
            .memPointer = 0x0000
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

            .reg = {
                [0] = {.name = 'a', .content = false, .actLine = false, .update = FALLING},
                [1] = {.name = 'b', .content = false, .actLine = false, .update = FALLING},
                [2] = {.name = 'c', .content = false, .actLine = false, .update = FALLING},
                [3] = {.name = 'd', .content = false, .actLine = false, .update = FALLING},
                [4] = {.name = 'l', .content = false, .actLine = false, .update = FALLING},
                [5] = {.name = 'h', .content = false, .actLine = false, .update = FALLING}
            }
        }
    };

    printf("%d\n", sizeof(cpu));

    u8 buf[1];

    while(!(cpu.control.halt)){
        fread(buf, sizeof(buf), 1, inFile);
        intDecode(*buf, &cpu);
    }

    //fclose(inFile);

    return 0;
}