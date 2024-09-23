#ifndef CLOCK_H
#define CLOCK_H

#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#define NANO_TO_SECS 1000000000

struct timespec start, end;

typedef enum ClockState{
    LOW,
    RISING,
    HIGH,
    FALLING
} ClockState;

int inPerSec;

#define SETSPEED(_s) ((_s)/4)

ClockState clockForward(ClockState cs);

typedef struct Clock{
    ClockState cs;
    int speed;
} Clock;

#endif
