#include "physix2d/Engine.h"
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>


namespace physix2d {

Engine::Engine(int width, int height) : renderer(width, height) {
    lastTime = std::chrono::high_resolution_clock::now();
}

Engine::Engine(int width, int height, std::string title) : renderer(width, height, title) {
    lastTime = std::chrono::high_resolution_clock::now();
}

void Engine::run() {
    while (renderer.isOpen()) {
        // Handle events
        sf::Event event;
        while (renderer.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                renderer.close();
            }
        }
        
        // Delta time logic
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        deltaTime = std::min(deltaTime, 0.05f);
        accumulator += deltaTime;
        
        // Fixed timestep physics update
        while (accumulator >= targetDt) {
            world.update(targetDt);
            userLogic(world, renderer); // HERE THE CORE LOGIC OF THE END USER WOULD BE PLUGGED IN FOR NOW
            accumulator -= targetDt;
        }

        // Rendering
        renderer.clear();
        renderer.draw(world);
        renderer.display();
    }
}

World& Engine::getWorld() {
    return world;
}

Renderer& Engine::getRenderer() {
    return renderer;
}

}