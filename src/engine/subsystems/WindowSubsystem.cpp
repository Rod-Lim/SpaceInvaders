#include "WindowSubsystem.h"
#include "raylib.h"

WindowSubsystem::WindowSubsystem(Engine& engine) : EngineSubsystem(engine) {}

void WindowSubsystem::Initialize(SubsystemCollection&) {
    ::InitWindow(width_, height_, "Space Invaders");
    ::SetTargetFPS(60);
}

void WindowSubsystem::Deinitialize() {
    ::CloseWindow();
}

bool WindowSubsystem::ShouldClose() const {
    return ::WindowShouldClose();
}

int WindowSubsystem::GetWidth() const {
    return width_;
}

int WindowSubsystem::GetHeight() const {
    return height_;
}