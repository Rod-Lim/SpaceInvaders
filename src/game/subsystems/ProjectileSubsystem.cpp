#include "ProjectileSubsystem.h"

#include "../World.h"

namespace {
    constexpr float BULLET_SPEED = 500.0f;
}

ProjectileSubsystem::ProjectileSubsystem(World& world) : WorldSubsystem(world) {}

bool ProjectileSubsystem::ShouldCreateSubsystem(const void*) const {
    return GetWorld().GetType() == WorldType::Gameplay;
}

void ProjectileSubsystem::Initialize(SubsystemCollection&) {
    Bullet* bullets = GetWorld().GetBullets();

    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i] = {};
    }
}

bool ProjectileSubsystem::TrySpawn(const Vector2& position) {
    Bullet* bullets = GetWorld().GetBullets();

    for (int i = 0; i < MAX_BULLETS; i++) {
        Bullet& bullet = bullets[i];

        if (bullet.active) {
            continue;
        }

        bullet.position = position;
        bullet.size = {4.0f, 12.0f};
        bullet.active = true;

        return true;
    }

    return false;
}

void ProjectileSubsystem::Update(float deltaTime) {
    Bullet* bullets = GetWorld().GetBullets();

    for (int i = 0; i < MAX_BULLETS; i++) {
        Bullet& bullet = bullets[i];

        if (!bullet.active) {
            continue;
        }

        bullet.position.y -= BULLET_SPEED * deltaTime;

        if (bullet.position.y + bullet.size.y < 0.0f) {
            bullet.active = false;
        }
    }
}

int ProjectileSubsystem::CountActiveBullets() const {
    const Bullet* bullets = GetWorld().GetBullets();

    int count = 0;

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            ++count;
        }
    }

    return count;
}