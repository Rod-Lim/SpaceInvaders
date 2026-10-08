#pragma once

#include <memory>
#include <type_traits>

#include "EngineSubsystem.h"
#include "WorldSubsystem.h"
#include "LocalPlayerSubsystem.h"

class Engine;
class World;
class LocalPlayer;

template<typename Base, typename Owner>
struct SubsystemFactory {
    const char* name;

    std::unique_ptr<Base> (*create)(Owner&);
};

template<typename Concrete, typename Base, typename Owner>
std::unique_ptr<Base> CreateRegisteredSubsystem(Owner& owner) {
    static_assert(
        std::is_base_of_v<Base, Concrete>,
        "The subsystem must inherit from the expected base class."
    );

    static_assert(
        std::is_constructible_v<Concrete, Owner&>,
        "The subsystem must have a constructor accepting its owner."
    );

    auto candidate = std::make_unique<Concrete>(owner);

    if (!candidate->ShouldCreateSubsystem(&owner)) {
        return nullptr;
    }

    return candidate;
}

using EngineSubsystemFactory = SubsystemFactory<EngineSubsystem, Engine>;
using WorldSubsystemFactory = SubsystemFactory<WorldSubsystem, World>;
using LocalPlayerSubsystemFactory = SubsystemFactory<LocalPlayerSubsystem, LocalPlayer>;

#define REGISTER_ENGINE_SUBSYSTEM(Type) \
    EngineSubsystemFactory{ \
        #Type, \
        &CreateRegisteredSubsystem<Type, EngineSubsystem, Engine> \
    }

#define REGISTER_WORLD_SUBSYSTEM(Type) \
    WorldSubsystemFactory{ \
        #Type, \
        &CreateRegisteredSubsystem<Type, WorldSubsystem, World> \
    }

#define REGISTER_LOCAL_PLAYER_SUBSYSTEM(Type) \
    LocalPlayerSubsystemFactory{ \
        #Type, \
        &CreateRegisteredSubsystem<Type, LocalPlayerSubsystem, LocalPlayer> \
    }