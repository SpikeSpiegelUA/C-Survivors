#pragma once
#include <stdbool.h>

typedef struct {
	float min, max;
} Projection;

bool ProjectionOverlap(Projection* firstProjection, Projection* secondProjection);
float GetOverlap(Projection* firstProjection, Projection* secondProjection);