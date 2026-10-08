#pragma once

#include "raylib.h"

struct Player {
    Vector2 position{};
    Vector2 size{};
    float speed = 350.0f;
    float fireCooldown = 0.0f;
};