#ifndef CSURVIVORS_C
#define CSURVIVORS_C

#include "CSurvivors.h"
#include "UI/Status.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "Utilities/ArrayFunctions.h"
#include "Utilities/GlobalVariables.h"
#include "Managers/GameState.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#undef main

void FreeMemoryAndQuit(GameState* gameState) {
    if (gameState->starTexture != NULL)
        SDL_DestroyTexture(gameState->starTexture);
    if (gameState->brickTexture != NULL)
        SDL_DestroyTexture(gameState->brickTexture);
    for (short Index = ArrayLength(gameState->manFrames) - 1; Index >= 0; Index--)
        if (gameState->manFrames[Index] != NULL)
            SDL_DestroyTexture(gameState->manFrames[Index]);
    if (gameState->font != NULL)
        TTF_CloseFont(gameState->font);
    if (gameState->bgMusic != NULL)
        Mix_FreeMusic(gameState->bgMusic);
    if (gameState->jumpMixChunk != NULL)
        Mix_FreeChunk(gameState->jumpMixChunk);
    if (gameState->landMixChunk != NULL)
        Mix_FreeChunk(gameState->landMixChunk);
    if (gameState->deathMixChunk != NULL)
        Mix_FreeChunk(gameState->deathMixChunk);

    if (gameState->window != NULL)
        SDL_DestroyWindow(gameState->window);
    if (gameState->renderer != NULL)
        SDL_DestroyRenderer(gameState->renderer);
    TTF_Quit();
    SDL_Quit();
    Mix_Quit();
    exit(1);
}

//Initialize all the textures, sounds, objects, etc...
void LoadGame(GameState* gameState) {

    //I don't want to store surfaces in gameState to free them in FreeMemoryAndQuit, so I will free them in the end of this function
    //and control that using this bool.
    bool error = false;

    //Load fonts.

    gameState->font = TTF_OpenFont("../Fonts/crazy-pixel.ttf", 48);
    if (gameState->font == NULL) {
        printf("Image not found!!!\n");
        error = true;
    }

    //Load surfaces.

    SDL_Surface* starSurface = NULL;
    SDL_Surface* manIdleSurface = NULL;
    SDL_Surface* manLeftLegSurface = NULL;
    SDL_Surface* manRightLegSurface = NULL;
    SDL_Surface* brickSurface = NULL;
    SDL_Surface* fireSurface = NULL;

    starSurface = IMG_Load("..\\Images\\star.png");
    if (starSurface == NULL) {
        printf("Star image not found!!!\n");
        error = true;
    }

    manIdleSurface = IMG_Load("..\\Images\\Player\\Idle.png");
    if (manIdleSurface == NULL) {
        printf("Image not found!!!\n");
        error = true;
    }

    manLeftLegSurface = IMG_Load("..\\Images\\Player\\LeftLeg.png");
    if (manLeftLegSurface == NULL) {
        printf("Image not found!!!\n");
        error = true;
    }

    manRightLegSurface = IMG_Load("..\\Images\\Player\\RightLeg.png");
    if (manRightLegSurface == NULL) {
        printf("Image not found!!!\n");
        error = true;
    }

    brickSurface = IMG_Load("..\\Images\\brick.png");
    if (brickSurface == NULL) {
        printf("Image not found!!!\n");
        error = true;
    }
    
    fireSurface = IMG_Load("..\\Images\\fire.png");
    if (fireSurface == NULL) {
        printf("Image not found!!!\n");
        error = true;
    }

    //Load sounds.

    gameState->bgMusic = Mix_LoadMUS("..\\Audio\\MainTheme.wav");
    if (gameState->bgMusic != NULL)
        Mix_VolumeMusic(32);
    gameState->deathMixChunk = Mix_LoadWAV("..\\Audio\\DeathSound.wav");
    gameState->jumpMixChunk = Mix_LoadWAV("..\\Audio\\JumpSound.wav");
    gameState->landMixChunk = Mix_LoadWAV("..\\Audio\\LandingSound.wav");
    if (gameState->deathMixChunk == NULL || gameState->jumpMixChunk == NULL || gameState->landMixChunk == NULL || gameState->bgMusic == NULL) {
        printf("Sound not found!!!\n");
        error = true;
    }

    //Initialize objects and create textures.
    gameState->man.x = 220.f;
    gameState->man.y = 70.f;
    gameState->man.w = (float)manIdleSurface->w;
    gameState->man.h = (float)manIdleSurface->h;
    gameState->man.dy = 0.f;
    gameState->man.dx = 0.f;
    gameState->man.currentSprite = 0;
    gameState->man.facingRight = true;
    gameState->man.lives = 3;
    gameState->man.isDead = false;
    gameState->man.onLedge = false;
    gameState->time = 0;
    gameState->scrollX = 0;
    gameState->deathCountdown = -1;
    gameState->statusState = STATUS_STATE_LIVES;

    gameState->starTexture = SDL_CreateTextureFromSurface(gameState->renderer, starSurface);
    gameState->manFrames[0] = SDL_CreateTextureFromSurface(gameState->renderer, manIdleSurface);
    gameState->manFrames[1] = SDL_CreateTextureFromSurface(gameState->renderer, manLeftLegSurface);
    gameState->manFrames[2] = SDL_CreateTextureFromSurface(gameState->renderer, manRightLegSurface);
    gameState->brickTexture = SDL_CreateTextureFromSurface(gameState->renderer, brickSurface);
    gameState->fireTexture = SDL_CreateTextureFromSurface(gameState->renderer, fireSurface);

    //Initialize bullets;
    //for (int index = 0; index < MAX_BULLETS; index++)
    //    gameState->bullets[index] = NULL;

    //Create stars.
    for (int i = 0; i < sizeof(gameState->stars) / sizeof(Star); i++){
        gameState->stars[i].x = (float)(rand()%640 * i+ SCREEN_WIDTH/2);
        gameState->stars[i].y = (float)(rand()%480);
        gameState->stars[i].w = (float)starSurface->w;
        gameState->stars[i].h = (float)starSurface->h;
    }

    //Create ledges.
    for (int Index = 0; Index < ArrayLength(gameState->ledges) - 1; Index++) {
        gameState->ledges[Index].h = 64.f;
        gameState->ledges[Index].w = 64.f;
        gameState->ledges[Index].x = Index * 64.f;
        gameState->ledges[Index].y = 400.f;
    }

    gameState->ledges[99].h = 64;
    gameState->ledges[99].w = 64;
    gameState->ledges[99].x = 350;
    gameState->ledges[99].y = 200;

    //Init UI.

    InitStatusLives(gameState);

    //Free all the dynamically allocated memory.

    SDL_FreeSurface(brickSurface);
    SDL_FreeSurface(manRightLegSurface);
    SDL_FreeSurface(manLeftLegSurface);
    SDL_FreeSurface(manIdleSurface);
    SDL_FreeSurface(starSurface);
    SDL_FreeSurface(fireSurface);

    if (error)
        FreeMemoryAndQuit(gameState);
}

