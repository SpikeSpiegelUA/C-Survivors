#include "stdlib.h"
#include "SDL.h"
#include "SDL_image.h"
#include "stdbool.h"
#undef main

#ifndef CSURVIVORS_H
#define CSURVIVORS_H

typedef struct {
    int x, y;
    char name[31];
} Man;

typedef struct {
    int x, y;
} Star;

typedef struct {
    //Players
    Man man;

    //Stars
    Star stars[100];

    //Hardware
    SDL_Renderer* renderer;

    //Textures
    SDL_Texture* star;
} GameState;

void LoadGame(GameState* gameState) {
    gameState->man.x = 220;
    gameState->man.y = 70;
    SDL_Surface* starSurface = NULL;

    starSurface = IMG_Load("C:\\Visual Studio Projects\\C-Survivors\\Images\\star.png");
    if (starSurface == NULL) {
        printf("Image not found!!!\n");
        SDL_Quit();
        exit(1);
    }

    gameState->star = SDL_CreateTextureFromSurface(gameState->renderer, starSurface);
    SDL_FreeSurface(starSurface);

    for (int i = 0; i < sizeof(gameState->stars) / sizeof(Star); i++){
        gameState->stars[i].x = rand()%640;
        gameState->stars[i].y = rand()%480;
    }
}

bool ProcessEvents(GameState* gameState) {
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
        gameState->man.x += 5;
    }
    if (state[SDL_SCANCODE_LEFT]) {
        gameState->man.x -= 5;
    }
    if (state[SDL_SCANCODE_UP]) {
        gameState->man.y -= 5;
    }
    if (state[SDL_SCANCODE_DOWN]) {
        gameState->man.y += 5;
    }
    return false;
}

void RenderFrame(SDL_Renderer* renderer, GameState* gameState){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_Rect manRect = { gameState->man.x, gameState->man.y, 140, 140 };
    SDL_RenderFillRect(renderer, &manRect);

    for (int i = 0; i < sizeof(gameState->stars) / sizeof(Star); i++) {
        SDL_Rect newStar = {gameState->stars[i].x, gameState->stars[i].y, 64, 64};
        SDL_RenderCopy(renderer, gameState->star, NULL, &newStar);
    }

    SDL_RenderPresent(renderer);
}

int main()
{
    srand(time(NULL));
    GameState gameState;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    SDL_InitSubSystem(SDL_INIT_VIDEO);
    
    window = SDL_CreateWindow("C Survivors", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    gameState.renderer = renderer;

    LoadGame(&gameState);

    bool done = false;

    while (!done) {
        done = ProcessEvents(&gameState);
        RenderFrame(renderer, &gameState);
    }

    SDL_DestroyTexture(gameState.star);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_Quit();

    return 0;
}
#endif
