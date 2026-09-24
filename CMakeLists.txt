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

add_executable(TP1___Space_Invaders main.cpp)
set_target_properties(TP1___Space_Invaders PROPERTIES CXX_SCAN_FOR_MODULES OFF)
target_link_libraries(TP1___Space_Invaders PRIVATE raylib)

add_custom_command(TARGET TP1___Space_Invaders POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:TP1___Space_Invaders>/assets"
)