bool ProcessEvents(GameState* gameState) {
    gameState->time++;
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
                if (gameState->man.onLedge) {
                    gameState->man.dy = -8.f;
                    gameState->man.onLedge = false;
                    Mix_PlayChannel(-1, gameState->jumpMixChunk, 0);
                }
            }
            break;
        case SDL_QUIT:
            return true;
        }
    }

    //Scancodes
    const Uint8* state = SDL_GetKeyboardState(NULL);

    //Jump enforce
    if (state[SDL_SCANCODE_UP]) {
        gameState->man.dy -= 0.2f;
    }

    //Walking
    if (state[SDL_SCANCODE_RIGHT]) {
        gameState->man.dx += 0.5f;
        if (gameState->man.dx > 6.f)
            gameState->man.dx = 6.f;
        gameState->man.facingRight = true;
    }
    else if (state[SDL_SCANCODE_LEFT]) {
        gameState->man.dx -= 0.5f;
        if (gameState->man.dx < -6.f)
            gameState->man.dx = -6.f;
        gameState->man.facingRight = false;
    }
    else {
        gameState->man.dx *= 0.8f;
        if (fabsf(gameState->man.dx) < 0.1f && fabsf(gameState->man.dx) > -0.1f)
            gameState->man.dx = 0;
    }

    if ((gameState->man.dx > 2.0f || gameState->man.dx < -2.0f) && gameState->man.onLedge)
    {
        if (gameState->time % 20 == 0) {
            if (gameState->man.currentSprite == 1 || gameState->man.currentSprite == 0) {
                gameState->man.currentSprite = 2;
            }
            else if (gameState->man.currentSprite == 2 || gameState->man.currentSprite == 0) {
                gameState->man.currentSprite = 1;
            }
        }
    }
    else 
    {
        gameState->man.currentSprite = 0;
    }

    return false;
}

void PreCollisionProcessing(GameState* gameState) {


    if (gameState->time > 120 && gameState->statusState == STATUS_STATE_LIVES) {
        ShutdownStatusLives(gameState);
        gameState->statusState = STATUS_STATE_GAME;
        Mix_PlayMusic(gameState->bgMusic, -1);
    }
    else if (gameState->statusState == STATUS_STATE_GAME ) {
        Man* man = &gameState->man;
        if (!gameState->man.isDead) {
            man->y += man->dy;
            man->x += man->dx;

            gameState->scrollX = -man->x + SCREEN_WIDTH / 2.20f;
            if (gameState->scrollX > 0)
                gameState->scrollX = 0;
        }
        man->dy += GRAVITY;
    }
    else if (gameState->time > 120 && gameState->statusState == STATUS_STATE_GAMEOVER) {
        ShutdownGameOver(gameState);
    }

    if (gameState->man.isDead && gameState->deathCountdown < 0) {
        gameState->deathCountdown = 120;
        Mix_PlayChannel(-1, gameState->deathMixChunk, 0);
        Mix_HaltMusic();
    }
    if (gameState->deathCountdown > 0) {
        gameState->deathCountdown--;
        if (gameState->deathCountdown <= 0) {
            gameState->man.lives--;
            if (gameState->man.lives > 0) {
                InitStatusLives(gameState);
                gameState->statusState = STATUS_STATE_LIVES;
                gameState->time = 0;
                gameState->man.isDead = false;
                gameState->man.x = 220.f;
                gameState->man.y = 70.f;
                gameState->man.dx = 0.f;
                gameState->man.dy = 0.f;
                gameState->man.onLedge = false;
                gameState->deathCountdown = -1;
            }
            else {
                gameState->statusState = STATUS_STATE_GAMEOVER;
                InitGameOver(gameState);
                gameState->time = 0;
            }
        }
    }
}
int Collide2D(float x1, float y1, float x2, float y2, float w1, float h1, float w2, float h2) {
    return (!((x1 > (x2 + w2)) || (x2 > (x1 + w1)) || (y1 > (y2 + h2)) || (y2 > (y1 + h1))));
}


