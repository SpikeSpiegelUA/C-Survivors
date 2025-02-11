#include "SATCollision.h"
#include "SDL.h"
#include "Math/Projection.h"

void GetAxis(Vector2DVector* axisVector, Vector2DVector* objectVectors);

bool CheckCollision(Vector2DVector* firstObjectVertices, Vector2DVector* secondObjectVertices)
{
	Vector2D* overlappingAxis = malloc(sizeof(Vector2D));
	float overlap = 9999999;
	Vector2DVector* firstObjectVectors = malloc(sizeof(Vector2DVector));
	InitVector2DVector(firstObjectVectors, 8);
	Vector2DVector* secondObjectVectors = malloc(sizeof(Vector2DVector));
	InitVector2DVector(secondObjectVectors, 8);
	//Get vectors of the edges.
	for (int i = 0; i < firstObjectVertices->used; i++) {
		Vector2D edge;
		if (i == firstObjectVertices->used - 1) {
			edge.x = firstObjectVertices->array[0]->x - firstObjectVertices->array[i]->x;
			edge.y = firstObjectVertices->array[0]->y - firstObjectVertices->array[i]->y;
			AddVector2DToGame(firstObjectVectors, edge.x, edge.y);

			break;
		}
		edge.x = firstObjectVertices->array[i + 1]->x - firstObjectVertices->array[i]->x;
		edge.y = firstObjectVertices->array[i + 1]->y - firstObjectVertices->array[i]->y;
		AddVector2DToGame(firstObjectVectors, edge.x, edge.y);
	}
	for (int i = 0; i < secondObjectVertices->used; i++) {
		Vector2D edge;
		if (i == secondObjectVertices->used - 1) {
			edge.x = secondObjectVertices->array[0]->x - secondObjectVertices->array[i]->x;
			edge.y = secondObjectVertices->array[0]->y - secondObjectVertices->array[i]->y;
			AddVector2DToGame(secondObjectVectors, edge.x, edge.y);
			break;
		}
		edge.x = secondObjectVertices->array[i + 1]->x - secondObjectVertices->array[i]->x;
		edge.y = secondObjectVertices->array[i + 1]->y - secondObjectVertices->array[i]->y;
		AddVector2DToGame(secondObjectVectors, edge.x, edge.y);
	}
	Vector2DVector* axis1 = malloc(sizeof(Vector2DVector));
	InitVector2DVector(axis1, 8);
	GetAxis(axis1, firstObjectVectors);
	//Projecting shapes onto the axis using dot product and storing minimum and maximum.
	for (int i = 0; i < axis1->used; i++) {
		Projection projection1;
		float min = DotProduct(axis1->array[i], firstObjectVectors->array[0]);
		float max = min;
		for (int j = 1; j < firstObjectVectors->used; j++) {
			float dotProduct = DotProduct(axis1->array[i], firstObjectVectors->array[j]);
			if (dotProduct < min)
				min = dotProduct;
			else if (dotProduct > max)
				max = dotProduct;
		}
		projection1.min = min;
		projection1.max = max;
		Projection projection2;
		float min = DotProduct(axis1->array[i], secondObjectVectors->array[0]);
		float max = min;
		for (int j = 1; j < secondObjectVectors->used; j++) {
			float dotProduct = DotProduct(axis1->array[i], secondObjectVectors->array[j]);
			if (dotProduct < min)
				min = dotProduct;
			else if (dotProduct > max)
				max = dotProduct;
		}
		projection2.min = min;
		projection2.max = max;
		if (!ProjectionOverlap(&projection1, &projection2))
			return false;
		else {
			float overlap = GetOverlap(&projection1, &projection2);
		}
	}
	Vector2DVector* axis2 = malloc(sizeof(Vector2DVector));
	GetAxis(axis2, secondObjectVectors);
	for (int i = 0; i < axis2->used; i++) {
		Projection projection1;
		float min = DotProduct(axis2->array[i], firstObjectVectors->array[0]);
		float max = min;
		for (int j = 1; j < firstObjectVectors->used; j++) {
			float dotProduct = DotProduct(axis2->array[i], firstObjectVectors->array[j]);
			if (dotProduct < min)
				min = dotProduct;
			else if (dotProduct > max)
				max = dotProduct;
		}
		projection1.min = min;
		projection1.max = max;
		Projection projection2;
		float min = DotProduct(axis2->array[i], secondObjectVectors->array[0]);
		float max = min;
		for (int j = 1; j < secondObjectVectors->used; j++) {
			float dotProduct = DotProduct(axis2->array[i], secondObjectVectors->array[j]);
			if (dotProduct < min)
				min = dotProduct;
			else if (dotProduct > max)
				max = dotProduct;
		}
		projection2.min = min;
		projection2.max = max;
		if (!ProjectionOverlap(&projection1, &projection2))
			return false;
	}
	FreeVector2DVector(firstObjectVectors);
	FreeVector2DVector(secondObjectVectors);
	FreeVector2DVector(axis1);
	FreeVector2DVector(axis2);
	return true;
}

void GetAxis(Vector2DVector* axisVector, Vector2DVector* objectVectors) {
	for (int i = 0; i < objectVectors->used; i++) {
		bool found = false;
		Vector2D ax;
		ax.x = -objectVectors->array[i]->y;
		ax.y = objectVectors->array[i]->x;
		for (int j = 0; j < axisVector->used; j++)
			if (fabsf(axisVector->array[j]->x) == fabsf(ax.x) && fabsf(axisVector->array[j]->y) == fabsf(ax.y)) {
				found = true;
				break;
			}
		
		if (!found)
			AddVector2DToGame(NormalizeVector(axisVector), ax.x, ax.y);
	}
}
