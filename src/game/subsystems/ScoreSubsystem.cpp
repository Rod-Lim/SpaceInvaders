#include "ScoreSubsystem.h"

#include "../World.h"
#include "../../engine/Engine.h"

ScoreSubsystem::ScoreSubsystem(World& world) : WorldSubsystem(world) {}

bool ScoreSubsystem::ShouldCreateSubsystem(const void*) const {
    return GetWorld().GetType() == WorldType::Gameplay;
}

void ScoreSubsystem::Initialize(SubsystemCollection&) {
    score_ = GetWorld().GetInitialScore();

    auto& events = GetWorld().GetEngine().GetEvents();

    alienHandle_ = events.OnAlienDestroyed.Subscribe<ScoreSubsystem, &ScoreSubsystem::HandleAlienDestroyed>(*this);
}

void ScoreSubsystem::Deinitialize() {
    auto& events = GetWorld().GetEngine().GetEvents();

    events.OnAlienDestroyed.Unsubscribe(alienHandle_);
    alienHandle_ = 0;
}

void ScoreSubsystem::HandleAlienDestroyed(const AlienDestroyedEvent& event) {
    if (&event.world != &GetWorld()) {
        return;
    }

    score_ += PointsPerAlien;
}

int ScoreSubsystem::GetScore() const {
    return score_;
}