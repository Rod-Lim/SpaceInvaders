#pragma once

#include "raylib.h"

inline constexpr int ALIEN_ROWS = 5;
inline constexpr int ALIEN_COLS = 11;
inline constexpr int MAX_ALIENS = ALIEN_ROWS * ALIEN_COLS;

struct Alien {
    Vector2 position{};
    Vector2 size{};
    bool alive = false;
};

struct Fleet {
    Alien aliens[MAX_ALIENS]{};

    float stepTimer = 0.0f;
    int direction = 1;
};