#include "EnemyJarhead.h"
#include <stdio.h>

void InitEnemyJarheadVector(EnemyJarheadVector* enemyJarheadVector, size_t initialSize)
{
	enemyJarheadVector->array = calloc(initialSize, sizeof(EnemyJarhead*));

	if (enemyJarheadVector->array == NULL) {
		printf("Not enough space to allocate the enemyJarheadVector!!! Crash!!!");
		exit(1);
	}
	enemyJarheadVector->used = 0;
	enemyJarheadVector->size = initialSize;
}

void InsertEnemyJarheadVector(EnemyJarheadVector* enemyJarheadVector, EnemyJarhead* element)
{
	if (enemyJarheadVector->used == enemyJarheadVector->size) {
		enemyJarheadVector->size = (size_t)(enemyJarheadVector->size * 1.5f);
		EnemyJarhead** reallocatedArray = realloc(enemyJarheadVector->array, enemyJarheadVector->size * sizeof(EnemyJarhead*));
		if (reallocatedArray == NULL) {
			printf("Not enough space to reallocate the enemyJarheadVector!!! Crash!!!");
			exit(1);
		}
		enemyJarheadVector->array = reallocatedArray;
		for (int index = (int)enemyJarheadVector->used; index < enemyJarheadVector->size; index++)
			enemyJarheadVector->array[index] = NULL;
	}
	enemyJarheadVector->array[enemyJarheadVector->used++] = element;
}

void Internal_ShrinkEnemyJarheadVector(EnemyJarheadVector* enemyJarheadVector, int currentIndex) {
	if (currentIndex + 1 < enemyJarheadVector->used) {
		enemyJarheadVector->array[currentIndex] = enemyJarheadVector->array[currentIndex + 1];
		Internal_ShrinkEnemyJarheadVector(enemyJarheadVector, currentIndex + 1);
	}
}

//Remove a bullet object from the enemyJarheadVector. Frees the memory of the object.
void RemoveEnemyJarheadVector(EnemyJarheadVector* enemyJarheadVector, EnemyJarhead* element)
{
	for (int i = 0; i < enemyJarheadVector->used; i++) {
		if (enemyJarheadVector->array[i] == element) {
			free(enemyJarheadVector->array[i]);
			enemyJarheadVector->array[i] = NULL;
			Internal_ShrinkEnemyJarheadVector(enemyJarheadVector, i);
			enemyJarheadVector->used--;
			break;
		}
	}
}

void FreeEnemyJarheadVector(EnemyJarheadVector* enemyJarheadVector)
{
	for (int index = 0; index < enemyJarheadVector->used; index++)
		free(enemyJarheadVector->array[index]);
	free(enemyJarheadVector->array);
	enemyJarheadVector->array = NULL;
	enemyJarheadVector->used = enemyJarheadVector->size = 0;
	free(enemyJarheadVector);
}

bool AddEnemyJarheadToGame(EnemyJarheadVector* enemyJarheadVector, float x, float y, float dx, float dy, int currentSprite, int w, int h, bool facingLeft, bool isDead, bool toGarbageCollect)
{
	EnemyJarhead* newEnemyJarhead = malloc(sizeof(EnemyJarhead));
	if (newEnemyJarhead != NULL) {
		newEnemyJarhead->x = x;
		newEnemyJarhead->y = y;
		newEnemyJarhead->dx = dx;
		newEnemyJarhead->dy = dy;
		newEnemyJarhead->w = w;
		newEnemyJarhead->h = h;
		newEnemyJarhead->isDead = false;
		newEnemyJarhead->toGarbageCollect = false;
		newEnemyJarhead->currentSprite = currentSprite;
		newEnemyJarhead->facingLeft = facingLeft;
		InsertEnemyJarheadVector(enemyJarheadVector, newEnemyJarhead);
		return true;
	}
	else {
		printf("Not enough space for a new enemy jarhead. Crash!!!");
		return false;
	}
}

void RemoveEnemyJarheadFromGame(EnemyJarheadVector* enemyJarheadVector, int index)
{
	if (enemyJarheadVector->array[index] != NULL)
		RemoveEnemyJarheadVector(enemyJarheadVector, enemyJarheadVector->array[index]);
}