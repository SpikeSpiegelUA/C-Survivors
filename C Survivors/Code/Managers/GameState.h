#pragma once

#include "Entities/Man.h"
#include "Entities/Star.h"
#include "Entities/Ledge.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "Entities/Bullet.h"

typedef struct {

    float scrollX;
    //Players
    Man man;

    //Stars
    Star stars[100];

    //Bullets.
    BulletVector* bulletVector;

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
    SDL_Texture* bulletTexture;

    //Sounds
    Mix_Chunk* deathMixChunk, * landMixChunk, * jumpMixChunk;
    Mix_Music* bgMusic;

    //Fonts
    TTF_Font* font;
} GameState;