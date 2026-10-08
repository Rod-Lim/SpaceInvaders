#pragma once

#include "../../subsystems/EngineSubsystem.h"
#include "raylib.h"

enum class SoundId {
    Shoot,
    AlienDeath,
    FleetStep
};

class AssetSubsystem : public EngineSubsystem {
public:
    explicit AssetSubsystem(Engine& engine);

    void Initialize(SubsystemCollection& collection) override;
    void Deinitialize() override;

    const Sound& GetSound(SoundId id) const;

private:
    Sound shoot_{};
    Sound alienDeath_{};
    Sound step_{};
};