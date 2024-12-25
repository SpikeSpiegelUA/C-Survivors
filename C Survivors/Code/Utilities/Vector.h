#pragma once

#include <stdlib.h>

// I don't have the luxuries of ++ in C, so I'm gonna write this macro shit.\

//Use this macro to create a struct for a vector of your type. 
#define CREATE_VECTOR_STRUCT(type) \
typedef struct {                   \
	type* array;                   \
	size_t used, size;             \
} type##Vector;

//Use this macro to create an init function for a vector of your type.
#define CREATE_INIT_VECTOR(type)                                  \
void Init##type##Vector(type##Vector* vector, size_t initialSize) \
{												                  \
	vector->array = malloc(initialSize * sizeof(type));           \
	vector->used = 0;								              \
	vector->size = initialSize;						              \
}

//Use this macro to create an insert function for a vector of your type.
#define CREATE_INSERT_VECTOR(type)                                          \
void Insert##type##Vector(type##Vector* vector, type element)               \
{												                            \
  if (vector->used == vector->size) {                                       \
	  vector->size = (size_t)(vector->size * 1.5f);                         \
	  vector->array = realloc(vector->array, vector->size * sizeof(type));  \
  }																		    \
  vector->array[vector->used++] = element;									\
}

//Use this macro to create a free function for a vector of your type.
#define CREATE_FREE_VECTOR(type)                                            \
void Insert##type##Vector(type##Vector* vector)								\
{												                            \
    free(vector->array);													\
	vector->array = NULL;													\
	vector->used = vector->size = 0;										\
}