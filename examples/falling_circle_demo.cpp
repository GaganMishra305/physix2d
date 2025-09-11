#include <iostream>
#include <random>
#include "physix2d/Vec2.h"
#include "physix2d/Body.h"
#include "physix2d/World.h"
#include "physix2d/Draw.h"

using namespace physix2d;

const int DOWNFORCE = 2.0f; // change this to change falling speed

int main() {
    std::cout << "Starting Physix2D Falling Circles Demo!" << std::endl;
    
    // Create world
    World world;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xPos(50.0f, 750.0f);
    std::uniform_real_distribution<float> radius(15.0f, 35.0f);
    
    for (int i = 0; i < 5; i++) {
        float x = xPos(gen);
        float r = radius(gen);
        Body circle(x, 50.0f + i * 50.0f, r, 1.0f); // mass = 1.0
        world.addBody(circle);
        Vec2 gravity(0.0f, DOWNFORCE); // gravity
        world.addForce(gravity);
        
        std::cout << "Created circle at x=" << x << " y=" << (50.0f + i * 50.0f) << " with radius=" << r << std::endl;
    }
    
    // Create renderer
    Draw renderer;
    renderer.run(world);
    
    std::cout << "Demo completed!" << std::endl;
    return 0;
}