#include "PlayerInputSubsystem.h"

#include <stdexcept>

#include "raylib.h"

#include "../LocalPlayer.h"
#include "../../engine/Engine.h"
#include "../../engine/subsystems/InputSubsystem.h"

PlayerInputSubsystem::PlayerInputSubsystem(LocalPlayer& player) : LocalPlayerSubsystem(player) {
    for (auto& keys : bindings_) {
        keys.fill(UnboundKey);
    }
}

void PlayerInputSubsystem::Initialize(SubsystemCollection&) {
    inputSubsystem_ = GetLocalPlayer().GetEngine().GetSubsystem<InputSubsystem>();

    if (!inputSubsystem_) {
        throw std::runtime_error("PlayerInputSubsystem requires InputSubsystem.");
    }

    BindKey(PlayerAction::MoveLeft, KEY_LEFT);
    BindKey(PlayerAction::MoveLeft, KEY_A);
    BindKey(PlayerAction::MoveLeft, KEY_Q);

    BindKey(PlayerAction::MoveRight, KEY_RIGHT);
    BindKey(PlayerAction::MoveRight, KEY_D);

    BindKey(PlayerAction::Fire, KEY_SPACE);
}

void PlayerInputSubsystem::Deinitialize() {
    inputSubsystem_ = nullptr;
}

void PlayerInputSubsystem::BindKey(PlayerAction action, int key) {
    const int index = static_cast<int>(action);

    if (index < 0 || index >= ActionCount || key < 0) {
        throw std::invalid_argument("Invalid input binding.");
    }

    auto& keys = bindings_[index];

    for (int boundKey : keys) {
        if (boundKey == key) {
            return;
        }
    }

    for (int& boundKey : keys) {
        if (boundKey == UnboundKey) {
            boundKey = key;
            return;
        }
    }

    throw std::logic_error("Too many keys bound to this action.");
}

bool PlayerInputSubsystem::IsActionDown(PlayerAction action) const {
    const auto& keys = bindings_[static_cast<int>(action)];

    for (int key : keys) {
        if (key != UnboundKey && inputSubsystem_->IsDown(key)) {
            return true;
        }
    }

    return false;
}

InputState PlayerInputSubsystem::ReadInputs() const {
    return {
        .moveLeft = IsActionDown(PlayerAction::MoveLeft),
        .moveRight = IsActionDown(PlayerAction::MoveRight),
        .shoot = IsActionDown(PlayerAction::Fire)
    };
}