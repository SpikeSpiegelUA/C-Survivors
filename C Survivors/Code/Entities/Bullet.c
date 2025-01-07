#include "Bullet.h"
#include <stdio.h>

void InitBulletVector(BulletVector* vector, size_t initialSize)
{
	vector->array = calloc(initialSize, sizeof(Bullet*));

	if (vector->array == NULL) {
		printf("Not enough space to allocate bullet vector!!! Crash!!!");
		exit(1);
	}
	vector->used = 0;
	vector->size = initialSize;
}

void InsertBulletVector(BulletVector* vector, Bullet* element)
{
	if (vector->used == vector->size) {
		vector->size = (size_t)(vector->size * 1.5f);
		Bullet** reallocatedArray = realloc(vector->array, vector->size * sizeof(Bullet*));
		if (reallocatedArray == NULL) {
			printf("Not enough space to reallocate bullet vector!!! Crash!!!");
			exit(1);
		}
		vector->array = reallocatedArray;
		for (int index = (int)vector->used; index < vector->size; index++)
			vector->array[index] = NULL;
	}
 	vector->array[vector->used++] = element;
}

void Internal_ShrinkVector(BulletVector* vector, int currentIndex) {
	if (currentIndex + 1 < vector->used) {
		vector->array[currentIndex] = vector->array[currentIndex + 1];
		Internal_ShrinkVector(vector, currentIndex + 1);
	}
}

//Remove a bullet object from the vector. Frees the memory of the object.
void RemoveBulletVector(BulletVector* vector, Bullet* element)
{
	for (int i = 0; i < vector->used; i++) {
		if (vector->array[i] == element) {
			free(vector->array[i]);
			Internal_ShrinkVector(vector, i);
			vector->used--;
			break;
		}
	}
}

void FreeBulletVector(BulletVector* vector)
{
	for (int index = 0; index < vector->used; index++)
		free(vector->array[index]);
	free(vector->array);
	vector->array = NULL;
	vector->used = vector->size = 0;
}

bool AddBulletToGame(BulletVector* bulletVector, float x, float y, float dx)
{
	Bullet* newBullet = malloc(sizeof(Bullet));
	if (newBullet != NULL) {
		newBullet->x = x;
		newBullet->y = y;
		newBullet->dx = dx;
		InsertBulletVector(bulletVector, newBullet);
		return true;
	}
	else {
		printf("Not enough space for a new bullet. Crash!!!");
		return false;
	}
}

void RemoveBulletFromGame(BulletVector* bulletVector, int index)
{
	if (bulletVector->array[index] != NULL)
		RemoveBulletVector(bulletVector, bulletVector->array[index]);
}
