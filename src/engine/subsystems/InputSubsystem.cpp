#include "InputSubsystem.h"

#include "raylib.h"

#include "WindowSubsystem.h"
#include "../../subsystems/SubsystemCollection.h"

InputSubsystem::InputSubsystem(Engine& engine) : EngineSubsystem(engine) {}

void InputSubsystem::Initialize(SubsystemCollection& collection) {
    collection.InitializeDependency<WindowSubsystem>();

    down_.fill(false);
    pressed_.fill(false);
}

void InputSubsystem::Deinitialize() {
    down_.fill(false);
    pressed_.fill(false);
}

void InputSubsystem::Poll() {
    for (int key = 0; key < KeyCapacity; key++) {
        down_[key] = ::IsKeyDown(key);
        pressed_[key] = ::IsKeyPressed(key);
    }
}

bool InputSubsystem::IsDown(int key) const {
    if (key < 0 || key >= KeyCapacity) {
        return false;
    }

    return down_[key];
}

bool InputSubsystem::IsPressed(int key) const {
    if (key < 0 || key >= KeyCapacity) {
        return false;
    }

    return pressed_[key];
}