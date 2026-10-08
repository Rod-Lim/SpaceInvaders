#include "CollisionSubsystem.h"

#include "raylib.h"

#include "../World.h"
#include "FleetSubsystem.h"
#include "ProjectileSubsystem.h"
#include "../../subsystems/SubsystemCollection.h"

CollisionSubsystem::CollisionSubsystem(World& world) : WorldSubsystem(world) {}

bool CollisionSubsystem::ShouldCreateSubsystem(const void*) const {
    return GetWorld().GetType() == WorldType::Gameplay;
}

void CollisionSubsystem::Initialize(SubsystemCollection& collection) {
    collection.InitializeDependency<FleetSubsystem>();
    collection.InitializeDependency<ProjectileSubsystem>();
}

int CollisionSubsystem::Update() {
    Bullet* bullets = GetWorld().GetBullets();
    Fleet& fleet = GetWorld().GetFleet();

    int destroyedAliens = 0;

    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++) {
        Bullet& bullet = bullets[bulletIndex];

        if (!bullet.active) {
            continue;
        }

        const Rectangle bulletBounds{
            bullet.position.x,
            bullet.position.y,
            bullet.size.x,
            bullet.size.y
        };

        for (Alien& alien : fleet.aliens) {
            if (!alien.alive) {
                continue;
            }

            const Rectangle alienBounds{
                alien.position.x,
                alien.position.y,
                alien.size.x,
                alien.size.y
            };

            if (!::CheckCollisionRecs(bulletBounds, alienBounds)) {
                continue;
            }

            bullet.active = false;
            alien.alive = false;
            ++destroyedAliens;

            break;
        }
    }

    return destroyedAliens;
}