#pragma once

#include <array>

#include "../../subsystems/LocalPlayerSubsystem.h"
#include "../InputState.h"

class InputSubsystem;

enum class PlayerAction {
    MoveLeft,
    MoveRight,
    Fire,
    Count
};

class PlayerInputSubsystem : public LocalPlayerSubsystem {
public:
    explicit PlayerInputSubsystem(LocalPlayer& player);

    void Initialize(SubsystemCollection& collection) override;
    void Deinitialize() override;

    InputState ReadInputs() const;

    void BindKey(PlayerAction action, int key);

private:
    static constexpr int ActionCount = static_cast<int>(PlayerAction::Count);

    static constexpr int MaxKeysPerAction = 3;
    static constexpr int UnboundKey = -1;

    bool IsActionDown(PlayerAction action) const;

    InputSubsystem* inputSubsystem_ = nullptr;

    std::array<std::array<int, MaxKeysPerAction>, ActionCount> bindings_;
};