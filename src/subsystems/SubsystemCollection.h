#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "Subsystem.h"
#include "SubsystemRegistry.h"

class SubsystemCollection {
public:
    template<typename Owner, typename Factory>
void Create(
    Owner& owner,
    SubsystemFactoryView<Factory> factories,
    const char* level
) {
        level_ = level;
        owner_ = static_cast<const void*>(std::addressof(owner));

        subsystems_.reserve(factories.size);
        initializationOrder_.reserve(factories.size);

        for (std::size_t i = 0; i < factories.size; i++) {
            const auto& factory = factories.data[i];

            auto subsystem = factory.create(owner);

            if (subsystem) {
                subsystems_.push_back({
                    factory.name,
                    std::move(subsystem),
                    State::NotInitialized
                });
            }
        }
    }

    void Initialize();
    void Deinitialize();

    template<typename T>
    T* Get() {
        for (auto& entry : subsystems_) {
            if (auto* result = dynamic_cast<T*>(entry.instance.get())) {
                return result;
            }
        }

        return nullptr;
    }

    template<typename T>
    T& InitializeDependency() {
        for (std::size_t i = 0; i < subsystems_.size(); i++) {
            auto* result =
                dynamic_cast<T*>(subsystems_[i].instance.get());

            if (result) {
                InitializeSubsystem(i);
                return *result;
            }
        }

        throw std::logic_error("Required subsystem is missing.");
    }

private:
    enum class State {
        NotInitialized,
        Initializing,
        Initialized
    };

    struct Entry {
        const char* name;
        std::unique_ptr<Subsystem> instance;
        State state;
    };

    void InitializeSubsystem(std::size_t index);

    std::vector<Entry> subsystems_;
    std::vector<std::size_t> initializationOrder_;

    const char* level_ = "Unknown";
    const void* owner_ = nullptr;
};