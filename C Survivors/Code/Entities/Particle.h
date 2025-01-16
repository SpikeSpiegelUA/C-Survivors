#pragma once
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	float x, y, dx, dy;
	float life;
} Particle;

typedef struct {
	Particle** array;
	size_t used, size;
} ParticleVector;

void InitParticleVector(ParticleVector* particleVector, size_t initialSize);
void InsertParticleVector(ParticleVector* particleVector, Particle* element);
void RemoveParticleVector(ParticleVector* particleVector, Particle* element);
void FreeParticleVector(ParticleVector* particleVector);
bool AddParticleToGame(ParticleVector* particleVector, float x, float y, float velocity, int numParticles);
void RemoveParticleFromGame(ParticleVector* particleVector, int index);
