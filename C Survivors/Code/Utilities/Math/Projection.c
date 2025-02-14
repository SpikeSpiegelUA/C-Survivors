#include "Projection.h"
#include "Math.h"

bool ProjectionOverlap(Projection* firstProjection, Projection* secondProjection)
{
	return !(firstProjection->min > secondProjection->max || secondProjection->min > firstProjection->max);
}

float GetOverlap(Projection* firstProjection, Projection* secondProjection)
{
	if (ProjectionOverlap(firstProjection, secondProjection)) {
		return Math_Min(firstProjection->max, secondProjection->max) - Math_Min(firstProjection->min, secondProjection->min);
	}
	else
		return 0;
}
