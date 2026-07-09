#include "physix2d/Engine.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <SFML/Graphics.hpp>


namespace physix2d {

Engine::Engine(int width, int height) : renderer(width, height) {
    lastTime = std::chrono::high_resolution_clock::now();
}

Engine::Engine(int width, int height, std::string title) : renderer(width, height, title) {
    lastTime = std::chrono::high_resolution_clock::now();
}

void Engine::run() {
    // --- Record mode: deterministic fixed-step capture to PNG frames. ---
    // Env-driven so no demo changes needed:
    //   PHYSIX_RECORD=1 PHYSIX_OUTDIR=/path PHYSIX_FRAMES=300 PHYSIX_SUBSTEPS=2
    if (const char* rec = std::getenv("PHYSIX_RECORD"); rec && rec[0] == '1') {
        const char* outdir = std::getenv("PHYSIX_OUTDIR");
        int frames = std::getenv("PHYSIX_FRAMES") ? std::atoi(std::getenv("PHYSIX_FRAMES")) : 300;
        int substeps = std::getenv("PHYSIX_SUBSTEPS") ? std::atoi(std::getenv("PHYSIX_SUBSTEPS")) : 2;
        if (substeps < 1) substeps = 1;
        std::string dir = outdir ? outdir : ".";
        for (int f = 0; f < frames && renderer.isOpen(); ++f) {
            sf::Event event;
            while (renderer.pollEvent(event))
                if (event.type == sf::Event::Closed) renderer.close();
            for (int s = 0; s < substeps; ++s) {
                world.update(targetDt);
                if (userLogic) userLogic(world, renderer, targetDt);
            }
            renderer.clear();
            renderer.draw(world);
            renderer.display();
            char path[512];
            std::snprintf(path, sizeof(path), "%s/frame_%05d.png", dir.c_str(), f);
            renderer.saveScreenshot(path);
        }
        renderer.close();
        return;
    }

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
            if (userLogic) {
                userLogic(world, renderer, deltaTime); // end-user logic plugs in here
            }
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