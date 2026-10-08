#include "AssetSubsystem.h"
#include "AudioSubsystem.h"
#include "../../subsystems/SubsystemCollection.h"

AssetSubsystem::AssetSubsystem(Engine& engine) : EngineSubsystem(engine) {}

void AssetSubsystem::Initialize(SubsystemCollection& collection) {
    collection.InitializeDependency<AudioSubsystem>();

    shoot_ = ::LoadSound("assets/shoot.ogg");
    alienDeath_ = ::LoadSound("assets/alien_death.ogg");
    step_ = ::LoadSound("assets/step.ogg");
}

void AssetSubsystem::Deinitialize() {
    ::UnloadSound(shoot_);
    ::UnloadSound(alienDeath_);
    ::UnloadSound(step_);

    shoot_ = {};
    alienDeath_ = {};
    step_ = {};
}

const Sound& AssetSubsystem::GetSound(SoundId id) const {
    switch (id) {
        case SoundId::Shoot:
            return shoot_;

        case SoundId::AlienDeath:
            return alienDeath_;

        case SoundId::FleetStep:
            return step_;
    }
}
