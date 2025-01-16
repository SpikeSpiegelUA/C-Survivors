#pragma once

#include "Entities/Man.h"
#include "Entities/Star.h"
#include "Entities/Ledge.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "Entities/Bullet.h"
#include "Entities/EnemyJarhead.h"
#include "Entities/Background.h"
#include "Entities/Particle.h"

typedef struct {

    float scrollX;
    //Players
    Man man;

    //Stars
    Star stars[100];

    //Bullets.
    BulletVector* bulletVector;

    //Enemies.
    EnemyJarheadVector* enemyJarheadVector;

    //Particles.
    ParticleVector* particleVector;

    //Ledges
    Ledge ledges[100];

    //Background
    Background backgrounds[15];

    //Hardware
    SDL_Renderer* renderer;
    SDL_Window* window;

    int time, deathCountdown;
    int statusState;

    //Surfaces
    SDL_Surface* fontSurface;

    //Textures
    SDL_Texture* starTexture;
    SDL_Texture* brickTexture;
    SDL_Texture* fireTexture;
    SDL_Texture* manTexture;
    SDL_Texture* backgroundTexture;
    SDL_Texture* label;
    SDL_Texture* bulletTexture;
    SDL_Texture* enemyJarheadTexture;
    SDL_Texture* bloodParticleTexture;

    //Sounds
    Mix_Chunk* deathMixChunk, *landMixChunk, *jumpMixChunk, *shotMixChunk, *enemyDeathMixChunk;
    Mix_Music* bgMusic;

    //Fonts
    TTF_Font* font;
} GameState;