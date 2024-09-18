#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdint.h>
#include "clock.h"

typedef uint8_t u8;

typedef struct Register{
    char name;
    u8 val;
    bool actLine;
    ClockState update;
} Register;

inline void updateRegister(struct Register* r, u8 val, Clock* clock, bool reset);
inline void asynReset(struct Register* r);

#endif