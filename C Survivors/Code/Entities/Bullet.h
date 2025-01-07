#pragma once
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    float x, y, dx;
} Bullet;

typedef struct {
	Bullet** array;
	size_t used, size;
} BulletVector;

void InitBulletVector(BulletVector* vector, size_t initialSize);
void InsertBulletVector(BulletVector* vector, Bullet* element);
void RemoveBulletVector(BulletVector* vector, Bullet* element);
void FreeBulletVector(BulletVector* vector);
bool AddBulletToGame(BulletVector* bulletVector, float x, float y, float dx);
void RemoveBulletFromGame(BulletVector* bulletVector, int index);