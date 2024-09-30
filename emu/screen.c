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

void drawPixel(u8* xCoord, u8* yCoord, u8* color, ClockState* cs, Screen* screen, bool instruction){
    if(*cs != RISING || !instruction) return;

    SDL_Rect rect = {
        .x = *xCoord,
        .y = *yCoord,
        .w = 1,
        .h = 1
    };
    
    switch(*color){
        case 0x00:
            SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255);
            break;
        case 0x01:
            SDL_SetRenderDrawColor(screen->renderer, 16, 16, 16, 255);
            break;
        case 0x02:
            SDL_SetRenderDrawColor(screen->renderer, 32, 32, 32, 255);
            break;
        case 0x03:
            SDL_SetRenderDrawColor(screen->renderer, 48, 48, 48, 255);
            break;
        case 0x04:
            SDL_SetRenderDrawColor(screen->renderer, 64, 64, 64, 255);
            break;
        case 0x05:
            SDL_SetRenderDrawColor(screen->renderer, 80, 80, 80, 255);
            break;
        case 0x06:
            SDL_SetRenderDrawColor(screen->renderer, 96, 96, 96, 255);
            break;
        case 0x07:
            SDL_SetRenderDrawColor(screen->renderer, 112, 112, 112, 255);
            break;
        case 0x08:
            SDL_SetRenderDrawColor(screen->renderer, 128, 128, 128, 255);
            break;
        case 0x09:
            SDL_SetRenderDrawColor(screen->renderer, 144, 144, 144, 255);
            break;
        case 0x0a:
            SDL_SetRenderDrawColor(screen->renderer, 160, 160, 160, 255);
            break;
        case 0x0b:
            SDL_SetRenderDrawColor(screen->renderer, 176, 176, 176, 255);
            break;
        case 0x0c:
            SDL_SetRenderDrawColor(screen->renderer, 192, 192, 192, 255);
            break;
        case 0x0d:
            SDL_SetRenderDrawColor(screen->renderer, 208, 208, 208, 255);
            break;
        case 0x0e:
            SDL_SetRenderDrawColor(screen->renderer, 224, 224, 224, 255);
            break;
        case 0x0f:
            SDL_SetRenderDrawColor(screen->renderer, 255, 255, 255, 255);
            break;
        default:
            perror("Invalid Color");
            SDL_DestroyWindow(screen->window);
            SDL_DestroyRenderer(screen->renderer);
            exit(1);
    }

    if(SDL_RenderDrawRect(screen->renderer, &rect) != 0){
        perror("Renderer Error");
        SDL_DestroyWindow(screen->window);
        SDL_DestroyRenderer(screen->renderer);
        exit(1);
    }
}

void drawToScreen(Screen* screen, bool* command){
    if(!command) return;
    SDL_RenderPresent(screen->renderer);
    if(SDL_RenderFlush(screen->renderer) != 0){
        perror("Failed to Flush Renderer");
        SDL_DestroyWindow(screen->window);
        SDL_DestroyRenderer(screen->renderer);
        exit(1);
    }
}
