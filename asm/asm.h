#ifndef ASM_H
#define ASM_H

#include <ctype.h>
#include <stdbool.h>
#include "gen.h"

#define MAX(_a, _b) ({\
    __typeof__(_a) __a = _a\
    __typeof__(_b) __b = _b\
    __a > __b ? __a : __b\
})

#define MIN(_a, _b) ({\
    __typeof__(_a) __a = _a\
    __typeof__(_b) __b = _b\
    __a < __b ? __a : __b\
})

void deallocToken(struct Token* tok, int i, int j);

#define RAM_LIMIT 65536
#define ROM_LIMIT 65536

#endif