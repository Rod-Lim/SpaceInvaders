#pragma once

#include "../../subsystems/WorldSubsystem.h"

class FleetSubsystem : public WorldSubsystem {
public:
    explicit FleetSubsystem(World& world);

    bool ShouldCreateSubsystem(const void* outer) const override;

    void Initialize(SubsystemCollection& collection) override;

    bool Update(float deltaTime);

    int CountLivingAliens() const;

private:
    int screenWidth_ = 0;
};