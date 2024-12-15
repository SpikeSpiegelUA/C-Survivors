#ifndef CSURVIVORS_H
#define CSURVIVORS_H

#include <stdlib.h>
#include <stdbool.h>
#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>
#include <time.h>
#include "ArrayFunctions.c"
#undef main

#define GRAVITY 0.35f

typedef struct {
    float x, y;
    float dy;
    char name[31];
} Man;

typedef struct {
    int x, y, w, h;
} Ledge;

typedef struct {
    int x, y;
} Star;

typedef struct {
    //Players
    Man man;

    //Stars
    Star stars[100];

    //Ledges
    Ledge ledges[100];

    //Hardware
    SDL_Renderer* renderer;

    //Textures
    SDL_Texture* starTexture;
    SDL_Texture* manFrames[3];
    SDL_Texture* brickTexture;
} GameState;

void LoadGame(GameState* gameState) {
    gameState->man.x = 220.f;
    gameState->man.y = 70.f;
    gameState->man.dy = 0.f;
    SDL_Surface* starSurface = NULL;
    SDL_Surface* manIdleSurface = NULL;
    SDL_Surface* manLeftLegSurface = NULL;
    SDL_Surface* manRightLegSurface = NULL;
    SDL_Surface* brickSurface = NULL;

    starSurface = IMG_Load("..\\Images\\star.png");
    if (starSurface == NULL) {
        printf("Star image not found!!!\n");
        SDL_Quit();
        exit(1);
    }

    manIdleSurface = IMG_Load("..\\Images\\Player\\Idle.png");
    if (manIdleSurface == NULL) {
        printf("Image not found!!!\n");
        SDL_Quit();
        exit(1);
    }

    manLeftLegSurface = IMG_Load("..\\Images\\Player\\LeftLeg.png");
    if (manLeftLegSurface == NULL) {
        printf("Image not found!!!\n");
        SDL_Quit();
        exit(1);
    }

    manRightLegSurface = IMG_Load("..\\Images\\Player\\RightLeg.png");
    if (manRightLegSurface == NULL) {
        printf("Image not found!!!\n");
        SDL_Quit();
        exit(1);
    }

    brickSurface = IMG_Load("..\\Images\\brick.png");
    if (brickSurface == NULL) {
        printf("Image not found!!!\n");
        SDL_Quit();
        exit(1);
    }

    gameState->starTexture = SDL_CreateTextureFromSurface(gameState->renderer, starSurface);
    SDL_FreeSurface(starSurface);
    gameState->manFrames[0] = SDL_CreateTextureFromSurface(gameState->renderer, manIdleSurface);
    SDL_FreeSurface(manIdleSurface);
    gameState->manFrames[1] = SDL_CreateTextureFromSurface(gameState->renderer, manLeftLegSurface);
    SDL_FreeSurface(manLeftLegSurface);
    gameState->manFrames[2] = SDL_CreateTextureFromSurface(gameState->renderer, manRightLegSurface);
    SDL_FreeSurface(manRightLegSurface);
    gameState->brickTexture = SDL_CreateTextureFromSurface(gameState->renderer, brickSurface);
    SDL_FreeSurface(brickSurface);

    for (int i = 0; i < sizeof(gameState->stars) / sizeof(Star); i++){
        gameState->stars[i].x = rand()%640;
        gameState->stars[i].y = rand()%480;
    }

    for (int Index = 0; Index < ArrayLength(gameState->ledges) - 1; Index++) {
        gameState->ledges[Index].h = 64;
        gameState->ledges[Index].w = 64;
        gameState->ledges[Index].x = Index * 64;
        gameState->ledges[Index].y = 400;
    }

    gameState->ledges[99].h = 64;
    gameState->ledges[99].w = 64;
    gameState->ledges[99].x = 350;
    gameState->ledges[99].y = 200;
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
            case SDLK_UP:
                if (gameState->man.dy >= 0.f) {
                    gameState->man.dy = -12.f;
                }
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

    return false;
}

void JumpProcess(GameState* gameState) {
    Man* man = &gameState->man;
    man->y += man->dy;
    man->dy += GRAVITY;
}

void CollisionDetection(GameState* gameState) {
    //TODO: better collision detection.
    for (int i = 0; i < ArrayLength(gameState->ledges); i++) {
        float mw = 64.f, mh = 64.f;
        float mx = gameState->man.x, my = gameState->man.y;
        float bw = (float)gameState->ledges[i].w, bh = (float)gameState->ledges[i].h,
            bx = (float)gameState->ledges[i].x, by = (float)gameState->ledges[i].y;

        if (my + mh > by && my < by + bh) {
            //Bumping right side of a brick.
            if (mx < bx + bw && mx + mw > bx + bw) {
                mx = bx + bw;
                gameState->man.x = mx;
            }
            //Bumping left side of a brick.
            else if (mx < bx && mx + mw > bx) {
                mx = bx - mw;
                gameState->man.x = mx;
            }
        }

        if (mx + mw > bx && mx < bx + bw) {
            //Bumping top of a brick.
            if (my < by && my + mh > by) {
                gameState->man.dy = 0.f;
                gameState->man.y = my;
            }
            //Bumping bottom of a brick.
            else if(my < by + bh && my > by) {
                gameState->man.dy = 0.f;
                gameState->man.y = my;
            }
        }
    }
}

void RenderFrame(SDL_Renderer* renderer, GameState* gameState){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    SDL_Rect manRect = { (int)gameState->man.x, (int)gameState->man.y, 64, 64 };
    SDL_RenderCopyEx(renderer, gameState->manFrames[0], NULL, &manRect, 0, NULL, 0);

    for (int i = 0; i < ArrayLength(gameState->ledges); i++) {
        SDL_Rect ledgeRect = { gameState->ledges[i].x, gameState->ledges[i].y, gameState->ledges[i].w, gameState->ledges[i].h};
        SDL_RenderCopy(renderer, gameState->brickTexture, NULL, &ledgeRect);
    }

    /*for (int i = 0; i < sizeof(gameState->stars) / sizeof(Star); i++) {
        SDL_Rect newStar = {gameState->stars[i].x, gameState->stars[i].y, 64, 64};
        SDL_RenderCopy(renderer, gameState->starTexture, NULL, &newStar);
    }*/

    SDL_RenderPresent(renderer);
}

int main()
{
    srand((unsigned int)(time(NULL)));
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
        JumpProcess(&gameState);
        CollisionDetection(&gameState);
        RenderFrame(renderer, &gameState);
    }


    SDL_DestroyTexture(gameState.starTexture);
    SDL_DestroyTexture(gameState.brickTexture);
    for (short Index = ArrayLength(gameState.manFrames) - 1; Index >= 0 ; Index--)
        SDL_DestroyTexture(gameState.manFrames[Index]);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_Quit();

    return 0;
}
#endif
