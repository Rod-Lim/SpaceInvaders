#pragma once

#include "Subsystem.h"

class World;

class WorldSubsystem : public Subsystem {
public:
    explicit WorldSubsystem(World& world);
    World& GetWorld() const;

private:
    World& world_;
};