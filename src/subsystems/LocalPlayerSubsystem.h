#pragma once

#include "Subsystem.h"

class LocalPlayer;

class LocalPlayerSubsystem : public Subsystem {
public:
    explicit LocalPlayerSubsystem(LocalPlayer& localPlayer);
    LocalPlayer& GetLocalPlayer() const;

private:
    LocalPlayer& localPlayer_;
};