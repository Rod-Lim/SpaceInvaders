#include "PlayerStatsSubsystem.h"

PlayerStatsSubsystem::PlayerStatsSubsystem(LocalPlayer& player) : LocalPlayerSubsystem(player) {}

void PlayerStatsSubsystem::Initialize(SubsystemCollection&) {
    aliensDestroyed_ = 0;
    gamesPlayed_ = 0;
}

void PlayerStatsSubsystem::RecordAlienKills(int count) {
    if (count > 0) {
        aliensDestroyed_ += count;
    }
}

void PlayerStatsSubsystem::RecordGameStarted() {
    ++gamesPlayed_;
}

int PlayerStatsSubsystem::GetAliensDestroyed() const {
    return aliensDestroyed_;
}

int PlayerStatsSubsystem::GetGamesPlayed() const {
    return gamesPlayed_;
}