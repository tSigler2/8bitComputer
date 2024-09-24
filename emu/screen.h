#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "clock.h"

#define SCREEN_DIMS 256
#define COLOR_MASK 0x0f

typedef uint8_t u8;

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
    ClockState update;
    bool actLine;
    u8 colorRegister;
} Screen;

void initScreen(Screen* screen);
void drawPixel(u8 xCoord, u8 yCoord, u8 color, ClockState cs);

#endif
