#include "GameRulesSubsystem.h"

#include "../World.h"
#include "FleetSubsystem.h"
#include "../../subsystems/SubsystemCollection.h"

GameRulesSubsystem::GameRulesSubsystem(World& world) : WorldSubsystem(world) {}

bool GameRulesSubsystem::ShouldCreateSubsystem(const void*) const {
    return GetWorld().GetType() == WorldType::Gameplay;
}

void GameRulesSubsystem::Initialize(SubsystemCollection& collection) {
    fleetSubsystem_ = &collection.InitializeDependency<FleetSubsystem>();

    status_ = GameStatus::Playing;
}

void GameRulesSubsystem::Deinitialize() {
    fleetSubsystem_ = nullptr;
}

void GameRulesSubsystem::Update(float playerY) {
    if (!IsPlaying()) {
        return;
    }

    if (fleetSubsystem_->CountLivingAliens() == 0) {
        status_ = GameStatus::Won;
        return;
    }

    const Fleet& fleet = GetWorld().GetFleet();

    for (const Alien& alien : fleet.aliens) {
        if (alien.alive && alien.position.y + alien.size.y >= playerY) {
            status_ = GameStatus::Lost;
            return;
        }
    }
}

GameStatus GameRulesSubsystem::GetStatus() const {
    return status_;
}

bool GameRulesSubsystem::IsPlaying() const {
    return status_ == GameStatus::Playing;
}