#include "World.h"

#include "../subsystems/SubsystemRegistry.h"
#include "../engine/Engine.h"
#include "../engine/subsystems/WindowSubsystem.h"
#include "subsystems/ProjectileSubsystem.h"

World::World(Engine& engine, WorldType type, int initialScore) : engine_(engine), type_(type), initialScore_(initialScore) {
    if (type_ == WorldType::Gameplay) {
        auto* window = engine_.GetSubsystem<WindowSubsystem>();

        if (!window) {
            throw std::runtime_error("World requires WindowSubsystem.");
        }

        player_ = {
            .position = {
                window->GetWidth() / 2.0f - 25.0f,
                window->GetHeight() - 70.0f
            },
            .size = {50.0f, 25.0f},
            .speed = 350.0f,
            .fireCooldown = 0.0f
        };
    }

    subsystems_.Create(*this, GetWorldSubsystemFactories(), "World");
    subsystems_.Initialize();
}

World::~World() {
    subsystems_.Deinitialize();
}

WorldType World::GetType() const {
    return type_;
}

Engine& World::GetEngine() {
    return engine_;
}

Fleet& World::GetFleet() {
    return fleet_;
}

const Fleet& World::GetFleet() const {
    return fleet_;
}

Bullet* World::GetBullets() {
    return bullets_;
}

const Bullet* World::GetBullets() const {
    return bullets_;
}

Player& World::GetPlayer() {
    return player_;
}

const Player& World::GetPlayer() const {
    return player_;
}

bool World::UpdatePlayer(const InputState& input, float deltaTime) {
    auto* projectiles = GetSubsystem<ProjectileSubsystem>();
    auto* window = engine_.GetSubsystem<WindowSubsystem>();

    float direction = 0.0f;

    if (input.moveLeft) {
        direction -= 1.0f;
    }

    if (input.moveRight) {
        direction += 1.0f;
    }

    player_.position.x += direction * player_.speed * deltaTime;

    const float maxX = window->GetWidth() - player_.size.x;

    if (player_.position.x > maxX) {
        player_.position.x = maxX;
    } else if (player_.position.x < 0.0f) {
        player_.position.x = 0.0f;
    }

    player_.fireCooldown -= deltaTime;

    if (!input.shoot || player_.fireCooldown > 0.0f) {
        return false;
    }

    const Vector2 bulletPosition{
        player_.position.x + (player_.size.x - 4.0f) / 2.0f,
        player_.position.y - 12.0f
    };

    if (!projectiles->TrySpawn(bulletPosition)) {
        return false;
    }

    player_.fireCooldown = 0.25f;

    return true;
}

int World::GetInitialScore() const {
    return initialScore_;
}

void World::LogActiveSubsystems() const {
    subsystems_.LogActiveSubsystems();
}