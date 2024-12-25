#pragma once

#include "Managers/GameState.h"


void InitStatusLives(GameState* gameState);
void DrawStatusLives(GameState* gameState);
void ShutdownStatusLives(GameState* gameState);
void InitGameOver(GameState* gameState);
void DrawGameOver(GameState* gameState);
void ShutdownGameOver(GameState* gameState);
