#pragma once

#include "../../subsystems/WorldSubsystem.h"
#include "../GameEvents.h"

class ScoreSubsystem : public WorldSubsystem {
public:
    explicit ScoreSubsystem(World& world);

    bool ShouldCreateSubsystem(const void* outer) const override;

    void Initialize(SubsystemCollection& collection) override;
    void Deinitialize() override;

    int GetScore() const;

private:
    void HandleAlienDestroyed(const AlienDestroyedEvent& event);

    static constexpr int PointsPerAlien = 10;

    int score_ = 0;

    MulticastDelegate<AlienDestroyedEvent>::Handle alienHandle_ = 0;
};