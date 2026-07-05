#include <iostream>
#include <memory>
#include <vector>

#include "physix2d/Engine.h"
#include "physix2d/Body.h"
#include "physix2d/PolygonShape.h"
#include "physix2d/Collision.h"
#include "physix2d/Broadphase.h"
#include "physix2d/GravityForce.h"

using namespace physix2d;

// Domino chain: a row of standing boxes; the first gets a nudge and topples
// the rest. Showcases polygon collision + rotational dynamics + friction.

const int   WIDTH = 1200, HEIGHT = 700;
const float GRAVITY = 900.0f;
const int   N_DOMINOES = 14;
const float DOM_HW = 6.0f, DOM_HH = 42.0f;   // half-width, half-height
const float SPACING = 55.0f;
const float GROUND_TOP = 640.0f;
const float CELL_SIZE = 110.0f;

void setup(World& world) {
    // Static ground.
    world.addBody(Body(WIDTH * 0.5f, GROUND_TOP + 20.0f,
                       std::make_shared<PolygonShape>(PolygonShape::box(WIDTH * 0.5f - 20, 20)), 0));

    std::vector<Body*> targets;
    float startX = 180.0f;
    for (int i = 0; i < N_DOMINOES; ++i) {
        float x = startX + i * SPACING;
        Body d(x, GROUND_TOP - DOM_HH,
               std::make_shared<PolygonShape>(PolygonShape::box(DOM_HW, DOM_HH)), 1.0f);
        d.setFriction(0.5f);
        Body& ref = world.addBody(d);
        targets.push_back(&ref);
    }

    // Nudge the first domino so it topples toward the next.
    targets[0]->angularVel = 3.5f;

    world.addForce(std::make_unique<GravityForce>(Vec2(0.0f, GRAVITY), targets));
}

void step(World& world) {
    auto& bodies = world.getBodies();
    auto pairs = Broadphase::computePairs(bodies, CELL_SIZE);
    // Several iterations tighten stacks/contacts.
    for (int it = 0; it < 12; ++it)
        for (auto& p : pairs)
            Collision::resolve(bodies[p.first], bodies[p.second], 0.05f);
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Domino Chain");
    setup(engine.getWorld());
    engine.userLogic = [](World& w, Renderer&, float) { step(w); };
    engine.run();
}
