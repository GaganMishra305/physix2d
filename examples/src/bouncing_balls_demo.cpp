#include <iostream>
#include <random>
#include <chrono>

#include "physix2d/Engine.h"
#include "physix2d/Vec2.h"
#include "physix2d/Body.h"
#include "physix2d/GravityForce.h"

using namespace physix2d;

/*
CORE DEMO LOGIC IS HERE
*/
const int WIDTH = 1000, HEIGHT = 800, N_BALLS = 75;
const float DOWNFORCE = 0.0f;

void collisionLogic(World &world) {
    for (Body &b : world.getBodies()) {
        float x = b.getPosition().x;
        float y = b.getPosition().y;
        float r = b.getRadius();
        Vec2 v = b.getVel();

        if (y + r > HEIGHT || y - r < 0)
            b.setVel(Vec2(v.x, -v.y));

        if (x + r > WIDTH || x - r < 0)
            b.setVel(Vec2(-v.x, v.y));
    }
}

void setupWorld(World &world) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> rDist(15.0f, 35.0f);

    std::vector<Body*> gravityTargets;

    for (int i = 0; i < N_BALLS; ++i) {
        float r = rDist(gen);

        Body body(
            WIDTH * 0.5f,
            HEIGHT * 0.5f,
            r,
            r/500.0
        );

        body.setVel(Vec2::random2d() * 1000.0f);

        Body& ref = world.addBody(body);
        gravityTargets.push_back(&ref);
    }

    // Apply constant downforce (gravity-like)
    world.addForce(
        std::make_unique<GravityForce>(
            Vec2(0.0f, DOWNFORCE),
            gravityTargets
        )
    );
}

int main() {
    Engine engine(WIDTH, HEIGHT);

    setupWorld(engine.getWorld());

    engine.userLogic = [](World &w, Renderer &, float dt) {
        collisionLogic(w);
    };

    engine.run();
}
