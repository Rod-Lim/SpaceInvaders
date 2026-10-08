#include "Engine.h"
#include <iostream>

#include "raylib.h"
#include "subsystems/InputSubsystem.h"
#include "subsystems/WindowSubsystem.h"
#include "subsystems/SaveSubsystem.h"

#include "../game/Fleet.h"
#include "../game/Bullet.h"
#include "../game/InputState.h"
#include "../game/Player.h"
#include "../game/subsystems/FleetSubsystem.h"
#include "../game/subsystems/ProjectileSubsystem.h"
#include "../game/subsystems/CollisionSubsystem.h"
#include "../game/subsystems/ScoreSubsystem.h"
#include "../game/subsystems/GameRulesSubsystem.h"
#include "../game/subsystems/PlayerInputSubsystem.h"
#include "../game/subsystems/PlayerStatsSubsystem.h"

/* ----- FONCTIONS UTILITAIRES -----  */
static int CountLivingAliens(const Alien aliens[]) {
    int count = 0;

    for (int i = 0; i < MAX_ALIENS; ++i) {
        if (aliens[i].alive) {
            ++count;
        }
    }

    return count;
}

/* ----- FONCTIONS DE DESSIN -----  */
static void DrawPlayer(const Player &player) {
    DrawRectangleV(player.position, player.size, SKYBLUE);
}

static void DrawBullets(const Bullet bullets[]) {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (bullets[i].active) {
            DrawRectangleV(bullets[i].position, bullets[i].size, YELLOW);
        }
    }
}

static void DrawAliens(const Alien aliens[]) {
    constexpr Color ROW_COLORS[ALIEN_ROWS] = {
        GREEN, ORANGE, YELLOW, DARKBLUE, RED
    };

    for (int i = 0; i < MAX_ALIENS; ++i) {
        if (!aliens[i].alive) {
            continue;
        }

        const int row = i / ALIEN_COLS;
        DrawRectangleV(
            aliens[i].position,
            aliens[i].size,
            ROW_COLORS[row]
        );
    }
}

static void DrawHUD(const Player &player, const Bullet bullets[], const Alien aliens[], const int score, WindowSubsystem& windowSubsystem) {
    constexpr int HUD_X = 5;
    constexpr int HUD_Y = 5;
    constexpr int FONT_SIZE = 15;

    const bool readyToFire = player.fireCooldown <= 0.0f;
    const char *status = readyToFire ? "OK" : "PAS OK";
    const Color statusColor = readyToFire ? GREEN : RED;

    int activeBullets = 0;
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (bullets[i].active) {
            ++activeBullets;
        }
    }

    const int livingAliens = CountLivingAliens(aliens);

    DrawText(TextFormat("FPS : %d", GetFPS()), windowSubsystem.GetWidth() - 100, HUD_Y, FONT_SIZE, WHITE);

    constexpr const char *LABEL = "Recharge tir : ";
    DrawText(LABEL, HUD_X, HUD_Y, FONT_SIZE, WHITE);
    DrawText(status, HUD_X + MeasureText(LABEL, FONT_SIZE), HUD_Y, FONT_SIZE, statusColor);
    DrawText(TextFormat("Balles actives : %d/%d", activeBullets, MAX_BULLETS), HUD_X, HUD_Y + 20, FONT_SIZE, WHITE);
    DrawText(TextFormat("Aliens en vie : %d/%d", livingAliens, MAX_ALIENS), HUD_X, HUD_Y + 40, FONT_SIZE, WHITE);
    DrawText(TextFormat("Score : %d", score), HUD_X, HUD_Y + 60, FONT_SIZE, WHITE);
}

void Engine::LoadWorld(WorldType type, int initialScore) {
    world_.reset();

    world_ = std::make_unique<World>(*this, type, initialScore);

    // if (type == WorldType::Gameplay) {
    //     for (const auto& localPlayer : localPlayers_) {
    //         auto* stats = localPlayer->GetSubsystem<PlayerStatsSubsystem>();
    //
    //         if (stats) {
    //             stats->RecordGameStarted();
    //         }
    //     }
    // }
}

void Engine::LogActiveSubsystems() const {
    std::cout << "\n===== SUBSYSTEMS ACTIFS =====\n";

    subsystems_.LogActiveSubsystems();

    for (const auto& player : localPlayers_) {
        player->LogActiveSubsystems();
    }

    if (world_) {
        world_->LogActiveSubsystems();
    } else {
        std::cout << "[World] Aucun monde actif\n";
    }

    std::cout << "============================\n\n";
}

