#pragma once
#include <math.h>
#include <stdbool.h>

typedef struct {
	float x, y;
} Vector2D;

typedef struct {
	Vector2D** array;
	size_t used, size;
} Vector2DVector;

void InitVector2DVector(Vector2DVector* vector2DVector, size_t initialSize);
void InsertVector2DVector(Vector2DVector* vector2DVector, Vector2D* element);
void RemoveVector2DVector(Vector2DVector* vector2DVector, Vector2D* element);
void FreeVector2DVector(Vector2DVector* vector2DVector);
bool AddVector2DToGame(Vector2DVector* vector2DVector, float x, float y);
void RemoveVector2DFromGame(Vector2DVector* vector2DVector, int index);

float DotProduct(Vector2D* firstVector, Vector2D* secondVector);
float Magnitude(Vector2D* firstVector);
Vector2D* NormalizeVector(Vector2D* vector);