#pragma once
#include <stdio.h>
#include "Math/Vector.h"
#include "Math/Projection.h"

typedef struct {
	Vector2D* shortestOverlapAxis;
	float overlapValue;
} SAT;

bool CheckCollision(Vector2DVector* firstObjectVertices, Vector2DVector* secondObjectVertices);