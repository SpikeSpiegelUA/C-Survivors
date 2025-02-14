#include "Math.h"

float Math_Min(float firstValue, float secondValue)
{
	if (firstValue > secondValue)
		return secondValue;
	else
		return firstValue;
}

float Math_Max(float firstValue, float secondValue)
{
	if (firstValue < secondValue)
		return secondValue;
	else
		return firstValue;
}
