#pragma once

#include "../../subsystems/WorldSubsystem.h"

class CollisionSubsystem : public WorldSubsystem {
public:
    explicit CollisionSubsystem(World& world);

    bool ShouldCreateSubsystem(const void* outer) const override;

    void Initialize(SubsystemCollection& collection) override;

    int Update();
};