void CollisionDetection(GameState* gameState) {

    for (int i = 0; i < ArrayLength(gameState->stars); i++) {
        if (Collide2D(gameState->man.x, gameState->man.y, gameState->stars[i].x, gameState->stars[i].y,
            gameState->man.w, gameState->man.h, gameState->stars[i].w, gameState->stars[i].h))
        {
            gameState->man.isDead = true;
            break;
        }
    }

    for (int i = 0; i < ArrayLength(gameState->ledges); i++) {
        float mw = 64.f, mh = 64.f;
        float mx = gameState->man.x, my = gameState->man.y;
        float bw = (float)gameState->ledges[i].w, bh = (float)gameState->ledges[i].h,
            bx = (float)gameState->ledges[i].x, by = (float)gameState->ledges[i].y;

        if (mx + mw > bx && mx < bx + bw) {
            //Bumping top of a brick.
            if (my < by && my + mh > by && gameState->man.dy > 0) {
                gameState->man.dy = 0.f;
                my = by - mh;
                gameState->man.y = my;
                if (gameState->man.onLedge == false) {
                    Mix_PlayChannel(-1, gameState->landMixChunk, 0);
                    gameState->man.onLedge = true;
                }
            }
            //Bumping bottom of a brick.
            else if (my < by + bh && my > by && gameState->man.dy < 0) {
                gameState->man.dy = 0.f;
                my = by + bh;
                gameState->man.y = my;
            }
        }

        if (my + mh > by && my < by + bh) {
            //Bumping right side of a brick.
            if (mx < bx + bw && mx + mw > bx + bw && gameState->man.dx < 0) {
                mx = bx + bw;
                gameState->man.x = mx;
                gameState->man.dx = 0;
            }
            //Bumping left side of a brick.
            else if (mx < bx && mx + mw > bx && gameState->man.dx > 0) {
                mx = bx - mw;
                gameState->man.x = mx;
                gameState->man.dx = 0;
            }
        }
    }
}

void RenderFrame(SDL_Renderer* renderer, GameState* gameState){
    if (gameState->statusState == STATUS_STATE_LIVES)
        DrawStatusLives(gameState);
    else if (gameState->statusState == STATUS_STATE_GAME) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

        SDL_Rect manRect = { (int)(gameState->scrollX + gameState->man.x), (int)gameState->man.y, 64, 64 };
        SDL_RenderCopyEx(renderer, gameState->manFrames[gameState->man.currentSprite], NULL, &manRect, 0, NULL, gameState->man.facingRight);

        for (int i = 0; i < ArrayLength(gameState->ledges); i++) {
            SDL_Rect ledgeRect = { (int)(gameState->scrollX + gameState->ledges[i].x), (int)gameState->ledges[i].y, (int)gameState->ledges[i].w, (int)gameState->ledges[i].h };
            SDL_RenderCopy(renderer, gameState->brickTexture, NULL, &ledgeRect);
        }

        for (int i = 0; i < sizeof(gameState->stars) / sizeof(Star); i++) {
            SDL_Rect newStar = {(int)(gameState->scrollX + gameState->stars[i].x), (int)gameState->stars[i].y, 64, 64};
            SDL_RenderCopy(renderer, gameState->starTexture, NULL, &newStar);
        }

        if (gameState->man.isDead) {
            SDL_Rect newFire = { (int)(gameState->scrollX + gameState->man.x + gameState->man.w/2 - 64/2), 
                (int)(gameState->man.y + gameState->man.h/2 - 64 / 2), 64, 64 };
            SDL_RenderCopyEx(renderer, gameState->fireTexture, NULL, &newFire, 0, NULL, (gameState->time % 20 < 10));
        }
    }

    SDL_RenderPresent(renderer);
}

int main()
{
    srand((unsigned int)(time(NULL)));
    GameState gameState;


    SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();

    gameState.window = SDL_CreateWindow("C Survivors", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    gameState.renderer = SDL_CreateRenderer(gameState.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

    LoadGame(&gameState);

    bool done = false;

    while (!done) {
        done = ProcessEvents(&gameState);
        PreCollisionProcessing(&gameState);
        CollisionDetection(&gameState);
        RenderFrame(gameState.renderer, &gameState);
    }

    FreeMemoryAndQuit(&gameState);

    return 0;
}
#endif
