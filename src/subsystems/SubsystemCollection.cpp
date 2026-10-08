#include "SubsystemCollection.h"

#include <iostream>

void SubsystemCollection::Initialize() {
    for (std::size_t i = 0; i < subsystems_.size(); i++) {
        InitializeSubsystem(i);
    }
}

void SubsystemCollection::InitializeSubsystem(std::size_t index) {
    auto& entry = subsystems_[index];

    if (entry.state == State::Initialized) {
        return;
    }

    if (entry.state == State::Initializing) {
        throw std::logic_error("Circular subsystem dependency.");
    }

    entry.state = State::Initializing;

    entry.instance->Initialize(*this);

    entry.state = State::Initialized;
    initializationOrder_.push_back(index);

    std::cout
        << "[Initialize]"
        << " [" << level_ << "]"
        << ' ' << entry.name
        << " owner=" << owner_
        << '\n';
}

void SubsystemCollection::Deinitialize() {
    for (auto i = initializationOrder_.rbegin(); i != initializationOrder_.rend(); i++) {
        auto& entry = subsystems_[*i];

        entry.instance->Deinitialize();
        entry.state = State::NotInitialized;

        std::cout
            << "[Deinitialize]"
            << " [" << level_ << "]"
            << ' ' << entry.name
            << " owner=" << owner_
            << '\n';
         }

    initializationOrder_.clear();
    subsystems_.clear();
}