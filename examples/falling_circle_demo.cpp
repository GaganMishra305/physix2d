#include <iostream>
#include <random>
#include <chrono>
#include "physix2d/Vec2.h"
#include "physix2d/Body.h"
#include "physix2d/World.h"
#include "physix2d/Renderer.h"

using namespace physix2d;

const int DOWNFORCE = 1.5f; // change this to change falling speed

int main() {
    std::cout << "Starting Physix2D Falling Circles Demo!" << std::endl;
    
    // Create world
    World world;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xPos(50.0f, 750.0f);
    std::uniform_real_distribution<float> radius(15.0f, 35.0f);
    
    for (int i = 0; i < 10; i++) {
        float x = xPos(gen);
        float r = radius(gen);
        Body circle(x, 50.0f + i * 50.0f, r, r/50.0f); // mass = 1.0
        world.addBody(circle);
        Vec2 gravity(0.0f, DOWNFORCE); // gravity
        world.addForce(gravity);
        
        std::cout << "Created circle at x=" << x << " y=" << (50.0f + i * 50.0f) << " with radius=" << r << std::endl;
    }
    
    Renderer renderer(800, 600);
    // timing for physics updates
    auto lastTime = std::chrono::high_resolution_clock::now();
    const float targetDt = 1.0f / 60.0f; // Target 60 FPS physics
    float accumulator = 0.0f;
    
    // main loop
    while (renderer.isOpen()) {
        // handle events
        sf::Event event;
        while (renderer.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                renderer.close();
            }
        }
        
        // delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        deltaTime = std::min(deltaTime, 0.05f);
        accumulator += deltaTime;
        while (accumulator >= targetDt) {
            world.update(targetDt);
            accumulator -= targetDt;
        }
        
        // rendering
        renderer.clear();
        renderer.draw(world);
        renderer.display();
    }

    std::cout << "Demo completed!" << std::endl;
    return 0;
}