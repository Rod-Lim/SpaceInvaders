#pragma once

#include <array>

#include "../../subsystems/EngineSubsystem.h"

class InputSubsystem : public EngineSubsystem {
public:
    explicit InputSubsystem(Engine& engine);

    void Initialize(SubsystemCollection& collection) override;
    void Deinitialize() override;

    void Poll();

    bool IsDown(int key) const;
    bool IsPressed(int key) const;

private:
    static constexpr int KeyCapacity = 512;

    std::array<bool, KeyCapacity> down_{};
    std::array<bool, KeyCapacity> pressed_{};
};