#pragma once

#include "../events/MulticastDelegate.h"
#include "GameStatus.h"

class World;

struct ShotFiredEvent {
    World& world;
    int playerId;
};

struct AlienDestroyedEvent {
    World& world;
    int playerId;
};

struct FleetStepEvent {
    World& world;
};

struct GameEndedEvent {
    World& world;
    GameStatus status;
    int score;
};

class GameEvents {
public:
    MulticastDelegate<ShotFiredEvent> OnShotFired;
    MulticastDelegate<AlienDestroyedEvent> OnAlienDestroyed;
    MulticastDelegate<FleetStepEvent> OnFleetStep;
    MulticastDelegate<GameEndedEvent> OnGameEnded;
};