#include "FleetSubsystem.h"

#include <stdexcept>

#include "../World.h"
#include "../../engine/Engine.h"
#include "../../engine/subsystems/WindowSubsystem.h"

FleetSubsystem::FleetSubsystem(World& world) : WorldSubsystem(world) {}

bool FleetSubsystem::ShouldCreateSubsystem(const void*) const {
    return GetWorld().GetType() == WorldType::Gameplay;
}

void FleetSubsystem::Initialize(SubsystemCollection&) {
    auto* window = GetWorld().GetEngine().GetSubsystem<WindowSubsystem>();

    if (!window) {
        throw std::runtime_error(
            "FleetSubsystem requires WindowSubsystem."
        );
    }

    screenWidth_ = window->GetWidth();

    constexpr float ALIEN_WIDTH = 40.0f;
    constexpr float ALIEN_HEIGHT = 24.0f;
    constexpr float COLUMN_SPACING = 18.0f;
    constexpr float ROW_SPACING = 24.0f;
    constexpr float START_Y = 80.0f;

    constexpr float GRID_WIDTH = ALIEN_COLS * ALIEN_WIDTH + (ALIEN_COLS - 1) * COLUMN_SPACING;

    const float startX = (screenWidth_ - GRID_WIDTH) / 2.0f;

    Fleet& fleet = GetWorld().GetFleet();

    for (int row = 0; row < ALIEN_ROWS; row++) {
        for (int col = 0; col < ALIEN_COLS; col++) {
            const int index = row * ALIEN_COLS + col;

            fleet.aliens[index] = {
                .position = {
                    startX + col * (ALIEN_WIDTH + COLUMN_SPACING),
                    START_Y + row * (ALIEN_HEIGHT + ROW_SPACING)
                },
                .size = {ALIEN_WIDTH, ALIEN_HEIGHT},
                .alive = true
            };
        }
    }

    fleet.stepTimer = 0.0f;
    fleet.direction = 1;
}

int FleetSubsystem::CountLivingAliens() const {
    const Fleet& fleet = GetWorld().GetFleet();

    int count = 0;

    for (const Alien& alien : fleet.aliens) {
        if (alien.alive) {
            ++count;
        }
    }

    return count;
}

bool FleetSubsystem::Update(float deltaTime) {
    constexpr float EDGE_MARGIN = 20.0f;
    constexpr float STEP_DISTANCE = 10.0f;
    constexpr float DESCENT_DISTANCE = 18.0f;

    Fleet& fleet = GetWorld().GetFleet();

    const int livingAliens = CountLivingAliens();

    if (livingAliens == 0) {
        return false;
    }

    const float stepInterval = 0.05f + 0.45f * static_cast<float>(livingAliens) / MAX_ALIENS;

    fleet.stepTimer += deltaTime;

    if (fleet.stepTimer < stepInterval) {
        return false;
    }

    fleet.stepTimer -= stepInterval;

    bool reachesEdge = false;

    for (const Alien& alien : fleet.aliens) {
        if (!alien.alive) {
            continue;
        }

        const float nextX = alien.position.x + fleet.direction * STEP_DISTANCE;

        if (nextX < EDGE_MARGIN || nextX + alien.size.x > screenWidth_ - EDGE_MARGIN) {
            reachesEdge = true;
            break;
        }
    }

    if (reachesEdge) {
        fleet.direction = -fleet.direction;
    }

    for (Alien& alien : fleet.aliens) {
        if (!alien.alive) {
            continue;
        }

        if (reachesEdge) {
            alien.position.y += DESCENT_DISTANCE;
        } else {
            alien.position.x += fleet.direction * STEP_DISTANCE;
        }
    }

    return true;
}