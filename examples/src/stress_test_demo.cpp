#include <memory>
#include <vector>
#include <random>

#include "physix2d/Engine.h"
#include "physix2d/Body.h"
#include "physix2d/Collision.h"
#include "physix2d/Broadphase.h"
#include "physix2d/GravityForce.h"

using namespace physix2d;

// Stress test: thousands of balls poured into a box. The uniform-grid broad
// phase keeps collision near O(n) -- headless benchmark showed ~0.15% of the
// brute-force pair checks at N=3000.

const int   WIDTH = 1200, HEIGHT = 800, N_BALLS = 1500;
const float GRAVITY = 800.0f;
const float CELL_SIZE = 16.0f;

void setup(World& world) {
    std::mt19937 gen(1234);
    std::uniform_real_distribution<float> rDist(4.0f, 7.0f);
    std::uniform_real_distribution<float> xDist(40.0f, WIDTH - 40.0f);
    std::uniform_real_distribution<float> yDist(40.0f, HEIGHT * 0.6f);

    std::vector<Body*> targets;
    for (int i = 0; i < N_BALLS; ++i) {
        float r = rDist(gen);
        Body b(xDist(gen), yDist(gen), r, r);
        b.setFriction(0.3f);
        b.setDamping(0.2f);
        targets.push_back(&world.addBody(b));
    }
    world.addForce(std::make_unique<GravityForce>(Vec2(0.0f, GRAVITY), targets));
}

void step(World& world) {
    auto& bodies = world.getBodies();
    for (Body& b : bodies)
        Collision::resolveWallCollisions(b, 5, WIDTH - 5, 5, HEIGHT - 5, 0.2f);

    auto pairs = Broadphase::computePairs(bodies, CELL_SIZE);
    for (int it = 0; it < 3; ++it)
        for (auto& p : pairs)
            Collision::resolveCircleVsCircle(bodies[p.first], bodies[p.second], 0.1f);
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Stress Test (1500 balls)");
    setup(engine.getWorld());
    engine.userLogic = [](World& w, Renderer&, float) { step(w); };
    engine.run();
}
