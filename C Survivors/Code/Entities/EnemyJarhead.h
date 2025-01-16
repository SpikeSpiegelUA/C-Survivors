#pragma once
#include "stdlib.h"
#include <stdbool.h>

typedef struct{
	float x, y, dx, dy;
	int w, h, currentSprite;
	bool facingLeft, isDead, toGarbageCollect;
} EnemyJarhead;

typedef struct {
	EnemyJarhead** array;
	size_t used, size;
} EnemyJarheadVector;

void InitEnemyJarheadVector(EnemyJarheadVector* enemyJarheadVector, size_t initialSize);
void InsertEnemyJarheadVector(EnemyJarheadVector* enemyJarheadVector, EnemyJarhead* element);
void RemoveEnemyJarheadVector(EnemyJarheadVector* enemyJarheadVector, EnemyJarhead* element);
void FreeEnemyJarheadVector(EnemyJarheadVector* enemyJarheadVector);
bool AddEnemyJarheadToGame(EnemyJarheadVector* enemyJarheadVector, float x, float y, float dx, float dy, int currentSprite, int w, int h, bool facingLeft, bool isDead, bool toGarbageCollect);
void RemoveEnemyJarheadFromGame(EnemyJarheadVector* enemyJarheadVector, int index);