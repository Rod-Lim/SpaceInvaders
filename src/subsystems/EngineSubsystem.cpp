#include "EngineSubsystem.h"

EngineSubsystem::EngineSubsystem(Engine &engine) : engine_(engine) {}

Engine& EngineSubsystem::GetEngine() const {
    return engine_;
}