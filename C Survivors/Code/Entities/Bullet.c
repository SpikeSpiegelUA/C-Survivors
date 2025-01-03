#include "Bullet.h"
#include <stdio.h>

void InitBulletVector(BulletVector* vector, size_t initialSize)
{
	vector->array = malloc(initialSize * sizeof(Bullet));
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
		Bullet* reallocatedArray = realloc(vector->array, vector->size * sizeof(Bullet));
		if (reallocatedArray == NULL) {
			printf("Not enough space to reallocate bullet vector!!! Crash!!!");
			exit(1);
		}
		vector->array = reallocatedArray;
	}
	vector->array[vector->used++] = *element;
}

void Internal_ShrinkVector(BulletVector* vector, int currentIndex) {
	if (currentIndex + 1 < vector->used) {
		vector->array[currentIndex] = vector->array[currentIndex + 1];
		Internal_ShrinkVector(vector, currentIndex + 1);
	}
}

void RemoveBulletVector(BulletVector* vector, Bullet* element)
{
	for (int i = 0; i < vector->used; i++) {
		if (&vector->array[i] == element) {
			Internal_ShrinkVector(vector, i);
			vector->used--;
			break;
		}
	}
	if (vector->used < vector->size / 1.5f) {
		vector->size = (size_t)(vector->size / 1.5f);
		Bullet* reallocatedArray = realloc(vector->array, vector->size * sizeof(Bullet));
		if (reallocatedArray == NULL) {
			printf("Not enough space to reallocate bullet vector!!! Crash!!!");
			exit(1);
		}
		vector->array = reallocatedArray;
	}
}

void FreeBulletVector(BulletVector* vector)
{
	free(vector->array);
	vector->array = NULL;
	vector->used = vector->size = 0;
}