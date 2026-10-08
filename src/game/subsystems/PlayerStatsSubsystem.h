#pragma once

#include "../../subsystems/LocalPlayerSubsystem.h"

class PlayerStatsSubsystem : public LocalPlayerSubsystem {
public:
    explicit PlayerStatsSubsystem(LocalPlayer& player);

    void Initialize(SubsystemCollection& collection) override;

    void RecordAlienKills(int count);
    void RecordGameStarted();

    int GetAliensDestroyed() const;
    int GetGamesPlayed() const;

private:
    int aliensDestroyed_ = 0;
    int gamesPlayed_ = 0;
};