#include "stdlib.h"
#include "SDL.h"
#undef main

#ifndef CSURVIVORS_H
#define CSURVIVORS_H
int main()
{
    SDL_Window* Window;
    SDL_Renderer* Renderer;

    SDL_InitSubSystem(SDL_INIT_VIDEO);
    
    Window = SDL_CreateWindow("C Survivors", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(Renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    SDL_Rect Rect = { 220, 140, 140, 220 };
    SDL_RenderFillRect(Renderer, &Rect);

    SDL_RenderPresent(Renderer);

    SDL_Delay(20000);

    SDL_DestroyWindow(Window);
    SDL_DestroyRenderer(Renderer);

    SDL_Quit();

    return 0;
}
#endif
