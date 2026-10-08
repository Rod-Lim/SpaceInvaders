#include "LocalPlayer.h"

#include "../subsystems/SubsystemRegistry.h"

LocalPlayer::LocalPlayer(Engine& engine, int id) : engine_(engine), id_(id) {
    subsystems_.Create(*this, GetLocalPlayerSubsystemFactories(), "LocalPlayer");
    subsystems_.Initialize();
}

LocalPlayer::~LocalPlayer() {
    subsystems_.Deinitialize();
}

int LocalPlayer::GetId() const {
    return id_;
}

Engine& LocalPlayer::GetEngine() {
    return engine_;
}

const Engine& LocalPlayer::GetEngine() const {
    return engine_;
}