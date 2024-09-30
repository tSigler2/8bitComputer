#!/bin/zsh

gcc asm/*.c -std=c17 -o asm.o -g
gcc emu/*.c -std=c17 -o emu.o -g -lSDL2main -lSDL2
