#include "WorldSubsystem.h"

WorldSubsystem::WorldSubsystem(World &world) : world_(world) {}

World& WorldSubsystem::GetWorld() const {
    return world_;
}