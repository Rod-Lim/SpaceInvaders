#include "SubsystemRegistry.h"
#include "../engine/subsystems/WindowSubsystem.h"
#include "../engine/subsystems/AudioSubsystem.h"
#include "../engine/subsystems/AssetSubsystem.h"
#include "../engine/subsystems/InputSubsystem.h"
#include "../engine/subsystems/SaveSubsystem.h"

#include "../game/subsystems/FleetSubsystem.h"
#include "../game/subsystems/ProjectileSubsystem.h"
#include "../game/subsystems/CollisionSubsystem.h"
#include "../game/subsystems/ScoreSubsystem.h"
#include "../game/subsystems/GameRulesSubsystem.h"
#include "../game/subsystems/PlayerInputSubsystem.h"
#include "../game/subsystems/PlayerStatsSubsystem.h"

#include <array>

namespace {
    constexpr std::array<EngineSubsystemFactory, 5> engineFactories{
        REGISTER_ENGINE_SUBSYSTEM(WindowSubsystem),
        REGISTER_ENGINE_SUBSYSTEM(AudioSubsystem),
        REGISTER_ENGINE_SUBSYSTEM(AssetSubsystem),
        REGISTER_ENGINE_SUBSYSTEM(InputSubsystem),
        REGISTER_ENGINE_SUBSYSTEM(SaveSubsystem),
    };

    constexpr std::array<WorldSubsystemFactory, 5> worldFactories {
        REGISTER_WORLD_SUBSYSTEM(FleetSubsystem),
        REGISTER_WORLD_SUBSYSTEM(ProjectileSubsystem),
        REGISTER_WORLD_SUBSYSTEM(CollisionSubsystem),
        REGISTER_WORLD_SUBSYSTEM(ScoreSubsystem),
        REGISTER_WORLD_SUBSYSTEM(GameRulesSubsystem),
    };

    constexpr std::array<LocalPlayerSubsystemFactory, 2> localPlayerFactories{
        REGISTER_LOCAL_PLAYER_SUBSYSTEM(PlayerInputSubsystem),
        REGISTER_LOCAL_PLAYER_SUBSYSTEM(PlayerStatsSubsystem),
    };
}

EngineSubsystemFactoryView GetEngineSubsystemFactories() {
    return {
        engineFactories.data(),
        engineFactories.size()
    };
}

WorldSubsystemFactoryView GetWorldSubsystemFactories() {
    return {
        worldFactories.data(),
        worldFactories.size()
    };
}

LocalPlayerSubsystemFactoryView GetLocalPlayerSubsystemFactories() {
    return {
        localPlayerFactories.data(),
        localPlayerFactories.size()
    };
}