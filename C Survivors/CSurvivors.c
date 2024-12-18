#ifndef CSURVIVORS_C
#define CSURVIVORS_C

#include "CSurvivors.h"
#include "Status.h"
#undef main

void LoadGame(GameState* gameState) {
    gameState->font = TTF_OpenFont("../Open-Pixel.ttf", 48);
    if (gameState->font == NULL) {
        printf("Image not found!!!\n");
        SDL_Quit();
        exit(1);
    }

    gameState->man.x = 220.f;
    gameState->man.y = 70.f;
    gameState->man.dy = 0.f;
    gameState->man.dx = 0.f;
    gameState->man.animFrame = 0;
    gameState->man.facingRight = true;
    gameState->statusState = STATUS_STATE_LIVES;

    InitStatusLives(gameState);

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

    gameState->time = 0;

    gameState->ledges[99].h = 64;
    gameState->ledges[99].w = 64;
    gameState->ledges[99].x = 350;
    gameState->ledges[99].y = 200;
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
            if (gameState->man.animFrame == 1 || gameState->man.animFrame == 0) {
                gameState->man.animFrame = 2;
            }
            else if (gameState->man.animFrame == 2 || gameState->man.animFrame == 0) {
                gameState->man.animFrame = 1;
            }
        }
    }
    else 
    {
        gameState->man.animFrame = 0;
    }

    return false;
}

void ProcessMiscellaneous(GameState* gameState) {

}

void PreCollisionProcessing(GameState* gameState) {

    if (gameState->man.dy != 0)
        gameState->man.onLedge = false;
    else
        gameState->man.onLedge = true;

    if (gameState->time > 120) {
        gameState->statusState = STATUS_STATE_GAME;
        ShutdownStatusLives(gameState);
    }

    Man* man = &gameState->man;
    man->y += man->dy;
    man->x += man->dx;

    man->dy += GRAVITY;
}

void CollisionDetection(GameState* gameState) {
    //TODO: better collision detection.
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
                gameState->man.onLedge = true;
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
    if (STATUS_STATE_LIVES)
        DrawStatusLives(gameState);
    else if (STATUS_STATE_GAME) {


        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

        SDL_Rect manRect = { (int)gameState->man.x, (int)gameState->man.y, 64, 64 };
        SDL_RenderCopyEx(renderer, gameState->manFrames[gameState->man.animFrame], NULL, &manRect, 0, NULL, gameState->man.facingRight);

        for (int i = 0; i < ArrayLength(gameState->ledges); i++) {
            SDL_Rect ledgeRect = { gameState->ledges[i].x, gameState->ledges[i].y, gameState->ledges[i].w, gameState->ledges[i].h };
            SDL_RenderCopy(renderer, gameState->brickTexture, NULL, &ledgeRect);
        }

        /*for (int i = 0; i < sizeof(gameState->stars) / sizeof(Star); i++) {
            SDL_Rect newStar = {gameState->stars[i].x, gameState->stars[i].y, 64, 64};
            SDL_RenderCopy(renderer, gameState->starTexture, NULL, &newStar);
        }*/
    }

    SDL_RenderPresent(renderer);
}

int main()
{
    srand((unsigned int)(time(NULL)));
    GameState gameState;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    SDL_InitSubSystem(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow("C Survivors", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    gameState.renderer = renderer;

    LoadGame(&gameState);

    bool done = false;

    while (!done) {
        done = ProcessEvents(&gameState);
        ProcessMiscellaneous(&gameState);
        PreCollisionProcessing(&gameState);
        CollisionDetection(&gameState);
        RenderFrame(renderer, &gameState);
    }


    SDL_DestroyTexture(gameState.starTexture);
    SDL_DestroyTexture(gameState.brickTexture);
    for (short Index = ArrayLength(gameState.manFrames) - 1; Index >= 0 ; Index--)
        SDL_DestroyTexture(gameState.manFrames[Index]);
    TTF_CloseFont(gameState.font);
    if(gameState.label != NULL)
        SDL_DestroyTexture(gameState.label);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    TTF_Quit();
    SDL_Quit();

    return 0;
}
#endif
