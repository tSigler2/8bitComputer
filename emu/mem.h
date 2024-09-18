#ifndef MEM_H
#define MEM_H

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;

typedef struct Memory{
    u8 mem[65536];
    u16 memPointer;
} RAM;

#endif