#pragma once

#include "../../subsystems/WorldSubsystem.h"
#include "../GameStatus.h"

class FleetSubsystem;

class GameRulesSubsystem : public WorldSubsystem {
public:
    explicit GameRulesSubsystem(World& world);

    bool ShouldCreateSubsystem(const void* outer) const override;

    void Initialize(SubsystemCollection& collection) override;
    void Deinitialize() override;

    void Update(float playerY);

    GameStatus GetStatus() const;
    bool IsPlaying() const;

private:
    FleetSubsystem* fleetSubsystem_ = nullptr;

    GameStatus status_ = GameStatus::Playing;
};