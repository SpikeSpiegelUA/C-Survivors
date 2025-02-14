#include "Vector.h"
#include <stdio.h>
#include <stdlib.h>

void InitVector2DVector(Vector2DVector* vector2DVector, size_t initialSize)
{
	vector2DVector->array = calloc(initialSize, sizeof(Vector2D*));

	if (vector2DVector->array == NULL) {
		printf("Not enough space to allocate the Vector2D vector!!! Crash!!!");
		exit(1);
	}
	vector2DVector->used = 0;
	vector2DVector->size = initialSize;
}

void InsertVector2DVector(Vector2DVector* vector2DVector, Vector2D* element)
{
	if (vector2DVector->used == vector2DVector->size) {
		vector2DVector->size = (size_t)(vector2DVector->size * 1.5f);
		Vector2D** reallocatedArray = realloc(vector2DVector->array, vector2DVector->size * sizeof(Vector2D*));
		if (reallocatedArray == NULL) {
			printf("Not enough space to reallocate the Vector2D vector!!! Crash!!!");
			exit(1);
		}
		vector2DVector->array = reallocatedArray;
		for (int index = (int)vector2DVector->used; index < vector2DVector->size; index++)
			vector2DVector->array[index] = NULL;
	}
	vector2DVector->array[vector2DVector->used++] = element;
}

void Internal_ShrinkVector2DVector(Vector2DVector* vector2DVector, int currentIndex) {
	if (currentIndex + 1 < vector2DVector->used) {
		vector2DVector->array[currentIndex] = vector2DVector->array[currentIndex + 1];
		Internal_ShrinkVector2DVector(vector2DVector, currentIndex + 1);
	}
}

//Remove a bullet object from the enemyJarheadVector. Frees the memory of the object.
void RemoveVector2DVector(Vector2DVector* vector2DVector, Vector2D* element)
{
	for (int i = 0; i < vector2DVector->used; i++) {
		if (vector2DVector->array[i] == element) {
			free(vector2DVector->array[i]);
			vector2DVector->array[i] = NULL;
			Internal_ShrinkVector2DVector(vector2DVector, i);
			vector2DVector->used--;
			break;
		}
	}
}

void FreeVector2DVector(Vector2DVector* vector2DVector)
{
	for (int index = 0; index < vector2DVector->used; index++)
		free(vector2DVector->array[index]);
	free(vector2DVector->array);
	vector2DVector->array = NULL;
	vector2DVector->used = vector2DVector->size = 0;
	free(vector2DVector);
}

bool AddVector2DToGame(Vector2DVector* vector2DVector, float x, float y)
{
	Vector2D* newVector2D = malloc(sizeof(Vector2D));
	if (newVector2D != NULL) {
		newVector2D->x = x;
		newVector2D->y = y;
		InsertVector2DVector(vector2DVector, newVector2D);
		return true;
	}
	else {
		printf("Not enough space for a new Vector2D. Crash!!!");
		return false;
	}
}

void RemoveVector2DFromGame(Vector2DVector* vector2DVector, int index)
{
	if (vector2DVector->array[index] != NULL)
		RemoveVector2DVector(vector2DVector, vector2DVector->array[index]);
}


float DotProduct(Vector2D* firstVector, Vector2D* secondVector)
{
	return firstVector->x * secondVector->x + firstVector->y * secondVector->y;
}

float Magnitude(Vector2D* firstVector)
{
	return (float)sqrt(firstVector->x * firstVector->x + firstVector->y * firstVector->y);
}

Vector2D* NormalizeVector(Vector2D* vector)
{
	float magnitude = Magnitude(vector);
	vector->x = vector->x / magnitude;
	vector->y = vector->y / magnitude;
	return vector;
}

