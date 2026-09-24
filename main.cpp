#include "raylib.h"

/* ----- CONSTANTES -----  */
constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

constexpr int MAX_BULLETS = 10;

constexpr int ALIEN_ROWS = 5;
constexpr int ALIEN_COLS = 11;
constexpr int MAX_ALIENS = ALIEN_ROWS * ALIEN_COLS;

/* ----- STRUCTURES & ENUMS -----  */
struct Player {
    Vector2 position;
    Vector2 size;
    float speed;
    float fireCooldown;
};

struct InputState {
    bool moveLeft;
    bool moveRight;
    bool shoot;
};

struct Bullet {
    Vector2 position;
    Vector2 size;
    bool active;
};

struct Alien {
    Vector2 position;
    Vector2 size;
    bool alive;
};

struct Fleet {
    Alien aliens[MAX_ALIENS];
    float stepTimer;
    int direction;
};

enum class GameStatus {
    Playing,
    Won,
    Lost
};

/* ----- FONCTIONS UTILITAIRES -----  */
InputState ReadInput() {
    return {
        .moveLeft = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) || IsKeyDown(KEY_Q),
        .moveRight = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D),
        .shoot = IsKeyDown(KEY_SPACE)
    };
}

static void InitFleet(Fleet &fleet) {
    constexpr float ALIEN_WIDTH = 40.0f;
    constexpr float ALIEN_HEIGHT = 24.0f;
    constexpr float COLUMN_SPACING = 18.0f;
    constexpr float ROW_SPACING = 24.0f;
    constexpr float START_Y = 80.0f;

    constexpr float GRID_WIDTH = ALIEN_COLS * ALIEN_WIDTH + (ALIEN_COLS - 1) * COLUMN_SPACING;

    constexpr float START_X = (SCREEN_WIDTH - GRID_WIDTH) / 2.0f;

    for (int row = 0; row < ALIEN_ROWS; ++row) {
        for (int col = 0; col < ALIEN_COLS; ++col) {
            const int index = row * ALIEN_COLS + col;

            fleet.aliens[index] = {
                .position = {
                    START_X + col * (ALIEN_WIDTH + COLUMN_SPACING),
                    START_Y + row * (ALIEN_HEIGHT + ROW_SPACING)
                },
                .size = {ALIEN_WIDTH, ALIEN_HEIGHT},
                .alive = true
            };
        }
    }
    fleet.stepTimer = 0.0f;
    fleet.direction = 1;
}

static int CountLivingAliens(const Alien aliens[]) {
    int count = 0;

    for (int i = 0; i < MAX_ALIENS; ++i) {
        if (aliens[i].alive) {
            ++count;
        }
    }

    return count;
}

static GameStatus CheckGameStatus(const Player &player, const Alien aliens[]) {
    if (CountLivingAliens(aliens) == 0) {
        return GameStatus::Won;
    }

    for (int i = 0; i < MAX_ALIENS; ++i) {
        if (aliens[i].alive &&
            aliens[i].position.y + aliens[i].size.y >= player.position.y) {
            return GameStatus::Lost;
        }
    }

    return GameStatus::Playing;
}

/* ----- FONCTIONS D'UPDATE -----  */
static void UpdatePlayer(Player &player, const InputState &input, Bullet bullets[], const float dt, const Sound& shoot) {
    float direction = 0.0f;
    if (input.moveLeft) {
        direction -= 1.0f;
    }
    if (input.moveRight) {
        direction += 1.0f;
    }
    player.position.x += direction * player.speed * dt; // bouger le joueur

    if (player.position.x > SCREEN_WIDTH - player.size.x) {
        // empêcher de sortir de l'écran
        player.position.x = SCREEN_WIDTH - player.size.x;
    } else if (player.position.x < 0.0f) {
        player.position.x = 0.0f;
    }

    player.fireCooldown -= dt;
    if (input.shoot && player.fireCooldown <= 0.0f) {
        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (bullets[i].active) {
                continue;
            }

            bullets[i].size = {4.0f, 12.0f};
            bullets[i].position = {
                player.position.x + (player.size.x - bullets[i].size.x) / 2.0f,
                player.position.y - bullets[i].size.y
            };
            bullets[i].active = true;
            PlaySound(shoot);

            player.fireCooldown = 0.25f;
            break;
        }
    }
}

static void UpdateBullets(Bullet bullets[], const float dt) {
    constexpr float BULLET_SPEED = 500.0f;

    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!bullets[i].active) {
            continue;
        }

        bullets[i].position.y -= BULLET_SPEED * dt;

        if (bullets[i].position.y + bullets[i].size.y < 0.0f) {
            bullets[i].active = false;
        }
    }
}

