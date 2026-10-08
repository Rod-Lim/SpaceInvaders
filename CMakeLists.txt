cmake_minimum_required(VERSION 3.20)
project(TP1___Space_Invaders LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
)

# Raylib 5.0 déclare une compatibilité CMake trop ancienne pour CMake 4.x.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
FetchContent_MakeAvailable(raylib)

add_executable(
        TP1___Space_Invaders
        src/main.cpp
        src/subsystems/Subsystem.h
        src/subsystems/EngineSubsystem.cpp
        src/subsystems/EngineSubsystem.h
        src/engine/Engine.cpp
        src/engine/Engine.h
        src/subsystems/WorldSubsystem.cpp
        src/subsystems/WorldSubsystem.h
        src/subsystems/LocalPlayerSubsystem.cpp
        src/subsystems/LocalPlayerSubsystem.h
        src/subsystems/SubsystemRegistration.h
        src/subsystems/SubsystemRegistry.cpp
        src/subsystems/SubsystemRegistry.h
        src/game/subsystems/FleetSubsystem.cpp
        src/game/subsystems/FleetSubsystem.h
        src/game/World.cpp
        src/game/World.h
        src/engine/subsystems/WindowSubsystem.cpp
        src/engine/subsystems/WindowSubsystem.h
        src/engine/subsystems/AudioSubsystem.cpp
        src/engine/subsystems/AudioSubsystem.h
        src/subsystems/SubsystemCollection.cpp
        src/subsystems/SubsystemCollection.h
        src/engine/subsystems/AssetSubsystem.cpp
        src/engine/subsystems/AssetSubsystem.h
        src/engine/subsystems/InputSubsystem.cpp
        src/engine/subsystems/InputSubsystem.h
        src/engine/subsystems/SaveSubsystem.cpp
        src/engine/subsystems/SaveSubsystem.h
        src/game/Fleet.h
        src/game/subsystems/ProjectileSubsystem.cpp
        src/game/subsystems/ProjectileSubsystem.h
        src/game/Bullet.h
        src/game/subsystems/CollisionSubsystem.cpp
        src/game/subsystems/CollisionSubsystem.h
        src/game/subsystems/ScoreSubsystem.cpp
        src/game/subsystems/ScoreSubsystem.h
        src/game/GameStatus.h
        src/game/subsystems/GameRulesSubsystem.cpp
        src/game/subsystems/GameRulesSubsystem.h
        src/game/LocalPlayer.cpp
        src/game/LocalPlayer.h
        src/game/subsystems/PlayerInputSubsystem.cpp
        src/game/subsystems/PlayerInputSubsystem.h
        src/game/InputState.h
        src/game/subsystems/PlayerStatsSubsystem.cpp
        src/game/subsystems/PlayerStatsSubsystem.h
        src/events/MulticastDelegate.h
        src/game/GameEvents.h
        src/game/Player.h
)
set_target_properties(TP1___Space_Invaders PROPERTIES CXX_SCAN_FOR_MODULES OFF)
target_link_libraries(TP1___Space_Invaders PRIVATE raylib)

add_custom_command(TARGET TP1___Space_Invaders POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:TP1___Space_Invaders>/assets"
)