#pragma once

#include "physix2d/World.h"
#include "physix2d/Renderer.h"
#include <functional>
#include <chrono>
#include <string>

namespace physix2d {

class Engine {
private:
    World world;
    Renderer renderer;
    std::chrono::high_resolution_clock::time_point lastTime;
    const float targetDt = 1.0f / 60.0f;
    float accumulator = 0.0f;

public:
    Engine(int width, int height);
    Engine(int width, int height, std::string title);
    World& getWorld();
    Renderer& getRenderer();
    std::function<void(World&, Renderer&)> userLogic;

    
    // runs the engine
    void run();
};

}