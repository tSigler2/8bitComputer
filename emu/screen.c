#include "screen.h"

void initScreen(Screen* screen){
    int renderFlags, windowFlags;

    renderFlags = SDL_RENDERER_ACCELERATED;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Error Opening SDL Window: %s\n", SDL_GetError());
        exit(1);
    }

    windowFlags = 0;

    screen->window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_DIMS, SCREEN_DIMS, windowFlags);

    if(!screen->window){
        printf("Error Initializing SDL Window\n");
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    screen->renderer = SDL_CreateRenderer(screen->window, -1, renderFlags);

    if(!screen->renderer){
        printf("Error Initializing SDL Renderer\n");
        exit(1);
    }
}
