cmake_minimum_required(VERSION 3.15)
project(Physix2D LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library target
add_library(physix2d
    src/Vec2.cpp
    src/Body.cpp
    src/World.cpp
    src/Renderer.cpp
    src/Engine.cpp
    src/Collision.cpp
    src/GravityForce.cpp
)

target_include_directories(physix2d PUBLIC include)