int Engine::Run() {
    subsystems_.Create(*this, GetEngineSubsystemFactories(),"Engine");
    subsystems_.Initialize();

    localPlayers_.push_back(std::make_unique<LocalPlayer>(*this, 0));

    LoadWorld(WorldType::Gameplay);

    auto* windowSubsystem = subsystems_.Get<WindowSubsystem>();
    auto* inputSubsystem = subsystems_.Get<InputSubsystem>();
    auto* saveSubsystem = subsystems_.Get<SaveSubsystem>();

    auto* playerInputSubsystem = localPlayers_.front()->GetSubsystem<PlayerInputSubsystem>();
    auto* playerStatsSubsystem = localPlayers_.front()->GetSubsystem<PlayerStatsSubsystem>();

    auto& events = GetEvents();
    const int playerId = localPlayers_.front()->GetId();

    constexpr float FIXED_DT = 1.0f / 60.0f;
    float accumulator = 0.0f;

    while (!windowSubsystem->ShouldClose()) {
        inputSubsystem->Poll();

        if (inputSubsystem->IsPressed(KEY_F1)) {
            LogActiveSubsystems();
        }

        auto* currentRules  = world_->GetSubsystem<GameRulesSubsystem>();

        if (!currentRules->IsPlaying() && inputSubsystem->IsPressed(KEY_ENTER)) {
            const bool won = currentRules->GetStatus() == GameStatus::Won;
            const int nextScore = won
                ? world_->GetSubsystem<ScoreSubsystem>()->GetScore()
                : 0;

            LoadWorld(WorldType::Gameplay, nextScore);

            if (!won) {
                playerStatsSubsystem->RecordGameStarted();
            }

            accumulator = 0.0f;
        }

        auto* fleetSubsystem = world_->GetSubsystem<FleetSubsystem>();
        auto* projectileSubsystem = world_->GetSubsystem<ProjectileSubsystem>();
        auto* collisionSubsystem = world_->GetSubsystem<CollisionSubsystem>();
        auto* scoreSubsystem = world_->GetSubsystem<ScoreSubsystem>();
        auto* gameRulesSubsystem = world_->GetSubsystem<GameRulesSubsystem>();

        Player& player = world_->GetPlayer();
        Fleet& fleet = world_->GetFleet();
        Bullet* bullets = world_->GetBullets();

        const InputState input = playerInputSubsystem->ReadInputs();

        accumulator += GetFrameTime();

        while (accumulator >= FIXED_DT) {
            if (gameRulesSubsystem->IsPlaying()) {
                if (world_->UpdatePlayer(input, FIXED_DT)) {
                    events.OnShotFired.Broadcast({
                        *world_,
                        playerId
                    });
                }
                projectileSubsystem->Update(FIXED_DT);
                if (fleetSubsystem->Update(FIXED_DT)) {
                    events.OnFleetStep.Broadcast({
                        *world_
                    });
                }
                const int destroyedAliens = collisionSubsystem->Update();
                playerStatsSubsystem->RecordAlienKills(destroyedAliens);
                for (int i = 0; i < destroyedAliens; ++i) {
                    events.OnAlienDestroyed.Broadcast({
                        *world_,
                        playerId
                    });
                }
                saveSubsystem->SubmitScore(scoreSubsystem->GetScore());

                const bool wasPlaying = gameRulesSubsystem->IsPlaying();
                gameRulesSubsystem->Update(player.position.y);
                if (wasPlaying && !gameRulesSubsystem->IsPlaying()) {
                    events.OnGameEnded.Broadcast({
                        *world_,
                        gameRulesSubsystem->GetStatus(),
                        scoreSubsystem->GetScore()
                    });
                }
            }

            accumulator -= FIXED_DT;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawPlayer(player);
        DrawBullets(bullets);
        DrawAliens(fleet.aliens);
        DrawHUD(player, bullets, fleet.aliens, scoreSubsystem->GetScore(), *windowSubsystem);

        const GameStatus status = gameRulesSubsystem->GetStatus();
        if (status == GameStatus::Won) {
            DrawText("VICTOIRE !", 300, 300, 30, GREEN);
        } else if (status == GameStatus::Lost) {
            DrawText("GAME OVER", 300, 300, 30, RED);
        }

        if (status == GameStatus::Won) {
            DrawText("ENTREE : vague suivante", 250, 350, 20, WHITE);
        } else if (status == GameStatus::Lost) {
            DrawText("ENTREE : nouvelle partie", 250, 350, 20, WHITE);
        }

        // DrawText(TextFormat("Aliens détruits (session) : %d", playerStatsSubsystem->GetAliensDestroyed()), 5, 80, 15, WHITE);
        // DrawText(TextFormat("Rounds joués : %d", playerStatsSubsystem->GetGamesPlayed()), 5, 100, 15, WHITE);
        DrawText(TextFormat("Record : %d", saveSubsystem->GetBestScore()), 5,85,15, WHITE);
        EndDrawing();
    }

    world_.reset();
    localPlayers_.clear();
    subsystems_.Deinitialize();
    return 0;
}
