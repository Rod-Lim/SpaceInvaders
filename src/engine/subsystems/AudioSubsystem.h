#pragma once

#include "../../subsystems/EngineSubsystem.h"
#include "../../game/GameEvents.h"

struct Sound;

class AudioSubsystem : public EngineSubsystem {
public:
    explicit AudioSubsystem(Engine& engine);

    void Initialize(SubsystemCollection& collection) override;
    void Deinitialize() override;

    void Play(const Sound& sound) const;

private:
    void HandleShotFired(const ShotFiredEvent& event);
    void HandleAlienDestroyed(const AlienDestroyedEvent& event);
    void HandleFleetStep(const FleetStepEvent& event);

    MulticastDelegate<ShotFiredEvent>::Handle shotHandle_ = 0;
    MulticastDelegate<AlienDestroyedEvent>::Handle alienHandle_ = 0;
    MulticastDelegate<FleetStepEvent>::Handle fleetHandle_ = 0;

    bool initialized_ = false;
};