#pragma once

#include "SubsystemRegistration.h"

template<typename Factory>
struct SubsystemFactoryView {
    const Factory* data;
    std::size_t size;
};

using EngineSubsystemFactoryView = SubsystemFactoryView<EngineSubsystemFactory>;
using WorldSubsystemFactoryView = SubsystemFactoryView<WorldSubsystemFactory>;
using LocalPlayerSubsystemFactoryView = SubsystemFactoryView<LocalPlayerSubsystemFactory>;

EngineSubsystemFactoryView GetEngineSubsystemFactories();
WorldSubsystemFactoryView GetWorldSubsystemFactories();
LocalPlayerSubsystemFactoryView GetLocalPlayerSubsystemFactories();