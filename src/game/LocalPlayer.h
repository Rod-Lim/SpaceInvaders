#pragma once

#include "../subsystems/SubsystemCollection.h"

class Engine;

class LocalPlayer {
public:
    LocalPlayer(Engine& engine, int id);
    ~LocalPlayer();

    LocalPlayer(const LocalPlayer&) = delete;
    LocalPlayer& operator=(const LocalPlayer&) = delete;

    LocalPlayer(LocalPlayer&&) = delete;
    LocalPlayer& operator=(LocalPlayer&&) = delete;

    int GetId() const;

    Engine& GetEngine();
    const Engine& GetEngine() const;

    template<typename T>
    T* GetSubsystem() {
        return subsystems_.Get<T>();
    }

    void LogActiveSubsystems() const;

private:
    Engine& engine_;
    int id_;

    SubsystemCollection subsystems_;
};