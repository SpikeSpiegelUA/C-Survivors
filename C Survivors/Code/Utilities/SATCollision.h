#pragma once
#include "Math/Vector.h"
#include <stdio.h>

typedef struct {
	Vector2D* shortestOverlapAxis;
	float overlapValue;
};

bool CheckCollision(Vector2DVector* firstObjectVertices, Vector2DVector* secondObjectVertices);