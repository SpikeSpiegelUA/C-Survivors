#pragma once

#include "Utilities/Vector.h"

CREATE_VECTOR_STRUCT(Bullet)
CREATE_INIT_VECTOR(Bullet)
CREATE_INSERT_VECTOR(Bullet)
CREATE_FREE_VECTOR(Bullet)

typedef struct {
    float x, y, dx;
} Bullet;

void RemoveBullet(int bulletsArrayIndex);
BulletVector* AddBullet(Bullet* gameState, float x, float y, float dx);