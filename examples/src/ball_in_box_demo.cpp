#include <iostream>
#include <random>

#include "physix2d/Engine.h"
#include "physix2d/Vec2.h"
#include "physix2d/Body.h"
#include "physix2d/Collision.h"
#include "physix2d/Broadphase.h"
#include "physix2d/GravityForce.h"

using namespace physix2d;

// Ball-in-box demo: a pile of balls settling inside a box under gravity.
// Showcases the newer engine features -> linear damping (anti-jitter),
// Coulomb friction (balls slow/roll instead of sliding forever), and the
// uniform spatial-hash broad phase (only nearby pairs get narrow-phased).

const int   WIDTH = 1000, HEIGHT = 800, N_BALLS = 120;
const float GRAVITY = 1200.0f;
const float RESTITUTION = 0.45f;
const float CELL_SIZE = 80.0f; // ~2x max radius; good broad-phase bucket size

GravityForce* gravityForce = nullptr;

void setupWorld(World& world) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> rDist(12.0f, 30.0f);
    std::uniform_real_distribution<float> xDist(60.0f, WIDTH - 60.0f);
    std::uniform_real_distribution<float> yDist(60.0f, HEIGHT * 0.5f);

    std::vector<Body*> targets;
    for (int i = 0; i < N_BALLS; ++i) {
        float r = rDist(gen);
        Body body(xDist(gen), yDist(gen), r, r / 8.0f);
        body.setVel(Vec2::random2d() * 200.0f);
        body.setDamping(0.4f);   // gentle velocity decay -> no perpetual jitter
        body.setFriction(0.5f);  // balls grip each other and settle
        Body& ref = world.addBody(body);
        targets.push_back(&ref);
    }

    auto gravity = std::make_unique<GravityForce>(Vec2(0.0f, GRAVITY), targets);
    gravityForce = gravity.get();
    world.addForce(std::move(gravity));
}

void userLogic(World& world) {
    auto& bodies = world.getBodies();

    // Keep everyone inside the box.
    for (Body& b : bodies)
        Collision::resolveWallCollisions(b, 5, WIDTH - 5, 5, HEIGHT - 5, RESTITUTION);

    // Broad phase -> narrow phase. A couple of solver iterations tightens stacks.
    auto pairs = Broadphase::computePairs(bodies, CELL_SIZE);
    for (int iter = 0; iter < 2; ++iter)
        for (auto& p : pairs)
            Collision::resolveCircleVsCircle(bodies[p.first], bodies[p.second], RESTITUTION);
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Ball in Box");
    setupWorld(engine.getWorld());

    engine.userLogic = [](World& w, Renderer&, float) { userLogic(w); };

    engine.run();
}
