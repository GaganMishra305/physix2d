cmake_minimum_required(VERSION 3.15)
project(Physix2D LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# The library uses SFML for its Renderer, so it declares that dependency
# itself (only the modules we use -- no audio => no openal).
find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML REQUIRED sfml-graphics>=2.4 sfml-window sfml-system)

# Library target
add_library(physix2d
    src/Vec2.cpp
    src/Body.cpp
    src/CircleShape.cpp
    src/PolygonShape.cpp
    src/World.cpp
    src/Renderer.cpp
    src/Engine.cpp
    src/Collision.cpp
    src/Manifold.cpp
    src/Broadphase.cpp
    src/SweepAndPrune.cpp
    src/CCD.cpp
    src/ParticleSystem.cpp
    src/DistanceJoint.cpp
    src/RevoluteJoint.cpp
    src/RopeJoint.cpp
    src/Island.cpp
    src/GravityForce.cpp
)

target_include_directories(physix2d PUBLIC include ${SFML_INCLUDE_DIRS})
target_link_directories(physix2d PUBLIC ${SFML_LIBRARY_DIRS})
target_link_libraries(physix2d PUBLIC ${SFML_LIBRARIES})
target_compile_options(physix2d PUBLIC ${SFML_CFLAGS_OTHER})
