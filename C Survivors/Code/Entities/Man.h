#pragma once

#include <stdbool.h>

typedef struct {
    float x, y;
    float w, h;
    float dx, dy;
    char name[31];
    short lives;
    bool onLedge, isDead;
    int currentSprite;
    bool facingRight;
} Man;