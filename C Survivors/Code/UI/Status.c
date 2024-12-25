#pragma once

#include "Status.h"
#include "SDL.h"
#include <stdio.h>

void InitStatusLives(GameState* gameState)
{
	char str[128];

	sprintf_s(str, sizeof(str), "x %i \0", (int)gameState->man.lives);
	SDL_Color white = { 255, 255, 255, 255 };
	gameState->fontSurface = TTF_RenderText_Blended(gameState->font, str, white);
	gameState->label = SDL_CreateTextureFromSurface(gameState->renderer, gameState->fontSurface);
}

void DrawStatusLives(GameState* gameState)
{
	SDL_SetRenderDrawColor(gameState->renderer, 0, 0, 0, 255);

	SDL_RenderClear(gameState->renderer);

	SDL_Rect manRect = { 320-80, 240 - 24, 64, 64 };
	SDL_RenderCopyEx(gameState->renderer, gameState->manFrames[0], NULL, &manRect, 0, NULL, gameState->man.facingRight);

	SDL_SetRenderDrawColor(gameState->renderer, 255, 255, 255, 255);

	SDL_Rect textRect = { 320, 240 - gameState->fontSurface->h/2, gameState->fontSurface->w, gameState->fontSurface->h};
	SDL_RenderCopy(gameState->renderer, gameState->label, NULL, &textRect);
}

void ShutdownStatusLives(GameState* gameState)
{
	SDL_FreeSurface(gameState->fontSurface);
	SDL_DestroyTexture(gameState->label);
	gameState->label = NULL;
}

void InitGameOver(GameState* gameState)
{
	char str[16] = "Game Over!";
	SDL_Color white = { 255, 255, 255, 255 };
	gameState->fontSurface = TTF_RenderText_Blended(gameState->font, str, white);
	gameState->label = SDL_CreateTextureFromSurface(gameState->renderer, gameState->fontSurface);
	DrawGameOver(gameState);
}

void DrawGameOver(GameState* gameState)
{
	SDL_SetRenderDrawColor(gameState->renderer, 0, 0, 0, 255);

	SDL_RenderClear(gameState->renderer);

	SDL_SetRenderDrawColor(gameState->renderer, 255, 255, 255, 255);

	SDL_Rect textRect = { 320 - gameState->fontSurface->w / 2, 240 - gameState->fontSurface->h / 2, gameState->fontSurface->w, gameState->fontSurface->h };
	SDL_RenderCopy(gameState->renderer, gameState->label, NULL, &textRect);
}

void ShutdownGameOver(GameState* gameState)
{
	SDL_FreeSurface(gameState->fontSurface);
	SDL_DestroyTexture(gameState->label);
	gameState->label = NULL;
	SDL_Quit();
	exit(1);
}
