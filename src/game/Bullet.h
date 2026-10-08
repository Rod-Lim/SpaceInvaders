#pragma once

#include "raylib.h"

inline constexpr int MAX_BULLETS = 10;

struct Bullet {
    Vector2 position{};
    Vector2 size{};
    bool active = false;
};