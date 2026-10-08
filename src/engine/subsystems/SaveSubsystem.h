#pragma once

#include "../../subsystems/EngineSubsystem.h"

class SaveSubsystem : public EngineSubsystem {
public:
    explicit SaveSubsystem(Engine& engine);

    void Initialize(SubsystemCollection& collection) override;
    void Deinitialize() override;

    int GetBestScore() const;
    void SubmitScore(int score);

private:
    int bestScore_ = 0;
};