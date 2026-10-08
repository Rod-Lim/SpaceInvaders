#include "LocalPlayerSubsystem.h"

LocalPlayerSubsystem::LocalPlayerSubsystem(LocalPlayer &localPlayer) : localPlayer_(localPlayer) {}

LocalPlayer& LocalPlayerSubsystem::GetLocalPlayer() const {
    return localPlayer_;
}