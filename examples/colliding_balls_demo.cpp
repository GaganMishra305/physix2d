#include <iostream>
#include <random>
#include "physix2d/Engine.h"
#include "physix2d/Vec2.h"
#include "physix2d/Body.h"
#include "physix2d/Collision.h"

using namespace physix2d;

const int WIDTH = 1000, HEIGHT = 800, N_BALLS = 75;
const float DOWNFORCE = 5.0f;
const float RESTITUTION = 1.0f;

void addBody(World &world) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> rDist(15.0f, 35.0f);
    float r = rDist(gen);
    Body body(WIDTH * 0.5f, HEIGHT * 0.5f, r, r / 50.0f);
    Vec2 vel = Vec2::random2d();
    vel.y = 0;
    body.setVel(vel);
    world.addBody(body);
    world.addForce(Vec2(0.0f, DOWNFORCE));
}

void updateWorld(World &world, Renderer &r) {
    if(r.getFrameCount() % 100 == 0){
        addBody(world);
    }

    auto& bodies = world.getBodies();
    
    // Wall collisions with positional correction
    for (Body &b : bodies) {
        Collision::resolveWallCollisions(b, 0, WIDTH, 0, HEIGHT, RESTITUTION);
    }
    
    // Circle–circle collisions
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = i + 1; j < bodies.size(); ++j) {
            Collision::resolveCircleVsCircle(bodies[i], bodies[j], RESTITUTION);
        }
    }
}

void setupWorld(World &world) {}

int main() {
    Engine engine(WIDTH, HEIGHT);
    setupWorld(engine.getWorld());
    engine.userLogic = [](World &w, Renderer &r) { updateWorld(w, r); };
    engine.run();
}