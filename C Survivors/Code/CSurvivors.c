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
#include "Utilities/SATCollision.h"

#undef main

void FreeMemoryAndQuit(GameState* gameState) {
    if (gameState->starTexture != NULL)
        SDL_DestroyTexture(gameState->starTexture);
    if (gameState->brickTexture != NULL)
        SDL_DestroyTexture(gameState->brickTexture);
    if (gameState->bulletTexture != NULL)
        SDL_DestroyTexture(gameState->bulletTexture);
    if (gameState->manTexture != NULL)
        SDL_DestroyTexture(gameState->manTexture);
    if (gameState->backgroundTexture != NULL)
        SDL_DestroyTexture(gameState->backgroundTexture);
    if (gameState->fireTexture != NULL)
        SDL_DestroyTexture(gameState->fireTexture);
    if (gameState->label != NULL)
        SDL_DestroyTexture(gameState->label);
    if (gameState->enemyJarheadTexture != NULL)
        SDL_DestroyTexture(gameState->enemyJarheadTexture);
    if (gameState->bloodParticleTexture != NULL)
        SDL_DestroyTexture(gameState->bloodParticleTexture);
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
    if (gameState->shotMixChunk != NULL)
        Mix_FreeChunk(gameState->shotMixChunk);
    if (gameState->enemyDeathMixChunk != NULL)
        Mix_FreeChunk(gameState->enemyDeathMixChunk);

    if (gameState->window != NULL)
        SDL_DestroyWindow(gameState->window);
    if (gameState->renderer != NULL)
        SDL_DestroyRenderer(gameState->renderer);
    
    if (gameState->bulletVector != NULL)
        FreeBulletVector(gameState->bulletVector);
    if (gameState->enemyJarheadVector != NULL)
        FreeEnemyJarheadVector(gameState->enemyJarheadVector);
    if (gameState->particleVector != NULL)
        FreeParticleVector(gameState->particleVector);

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
    SDL_Surface* manSpriteSheetSurface = NULL;
    SDL_Surface* brickSurface = NULL;
    SDL_Surface* fireSurface = NULL;
    SDL_Surface* bulletSurface = NULL;
    SDL_Surface* backgroundSurface = NULL;
    SDL_Surface* enemyJarheadSurface = NULL;
    SDL_Surface* bloodParticleSurface = NULL;

    starSurface = IMG_Load("..\\Images\\star.png");
    if (starSurface == NULL) {
        printf("Star image not found!!!\n");
        error = true;
    }

    brickSurface = IMG_Load("..\\Images\\brick.png");
    if (brickSurface == NULL) {
        printf("Image not found!!!\n");
        error = true;
    }

    backgroundSurface = IMG_Load("..\\Images\\background.png");
    if (backgroundSurface == NULL) {
        printf("Image not found!!!\n");
        error = true;
    }

    manSpriteSheetSurface = IMG_Load("..\\Images\\Sheets\\mansheet.png");
    if (manSpriteSheetSurface == NULL) {
        printf("Image not found!!!\n");
        error = true;
    }
    
    fireSurface = IMG_Load("..\\Images\\fire.png");
    if (fireSurface == NULL) {
        printf("Image not found!!!\n");
        error = true;
    }

    bulletSurface = IMG_Load("..\\Images\\bullet.png");
    if (bulletSurface == NULL) {
        printf("Image not found!!!\n");
        error = true;
    }

    bloodParticleSurface = IMG_Load("..\\Images\\blood.png");
    if (bloodParticleSurface == NULL) {
        printf("Image not found!!!\n");
        error = true;
    }

    enemyJarheadSurface = IMG_Load("..\\Images\\Sheets\\enemyjarheadsheet.png");
    if (enemyJarheadSurface == NULL) {
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
    gameState->shotMixChunk = Mix_LoadWAV("..\\Audio\\ShotSound.wav");
    gameState->enemyDeathMixChunk = Mix_LoadWAV("..\\Audio\\EnemyDeathSound.wav");
    if (gameState->deathMixChunk == NULL || gameState->jumpMixChunk == NULL || gameState->landMixChunk == NULL || gameState->bgMusic == NULL
        || gameState->shotMixChunk == NULL || gameState->enemyDeathMixChunk == NULL){
        printf("Sound not found!!!\n");
        error = true;
    }

    //Initialize objects and create textures.
    gameState->man.x = 220.f;
    gameState->man.y = 70.f;
    gameState->man.w = 110;
    gameState->man.h = 120;
    gameState->man.dy = 0.f;
    gameState->man.dx = 0.f;
    gameState->man.currentSprite = 5;
    gameState->man.facingLeft = false;
    gameState->man.lives = 3;
    gameState->man.isDead = false;
    gameState->man.onLedge = false;
    gameState->man.isShooting = false;
    gameState->time = 0;
    gameState->scrollX = 0;
    gameState->deathCountdown = -1;
    gameState->statusState = STATUS_STATE_LIVES;

    gameState->starTexture = SDL_CreateTextureFromSurface(gameState->renderer, starSurface);
    gameState->brickTexture = SDL_CreateTextureFromSurface(gameState->renderer, brickSurface);
    gameState->fireTexture = SDL_CreateTextureFromSurface(gameState->renderer, fireSurface);
    gameState->manTexture = SDL_CreateTextureFromSurface(gameState->renderer, manSpriteSheetSurface);
    gameState->bulletTexture = SDL_CreateTextureFromSurface(gameState->renderer, bulletSurface);
    gameState->backgroundTexture = SDL_CreateTextureFromSurface(gameState->renderer, backgroundSurface);
    gameState->enemyJarheadTexture = SDL_CreateTextureFromSurface(gameState->renderer, enemyJarheadSurface);
    gameState->bloodParticleTexture = SDL_CreateTextureFromSurface(gameState->renderer, bloodParticleSurface);

    //Create stars.
    /*for (int i = 0; i < sizeof(gameState->stars) / sizeof(Star); i++) {
        gameState->stars[i].x = (float)(rand()%640 * i + SCREEN_WIDTH);
        gameState->stars[i].y = (float)(rand()%480);
        gameState->stars[i].w = (float)starSurface->w;
        gameState->stars[i].h = (float)starSurface->h;
    }*/

    //Create backgrounds.
    for (int i = 0; i < sizeof(gameState->backgrounds) / sizeof(Background); i++) {
        gameState->backgrounds[i].x = (float)(640 * i);
        gameState->backgrounds[i].y = 0;
        gameState->backgrounds[i].w = 640.f;
        gameState->backgrounds[i].h = 480.f;
    }

    gameState->ledges[0].h = 64;
    gameState->ledges[0].w = 64;
    gameState->ledges[0].x = 350;
    gameState->ledges[0].y = 200;

    //Init UI.

    InitStatusLives(gameState);

    //Init vectors.
    gameState->bulletVector = malloc(sizeof(BulletVector));
    InitBulletVector(gameState->bulletVector, 10);
    gameState->enemyJarheadVector = malloc(sizeof(EnemyJarheadVector));
    InitEnemyJarheadVector(gameState->enemyJarheadVector, 10);
    gameState->particleVector = malloc(sizeof(ParticleVector));
    InitParticleVector(gameState->particleVector, 10);

    AddEnemyJarheadToGame(gameState->enemyJarheadVector, 1000.f, 300.f, 0.f, 0.f, 4, 110, 120, true, false, false);

    //Free all the dynamically allocated memory.

    SDL_FreeSurface(brickSurface);
    SDL_FreeSurface(manSpriteSheetSurface);
    SDL_FreeSurface(starSurface);
    SDL_FreeSurface(fireSurface);
    SDL_FreeSurface(bulletSurface);
    SDL_FreeSurface(backgroundSurface);
    SDL_FreeSurface(enemyJarheadSurface);
    SDL_FreeSurface(bloodParticleSurface);

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
                break;
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

    if (gameState->man.dx == 0 && gameState->man.onLedge) {
        if (state[SDL_SCANCODE_SPACE]) {
            gameState->man.isShooting = true;
            if (gameState->time % 12 == 0) {
                if (gameState->man.currentSprite == 4)
                    gameState->man.currentSprite = 5;
                else
                    gameState->man.currentSprite = 4;
                if (gameState->man.facingLeft)
                    AddBulletToGame(gameState->bulletVector, gameState->man.x - 30, gameState->man.y + gameState->man.h / 2 - 20, 8, 8, -10);
                else
                    AddBulletToGame(gameState->bulletVector, gameState->man.x + gameState->man.w, gameState->man.y + gameState->man.h / 2 - 20, 8, 8, 10);
                Mix_PlayChannel(-1, gameState->shotMixChunk, 0);
            }
        }
        else {
            gameState->man.isShooting = false;
            gameState->man.currentSprite = 4;
        }
    }
    
    //Walking
    if (state[SDL_SCANCODE_RIGHT]) {
        gameState->man.dx += 0.5f;
        if (gameState->man.dx > 6.f)
            gameState->man.dx = 6.f;
        gameState->man.facingLeft = false;
        gameState->man.isShooting = false;
    }
    else if (state[SDL_SCANCODE_LEFT]) {
        gameState->man.dx -= 0.5f;
        if (gameState->man.dx < -6.f)
            gameState->man.dx = -6.f;
        gameState->man.facingLeft = true;
        gameState->man.isShooting = false;
    }
    else {
        gameState->man.dx *= 0.8f;
        if (fabsf(gameState->man.dx) < 0.1f && fabsf(gameState->man.dx) > -0.1f)
            gameState->man.dx = 0;
    }

    if(!gameState->man.isShooting)
        if ((gameState->man.dx > 2.0f || gameState->man.dx < -2.0f) && gameState->man.onLedge)
        {
            if (gameState->time % 6 == 0) {
                gameState->man.currentSprite++;
                gameState->man.currentSprite %= 4;
            }
        }
        else 
        {
            gameState->man.currentSprite = 4;
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

            gameState->scrollX = -man->x + SCREEN_WIDTH / 3.f;
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

    for (int index = 0; index < gameState->bulletVector->used; index++) {
        if (gameState->bulletVector->array[index] != NULL) {
               gameState->bulletVector->array[index]->x += gameState->bulletVector->array[index]->dx;
            if (gameState->bulletVector->array[index]->x > SCREEN_RIGHT_MAX || gameState->bulletVector->array[index]->x < SCREEN_LEFT_MAX)
                RemoveBulletFromGame(gameState->bulletVector, index);
        }
    }

    //Process particles.
    for (int i = 0; i < gameState->particleVector->used; i++) {
        gameState->particleVector->array[i]->x += gameState->particleVector->array[i]->dx;
        gameState->particleVector->array[i]->y += gameState->particleVector->array[i]->dy;
        gameState->particleVector->array[i]->dy += GRAVITY;
        gameState->particleVector->array[i]->life--;
        if (gameState->particleVector->array[i]->life <= 0)
            RemoveParticleFromGame(gameState->particleVector, i);
    }
}


void AIProcessing(GameState* gameState) {
    for (int i = 0; i < gameState->enemyJarheadVector->used; i++) {
        EnemyJarhead* enemyJarhead = gameState->enemyJarheadVector->array[i];
        if (enemyJarhead->x + gameState->scrollX <= gameState->man.x + gameState->scrollX + 400 && enemyJarhead->x + gameState->scrollX >= gameState->man.x + gameState->scrollX - 400
            && !enemyJarhead->isDead) {
            if (enemyJarhead->x + gameState->scrollX <= gameState->man.x + gameState->scrollX) {
                enemyJarhead->x += 2.f;
                enemyJarhead->facingLeft = false;
            }
            else if (enemyJarhead->x + gameState->scrollX >= gameState->man.x + gameState->scrollX) {
                enemyJarhead->x -= 2.f;
                enemyJarhead->facingLeft = true;
            }
            if (gameState->time % 6 == 0 ) {
                enemyJarhead->currentSprite++;
                enemyJarhead->currentSprite %= 4;
            }
            if (gameState->time % 60 == 0) {
                if (enemyJarhead->facingLeft)
                    AddBulletToGame(gameState->bulletVector, enemyJarhead->x - 30, enemyJarhead->y + enemyJarhead->h / 2 - 20, 8, 8, -4.f);
                else
                    AddBulletToGame(gameState->bulletVector, enemyJarhead->x + enemyJarhead->w, enemyJarhead->y + enemyJarhead->h / 2 - 20, 8, 8, 4.f);
            }
        }
    }
}

int Collide2D(float x1, float y1, float x2, float y2, float w1, float h1, float w2, float h2) {
    return (!((x1 > (x2 + w2)) || (x2 > (x1 + w1)) || (y1 > (y2 + h2)) || (y2 > (y1 + h1))));
}

void CollisionDetection(GameState* gameState) {

    //Stars and the player collision.
    for (int i = 0; i < ArrayLength(gameState->stars); i++) {
        if (Collide2D(gameState->man.x, gameState->man.y, gameState->stars[i].x, gameState->stars[i].y,
            gameState->man.w, gameState->man.h, gameState->stars[i].w, gameState->stars[i].h))
        {
            gameState->man.isDead = true;
            break;
        }
    }

    //Bullets and enemies/player collision.
    for (int i = 0; i < gameState->bulletVector->used; i++) 
        for (int j = 0; j < gameState->enemyJarheadVector->used; j++) {
             if (Collide2D(gameState->bulletVector->array[i]->x, gameState->bulletVector->array[i]->y,
                gameState->enemyJarheadVector->array[j]->x, gameState->enemyJarheadVector->array[j]->y,
                (float) gameState->bulletVector->array[i]->w, (float) gameState->bulletVector->array[i]->h,
                (float) gameState->enemyJarheadVector->array[j]->w, (float) gameState->enemyJarheadVector->array[j]->h))
            {

                if (!gameState->enemyJarheadVector->array[j]->isDead) {
                    gameState->enemyJarheadVector->array[j]->isDead = true;
                    AddParticleToGame(gameState->particleVector, gameState->enemyJarheadVector->array[j]->x + gameState->enemyJarheadVector->array[j]->w/2,
                        gameState->enemyJarheadVector->array[j]->y + gameState->enemyJarheadVector->array[j]->h / 2, 5.f, 50);
                }
                break;
            }
        }

    Vector2DVector playerVertices;
    Vector2DVector ledgeVertices;
    InitVector2DVector(&playerVertices, 8);
    InitVector2DVector(&ledgeVertices, 8);
    for (int i = 0; i < ArrayLength(gameState->ledges); i++) {
        Vector2D playerVertice1;
        Vector2D playerVertice2;
        Vector2D playerVertice3;
        Vector2D playerVertice4;
        Vector2D ledgeVertice1;
        Vector2D ledgeVertice2;
        Vector2D ledgeVertice3;
        Vector2D ledgeVertice4;
        playerVertice1.x = gameState->man.x;
        playerVertice1.y = gameState->man.y;
        InsertVector2DVector(&playerVertices, &playerVertice1);
        playerVertice2.x = gameState->man.x + gameState->man.w;
        playerVertice2.y = gameState->man.y;
        InsertVector2DVector(&playerVertices, &playerVertice2);
        playerVertice3.x = gameState->man.x + gameState->man.w;
        playerVertice3.y = gameState->man.y + gameState->man.h;
        InsertVector2DVector(&playerVertices, &playerVertice3);
        playerVertice4.x = gameState->man.x;
        playerVertice4.y = gameState->man.y + gameState->man.h;
        InsertVector2DVector(&playerVertices, &playerVertice4);
        ledgeVertice1.x = gameState->ledges[i].x;
        ledgeVertice1.y = gameState->ledges[i].y;
        InsertVector2DVector(&ledgeVertices, &ledgeVertice1);
        ledgeVertice2.x = gameState->ledges[i].x + gameState->ledges[i].w;
        ledgeVertice2.y = gameState->ledges[i].y;
        InsertVector2DVector(&ledgeVertices, &ledgeVertice2);
        ledgeVertice3.x = gameState->ledges[i].x + gameState->ledges[i].w;
        ledgeVertice3.y = gameState->ledges[i].y + gameState->ledges[i].h;
        InsertVector2DVector(&ledgeVertices, &ledgeVertice3);
        ledgeVertice4.x = gameState->ledges[i].x;
        ledgeVertice4.y = gameState->ledges[i].y + gameState->ledges[i].h;
        InsertVector2DVector(&ledgeVertices, &ledgeVertice4);
        CheckCollision(&playerVertices, &ledgeVertices);
    }

    //Player and ledges collision.
    /*for (int i = 0; i < ArrayLength(gameState->ledges); i++) {
        float mw = gameState->man.w, mh = gameState->man.h;
        float mx = gameState->man.x, my = gameState->man.y;
        float bw = (float)gameState->ledges[i].w, bh = (float)gameState->ledges[i].h,
            bx = (float)gameState->ledges[i].x, by = (float)gameState->ledges[i].y;

        if (mx + mw > bx && mx < bx + bw) {
            //Bumping top of a brick.
            if (my < by && my + mh > by && my + mh < by + bh && gameState->man.dy > 0) {
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
            if (mx < bx + bw && mx + mw > bx + bw && mx > bx + bw/2 && gameState->man.dx < 0) {
                if (fabs(mx - bx + bw) > bw / 4) {
                    mx = bx + bw;
                    gameState->man.x = mx;
                    gameState->man.dx = 0;
                }
                else {
                    mx = 1;
                    gameState->man.x = mx;
                    gameState->man.dx = 0;
                }
            }
            //Bumping left side of a brick.
            else if (mx < bx && mx + mw > bx && mx < bx + bw / 2 && gameState->man.dx > 0) {
                if (fabs(mx - bx) > bw / 4) {
                    mx = bx - mw;
                    gameState->man.x = mx;
                    gameState->man.dx = 0;
                }
                else {
                    mx = 1;
                    gameState->man.x = mx;
                    gameState->man.dx = 0;
                }
            }
        }
    }*/

    //Prevent player to fall through the ground.
    if (gameState->man.y > 300) {
        gameState->man.dy = 0.f;
        gameState->man.y = 300;
        if (gameState->man.onLedge == false) {
            Mix_PlayChannel(-1, gameState->landMixChunk, 0);
            gameState->man.onLedge = true;
        }
    }
}

void PostCollisionProcessing(GameState* gameState) {
    //Process death of an enemy.
    for (int i = 0; i < gameState->enemyJarheadVector->used; i++) {
        if (gameState->enemyJarheadVector->array[i]->isDead && gameState->time % 6 == 0) {
            if (gameState->enemyJarheadVector->array[i]->toGarbageCollect) {
                RemoveEnemyJarheadFromGame(gameState->enemyJarheadVector, i);
            }
            else {
                if (gameState->enemyJarheadVector->array[i]->currentSprite < 6)
                    gameState->enemyJarheadVector->array[i]->currentSprite = 6;
                else if (gameState->enemyJarheadVector->array[i]->currentSprite >= 6) {
                    gameState->enemyJarheadVector->array[i]->currentSprite++;
                    if (gameState->enemyJarheadVector->array[i]->currentSprite >= 7) {
                        gameState->enemyJarheadVector->array[i]->toGarbageCollect = true;
                        gameState->enemyJarheadVector->array[i]->currentSprite = 7;
                    }
                }
            }
        }
    }
}

void RenderFrame(SDL_Renderer* renderer, GameState* gameState){
    //Draw the start screen at the beginning of each round.
    if (gameState->statusState == STATUS_STATE_LIVES)
        DrawStatusLives(gameState);
    else if (gameState->statusState == STATUS_STATE_GAME) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        //Draw the background.
        for (int i = 0; i < sizeof(gameState->backgrounds) / sizeof(Background); i++) {
            SDL_Rect backgroundRect = { (int)(gameState->scrollX + gameState->backgrounds[i].x), (int)gameState->backgrounds[i].y,
                (int)gameState->backgrounds[i].w, (int)gameState->backgrounds[i].h };
            SDL_RenderCopy(renderer, gameState->backgroundTexture, NULL, &backgroundRect);
        }

        //Draw the player.
        SDL_Rect srcManRect = { gameState->man.currentSprite * 40, 0, 40, 50 };
        SDL_Rect destManRect = { (int)(gameState->scrollX + gameState->man.x), (int)gameState->man.y, 110, 120 };
        SDL_RenderCopyEx(renderer, gameState->manTexture, &srcManRect, &destManRect, 0, NULL, gameState->man.facingLeft);

        //Draw ledges.
        for (int i = 0; i < ArrayLength(gameState->ledges); i++) {
            SDL_Rect ledgeRect = { (int)(gameState->scrollX + gameState->ledges[i].x), (int)gameState->ledges[i].y, (int)gameState->ledges[i].w, (int)gameState->ledges[i].h };
            SDL_RenderCopy(renderer, gameState->brickTexture, NULL, &ledgeRect);
        }

        //Draw stars.
        for (int i = 0; i < ArrayLength(gameState->stars); i++) {
            SDL_Rect starRect = {(int)(gameState->scrollX + gameState->stars[i].x), (int)gameState->stars[i].y, 64, 64};
            SDL_RenderCopy(renderer, gameState->starTexture, NULL, &starRect);
        }

        //Draw bullets.
        for (int i = 0; i < gameState->bulletVector->used; i++) {
            if (gameState->bulletVector->array[i] != NULL) {
                SDL_Rect newBullet = { (int)(gameState->scrollX + gameState->bulletVector->array[i]->x), (int)gameState->bulletVector->array[i]->y, 32, 32 };
                SDL_RenderCopy(renderer, gameState->bulletTexture, NULL, &newBullet);
            }
        }

        //Draw all the enemy jarheads.
        for (int i = 0; i < gameState->enemyJarheadVector->used; i++) {
            if (gameState->enemyJarheadVector->array[i] != NULL) {
                SDL_Rect newEnemyJarheadSourceRect = { 40 * gameState->enemyJarheadVector->array[i]->currentSprite, 0, 40, 50 };
                SDL_Rect newEnemyJarheadRect = { (int)(gameState->scrollX + gameState->enemyJarheadVector->array[i]->x), (int)gameState->enemyJarheadVector->array[i]->y, gameState->enemyJarheadVector->array[i]->w, gameState->enemyJarheadVector->array[i]->h };
                SDL_RenderCopyEx(renderer, gameState->enemyJarheadTexture, &newEnemyJarheadSourceRect, &newEnemyJarheadRect, 0, NULL, gameState->enemyJarheadVector->array[i]->facingLeft);
            }
        }

        //Draw particles.
        for (int i = 0; i < gameState->particleVector->used; i++) {
            if (gameState->particleVector->array[i] != NULL) {
                SDL_Rect newParticleRect = { (int)(gameState->scrollX + gameState->particleVector->array[i]->x), (int)gameState->particleVector->array[i]->y, 8, 8 };
                SDL_RenderCopy(renderer, gameState->bloodParticleTexture, NULL, &newParticleRect);
            }
        }

        //Draw the player's death fire.
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
        AIProcessing(&gameState);
        CollisionDetection(&gameState);
        PostCollisionProcessing(&gameState);
        RenderFrame(gameState.renderer, &gameState);
    }

    FreeMemoryAndQuit(&gameState);

    return 0;
}
#endif
