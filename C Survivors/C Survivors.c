#include "stdlib.h"
#include "SDL.h"
#include "stdbool.h"
#undef main

#ifndef CSURVIVORS_H
#define CSURVIVORS_H

typedef struct {
    int x, y;
    char name[31];
} Man;

bool ProcessEvents(Man* man) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_WINDOWEVENT_CLOSE:
            return true;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                return true;
            }
            break;
        case SDL_QUIT:
            return true;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_RIGHT]) {
        man->x += 5;
    }
    if (state[SDL_SCANCODE_LEFT]) {
        man->x -= 5;
    }
    if (state[SDL_SCANCODE_UP]) {
        man->y -= 5;
    }
    if (state[SDL_SCANCODE_DOWN]) {
        man->y += 5;
    }
    return false;
}

void RenderFrame(SDL_Renderer* Renderer, Man* man){
    SDL_SetRenderDrawColor(Renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Renderer);

    SDL_SetRenderDrawColor(Renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_Rect Rect = { man->x, man->y, 140, 220 };
    SDL_RenderFillRect(Renderer, &Rect);

    SDL_RenderPresent(Renderer);
}

int main()
{
    SDL_Window* Window;
    SDL_Renderer* Renderer;

    SDL_InitSubSystem(SDL_INIT_VIDEO);
    
    Window = SDL_CreateWindow("C Survivors", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

    bool done = false;

    Man man;
    man.x = 220;
    man.y = 140;

    while (!done) {
        done = ProcessEvents(&man);
        RenderFrame(Renderer, &man);
        SDL_Delay(10);
    }

    SDL_DestroyWindow(Window);
    SDL_DestroyRenderer(Renderer);

    SDL_Quit();

    return 0;
}
#endif
