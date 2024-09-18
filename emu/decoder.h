#ifndef DECODE_H
#define DECODE_H

#include "cpu.h"
#include <stdbool.h>

#define CONTROL(_r, _c) (((_r) || ((_c) && !(_r))) ? false : true)

inline void aluOps(u8 lower, Control* control);
inline void controlRegOps(u8 lower, Control* control);
inline void resetALU(Control* control);
inline void reset(CPU* cpu);
void intDecode(u8 instruction, CPU* cpu);

#endif