static void UpdateFleet(Fleet &fleet, const float dt, const Sound& step) {
    constexpr float EDGE_MARGIN = 20.0f;
    constexpr float STEP_DISTANCE = 10.0f;
    constexpr float DESCENT_DISTANCE = 18.0f;

    const int livingAliens = CountLivingAliens(fleet.aliens);
    if (livingAliens == 0) {
        return;
    }

    const float stepInterval = 0.05f + 0.45f * static_cast<float>(livingAliens) / MAX_ALIENS;

    fleet.stepTimer += dt;
    if (fleet.stepTimer < stepInterval) {
        return;
    }

    fleet.stepTimer -= stepInterval;

    bool reachesEdge = false;
    for (int i = 0; i < MAX_ALIENS; ++i) {
        if (!fleet.aliens[i].alive) {
            continue;
        }

        const float nextX =
                fleet.aliens[i].position.x + fleet.direction * STEP_DISTANCE;

        if (nextX < 0.0f + EDGE_MARGIN || nextX + fleet.aliens[i].size.x > SCREEN_WIDTH - EDGE_MARGIN) {
            reachesEdge = true;
            break;
        }
    }

    if (reachesEdge) {
        fleet.direction = -fleet.direction;
    }

    for (int i = 0; i < MAX_ALIENS; ++i) {
        if (!fleet.aliens[i].alive) {
            continue;
        }

        if (reachesEdge) {
            fleet.aliens[i].position.y += DESCENT_DISTANCE;
        } else {
            fleet.aliens[i].position.x += fleet.direction * STEP_DISTANCE;
        }
    }
    PlaySound(step);
}

static void UpdateCollisions(Bullet bullets[], Alien aliens[], int &score, const Sound& alienDeath) {
    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; ++bulletIndex) {
        Bullet &bullet = bullets[bulletIndex];

        if (!bullet.active) {
            continue;
        }

        const Rectangle bulletBounds{
            bullet.position.x,
            bullet.position.y,
            bullet.size.x,
            bullet.size.y
        };

        for (int alienIndex = 0; alienIndex < MAX_ALIENS; ++alienIndex) {
            Alien &alien = aliens[alienIndex];

            if (!alien.alive) {
                continue;
            }

            const Rectangle alienBounds{
                alien.position.x,
                alien.position.y,
                alien.size.x,
                alien.size.y
            };

            if (CheckCollisionRecs(bulletBounds, alienBounds)) {
                bullet.active = false;
                alien.alive = false;
                score += 10;
                PlaySound(alienDeath);
                break;
            }
        }
    }
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

static void DrawHUD(const Player &player, const Bullet bullets[], const Alien aliens[], const int score) {
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

    DrawText(TextFormat("FPS : %d", GetFPS()), SCREEN_WIDTH - 100, HUD_Y, FONT_SIZE, WHITE);

    constexpr const char *LABEL = "Recharge tir : ";
    DrawText(LABEL, HUD_X, HUD_Y, FONT_SIZE, WHITE);
    DrawText(
        status,
        HUD_X + MeasureText(LABEL, FONT_SIZE),
        HUD_Y,
        FONT_SIZE,
        statusColor
    );
    DrawText(TextFormat("Balles actives : %d/%d", activeBullets, MAX_BULLETS), HUD_X, HUD_Y + 20, FONT_SIZE, WHITE);
    DrawText(TextFormat("Aliens en vie : %d/%d", livingAliens, MAX_ALIENS), HUD_X, HUD_Y + 40, FONT_SIZE, WHITE);
    DrawText(TextFormat("Score : %d", score), HUD_X, HUD_Y + 60, FONT_SIZE, WHITE);
}

/* ----- MAIN -----  */
int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    InitAudioDevice();
    SetTargetFPS(60);

    Sound shoot = LoadSound("assets/shoot.ogg");
    Sound alienDeath = LoadSound("assets/alien_death.ogg");
    Sound step = LoadSound("assets/step.ogg");

    constexpr float FIXED_DT = 1.0f / 60.0f;
    float accumulator = 0.0f;

    Player player{
        .position = {SCREEN_WIDTH / 2.0f - 25.0f, SCREEN_HEIGHT - 70.0f},
        .size = {50.0f, 25.0f},
        .speed = 350.0f,
        .fireCooldown = 0.0f
    };
    Bullet bullets[MAX_BULLETS]{};
    Fleet fleet{};
    InitFleet(fleet);
    int score = 0;
    GameStatus status = GameStatus::Playing;

    while (!WindowShouldClose()) {
        accumulator += GetFrameTime();
        const InputState input = ReadInput();

        while (accumulator >= FIXED_DT) {
            if (status == GameStatus::Playing) {
                UpdatePlayer(player, input, bullets, FIXED_DT, shoot);
                UpdateBullets(bullets, FIXED_DT);
                UpdateFleet(fleet, FIXED_DT, step);
                UpdateCollisions(bullets, fleet.aliens, score, alienDeath);
                status = CheckGameStatus(player, fleet.aliens);
            }

            accumulator -= FIXED_DT;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawPlayer(player);
        DrawBullets(bullets);
        DrawAliens(fleet.aliens);
        DrawHUD(player, bullets, fleet.aliens, score);

        if (status == GameStatus::Won) {
            DrawText("VICTOIRE !", 300, 300, 30, GREEN);
        } else if (status == GameStatus::Lost) {
            DrawText("GAME OVER", 300, 300, 30, RED);
        }

        EndDrawing();
    }


    UnloadSound(shoot);
    UnloadSound(alienDeath);
    UnloadSound(step);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
