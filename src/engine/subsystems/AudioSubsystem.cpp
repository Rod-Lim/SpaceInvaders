#include "AudioSubsystem.h"

#include <stdexcept>

#include "raylib.h"

#include "AssetSubsystem.h"
#include "WindowSubsystem.h"

#include "../Engine.h"
#include "../../subsystems/SubsystemCollection.h"

AudioSubsystem::AudioSubsystem(Engine& engine) : EngineSubsystem(engine) {}

void AudioSubsystem::Initialize(SubsystemCollection& collection) {
    collection.InitializeDependency<WindowSubsystem>();

    ::InitAudioDevice();

    initialized_ = ::IsAudioDeviceReady();

    if (!initialized_) {
        throw std::runtime_error("Failed to initialize the audio device.");
    }

    auto& events = GetEngine().GetEvents();

    shotHandle_ = events.OnShotFired.Subscribe<AudioSubsystem, &AudioSubsystem::HandleShotFired>(*this);
    alienHandle_ = events.OnAlienDestroyed.Subscribe<AudioSubsystem, &AudioSubsystem::HandleAlienDestroyed>(*this);
    fleetHandle_ = events.OnFleetStep.Subscribe<AudioSubsystem, &AudioSubsystem::HandleFleetStep>(*this);
}

void AudioSubsystem::Deinitialize() {
    auto& events = GetEngine().GetEvents();

    events.OnShotFired.Unsubscribe(shotHandle_);
    events.OnAlienDestroyed.Unsubscribe(alienHandle_);
    events.OnFleetStep.Unsubscribe(fleetHandle_);

    shotHandle_ = 0;
    alienHandle_ = 0;
    fleetHandle_ = 0;

    if (initialized_) {
        ::CloseAudioDevice();
        initialized_ = false;
    }
}

void AudioSubsystem::Play(const Sound& sound) const {
    if (initialized_) {
        ::PlaySound(sound);
    }
}

void AudioSubsystem::HandleShotFired(const ShotFiredEvent&) {
    auto* assets = GetEngine().GetSubsystem<AssetSubsystem>();

    if (assets) {
        Play(assets->GetSound(SoundId::Shoot));
    }
}

void AudioSubsystem::HandleAlienDestroyed(const AlienDestroyedEvent&) {
    auto* assets = GetEngine().GetSubsystem<AssetSubsystem>();

    if (assets) {
        Play(assets->GetSound(SoundId::AlienDeath));
    }
}

void AudioSubsystem::HandleFleetStep(const FleetStepEvent&) {
    auto* assets = GetEngine().GetSubsystem<AssetSubsystem>();

    if (assets) {
        Play(assets->GetSound(SoundId::FleetStep));
    }
}