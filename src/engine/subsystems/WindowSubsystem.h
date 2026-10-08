#pragma once

#include "../../subsystems/EngineSubsystem.h"

class WindowSubsystem : public EngineSubsystem {
public:
    explicit WindowSubsystem(Engine& engine);

    void Initialize(SubsystemCollection& collection) override;
    void Deinitialize() override;

    bool ShouldClose() const;

    int GetWidth() const;
    int GetHeight() const;

private:
    int width_ = 800;
    int height_ = 600;
};