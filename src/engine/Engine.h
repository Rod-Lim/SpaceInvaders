#pragma once

#include "../subsystems/SubsystemCollection.h"

#include <memory>
#include <vector>
#include "../game/World.h"
#include "../game/LocalPlayer.h"
#include "../game/GameEvents.h"

class Engine {
public:
    int Run();

    template<typename T>
    T* GetSubsystem() {
        return subsystems_.Get<T>();
    }

    GameEvents& GetEvents() {
        return events_;
    }

    void LoadWorld(WorldType type, int initialScore = 0);

private:
    GameEvents events_;
    SubsystemCollection subsystems_;
    std::vector<std::unique_ptr<LocalPlayer>> localPlayers_;
    std::unique_ptr<World> world_;
};