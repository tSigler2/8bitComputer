#include "clock.h"

ClockState clockForward(ClockState cs){
    switch(cs){
        case LOW:
            return RISING;
        case RISING:
            return HIGH;
        case HIGH:
            return FALLING;
        case FALLING:
            return LOW;
        default:
            perror("Clock State Error\n");
            exit(1);
    }
}