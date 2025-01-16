#include "Particle.h"
#include <stdio.h>

void InitParticleVector(ParticleVector* particleVector, size_t initialSize)
{
	particleVector->array = (Particle**)calloc(initialSize, sizeof(Particle*));

	if (particleVector->array == NULL) {
		printf("Not enough space to allocate the particle vector!!! Crash!!!");
		exit(1);
	}
	particleVector->used = 0;
	particleVector->size = initialSize;
}

void InsertParticleVector(ParticleVector* particleVector, Particle* element)
{
	if (particleVector->used == particleVector->size) {
		particleVector->size = (size_t)(particleVector->size * 1.5f);
		Particle** reallocatedArray = (Particle**)realloc(particleVector->array, particleVector->size * sizeof(Particle*));
		if (reallocatedArray == NULL) {
			printf("Not enough space to reallocate the particle vector!!! Crash!!!");
			exit(1);
		}
		particleVector->array = reallocatedArray;
		for (int index = (int)particleVector->used; index < particleVector->size; index++)
			particleVector->array[index] = NULL;
	}
	particleVector->array[particleVector->used++] = element;
}

void Internal_ShrinkParticleVector(ParticleVector* particleVector, int currentIndex) {
	if (currentIndex + 1 < particleVector->used) {
		particleVector->array[currentIndex] = particleVector->array[currentIndex + 1];
		Internal_ShrinkParticleVector(particleVector, currentIndex + 1);
	}
}

//Remove a bullet object from the vector. Frees the memory of the object.
void RemoveParticleVector(ParticleVector* particleVector, Particle* element)
{
	for (int i = 0; i < particleVector->used; i++) {
		if (particleVector->array[i] == element) {
			free(particleVector->array[i]);
			particleVector->array[i] = NULL;
			Internal_ShrinkParticleVector(particleVector, i);
			particleVector->used--;
			break;
		}
	}
}

void FreeParticleVector(ParticleVector* particleVector)
{
	for (int index = 0; index < particleVector->used; index++)
		free(particleVector->array[index]);
	free(particleVector->array);
	particleVector->array = NULL;
	particleVector->used = particleVector->size = 0;
	free(particleVector);
}

bool AddParticleToGame(ParticleVector* particleVector, float x, float y, float velocity, int numParticles)
{
	for (int i = 0; i < numParticles; i++) {
		Particle* newParticle = (Particle*)malloc(sizeof(Particle));
		if (newParticle != NULL) {
			newParticle->x = x;
			newParticle->y = y;
			newParticle->dx = (((float)rand() / RAND_MAX) - 0.5f) * velocity;
			newParticle->dy = (-0.5f + ((float)rand() / RAND_MAX) - 0.5f) * velocity;
			newParticle->life = 60;
			InsertParticleVector(particleVector, newParticle);
		}
		else {
			printf("Not enough space for a new particle. Crash!!!");
			return false;
		}
	}
	return true;
}

void RemoveParticleFromGame(ParticleVector* particleVector, int index)
{
	if (particleVector->array[index] != NULL)
		RemoveParticleVector(particleVector, particleVector->array[index]);
}