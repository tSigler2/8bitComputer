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
            perror("Clock State Error");
            exit(1);
    }
}