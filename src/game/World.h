#pragma once

#include "../subsystems/SubsystemCollection.h"
#include "Fleet.h"
#include "Bullet.h"
#include "Player.h"
#include "InputState.h"

class Engine;

enum class WorldType {
    MainMenu,
    Gameplay
};

class World {
public:
    World(Engine& engine, WorldType type, int initialScore = 0);
    ~World();

    World(World&&) = delete;
    World& operator=(World&&) = delete;

    WorldType GetType() const;

    Engine& GetEngine();

    template<typename T>
    T* GetSubsystem() {
        return subsystems_.Get<T>();
    }

    Fleet& GetFleet();
    const Fleet& GetFleet() const;

    Bullet* GetBullets();
    const Bullet* GetBullets() const;

    Player& GetPlayer();
    const Player& GetPlayer() const;

    bool UpdatePlayer(const InputState& input, float deltaTime);

    int GetInitialScore() const;

    void LogActiveSubsystems() const;

private:
    Engine& engine_;
    WorldType type_;

    Fleet fleet_{};
    Bullet bullets_[MAX_BULLETS]{};

    Player player_{};

    int initialScore_ = 0;

    SubsystemCollection subsystems_;
};