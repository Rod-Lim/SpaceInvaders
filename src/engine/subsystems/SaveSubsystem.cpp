#include "SaveSubsystem.h"

#include <fstream>
#include <iostream>

namespace {
    constexpr const char* SavePath = "best_score.txt";
}

SaveSubsystem::SaveSubsystem(Engine& engine) : EngineSubsystem(engine) {}

void SaveSubsystem::Initialize(SubsystemCollection&) {
    bestScore_ = 0;

    std::ifstream file(SavePath);

    // Si premier lancement ya pas le fichier
    if (!file.is_open()) {
        return;
    }

    int savedScore = 0;

    if (file >> savedScore && savedScore >= 0) {
        bestScore_ = savedScore;
    } else {
        std::cerr << "Invalid best score file: " << SavePath << '\n';
    }
}

void SaveSubsystem::Deinitialize() {
    std::ofstream file(SavePath, std::ios::trunc);

    if (!file.is_open()) {
        std::cerr << "Cannot open save file: " << SavePath << '\n';
        return;
    }

    file << bestScore_ << '\n';
    file.close();

    if (!file) {
        std::cerr << "Cannot write save file: " << SavePath << '\n';
    }
}

int SaveSubsystem::GetBestScore() const {
    return bestScore_;
}

void SaveSubsystem::SubmitScore(int score) {
    if (score > bestScore_) {
        bestScore_ = score;
    }
}