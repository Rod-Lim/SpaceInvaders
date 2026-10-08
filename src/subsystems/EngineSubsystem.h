#pragma once

#include "Subsystem.h"

class Engine;

class EngineSubsystem : public Subsystem {
public:
    explicit EngineSubsystem(Engine& engine);
    Engine& GetEngine() const;

private:
    Engine& engine_;
};