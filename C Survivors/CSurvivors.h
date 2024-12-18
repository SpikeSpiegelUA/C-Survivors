#ifndef CSURVIVORS_H
#define CSURVIVORS_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ArrayFunctions.h"
#include "GlobalVariables.h"

#define STATUS_STATE_LIVES 0
#define STATUS_STATE_GAME 1
#define STATUS_STATE_GAMEOVER 2

typedef struct {
    float x, y;
    float dx, dy;
    char name[31];
    bool onLedge;
    int animFrame;
    bool facingRight;
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

    int time;
    int statusState;

    //Textures
    SDL_Texture* starTexture;
    SDL_Texture* manFrames[3];
    SDL_Texture* brickTexture;
    SDL_Texture* label;

    TTF_Font* font;
} GameState;
#endif