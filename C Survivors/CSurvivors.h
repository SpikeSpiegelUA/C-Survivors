#ifndef CSURVIVORS_H
#define CSURVIVORS_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "SDL_mixer.h"
#include "ArrayFunctions.h"
#include "GlobalVariables.h"

#define STATUS_STATE_LIVES 0
#define STATUS_STATE_GAME 1
#define STATUS_STATE_GAMEOVER 2

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

typedef struct {
    float x, y;
    float w, h;
    float dx, dy;
    char name[31];
    short lives;
    bool onLedge, isDead;
    int animFrame;
    bool facingRight;
} Man;

typedef struct {
    int x, y, w, h;
} Ledge;

typedef struct {
    float x, y;
    float w, h;
} Star;

typedef struct {
    
    float scrollX;
    //Players
    Man man;

    //Stars
    Star stars[100];

    //Ledges
    Ledge ledges[100];

    //Hardware
    SDL_Renderer* renderer;
    SDL_Window* window;

    int time, deathCountdown;
    int statusState;

    //Surfaces
    SDL_Surface* fontSurface;

    //Textures
    SDL_Texture* starTexture;
    SDL_Texture* manFrames[3];
    SDL_Texture* brickTexture;
    SDL_Texture* fireTexture;
    SDL_Texture* label;
    
    //Sounds
    Mix_Chunk *deathMixChunk, * landMixChunk, * jumpMixChunk;
    Mix_Music* bgMusic;

    //Fonts
    TTF_Font* font;
} GameState;
#endif