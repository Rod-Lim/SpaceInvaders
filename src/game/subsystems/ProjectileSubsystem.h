#pragma once

#include "../../subsystems/WorldSubsystem.h"
#include "../Bullet.h"

class ProjectileSubsystem : public WorldSubsystem {
public:
    explicit ProjectileSubsystem(World& world);

    bool ShouldCreateSubsystem(const void* outer) const override;

    void Initialize(SubsystemCollection& collection) override;

    bool TrySpawn(const Vector2& position);
    void Update(float deltaTime);

    int CountActiveBullets() const;
};