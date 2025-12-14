#include <iostream>
#include <random>

#include "physix2d/Engine.h"
#include "physix2d/Vec2.h"
#include "physix2d/Body.h"
#include "physix2d/Collision.h"
#include "physix2d/GravityForce.h"

using namespace physix2d;

const int WIDTH = 1000, HEIGHT = 800;
const float GRAVITY = 2500.0f;
const float RESTITUTION = 0.8f;

GravityForce* gravityForce = nullptr; // gravity force pointer -- so we can attach newly spawned bodies

// adds body with updating required forces
void addBody(World &world) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> rDist(15.0f, 35.0f);

    float r = rDist(gen);
    Body body(WIDTH * 0.5f, 50.0f, r, r/10.0f);
    Vec2 vel = Vec2::random2d() * 1000.0f;
    vel.y = 0;
    body.setVel(vel);

    Body &ref = world.addBody(body);
    gravityForce->addBody(&ref);
}

void userLogic(World &world, Renderer &r) {
    if (r.getFrameCount() % 10 == 0) {
        addBody(world);
    }

    // check collisions with other circles
    auto& bodies = world.getBodies();
    for (Body &b : bodies) {
        Collision::resolveWallCollisions(
            b, 5, WIDTH - 5, 5, HEIGHT - 5, RESTITUTION // threshold = 5 for walls
        );
    }
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = i + 1; j < bodies.size(); ++j) {
            Collision::resolveCircleVsCircle(
                bodies[i], bodies[j], RESTITUTION
            );
        }
    }
}

int main() {
    Engine engine(WIDTH, HEIGHT);
    World& world = engine.getWorld();

    // Create gravity force (initially empty)
    auto gravity = std::make_unique<GravityForce>(
        Vec2(0.0f, GRAVITY),
        std::vector<Body*>{}
    );

    gravityForce = gravity.get();
    world.addForce(std::move(gravity));
    engine.userLogic = [](World &world, Renderer &renderer, float dt) {
        userLogic(world, renderer);
    };

    engine.run();
}


/*
USER -> provides the {World(body, forces) + LOGIC}
ENGINE runs the user's world in the user defined logic system.       
*/