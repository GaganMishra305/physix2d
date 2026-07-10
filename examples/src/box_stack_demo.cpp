#include <memory>
#include <vector>

#include "physix2d/Engine.h"
#include "physix2d/Body.h"
#include "physix2d/PolygonShape.h"
#include "physix2d/Collision.h"
#include "physix2d/Broadphase.h"
#include "physix2d/GravityForce.h"

using namespace physix2d;

// Box stack: a pyramid of boxes settling under gravity. Stress-tests the
// polygon solver's stability with resting contacts and friction.

const int   WIDTH = 1200, HEIGHT = 700;
const float GRAVITY = 900.0f;
const float BOX_HW = 26.0f, BOX_HH = 20.0f;
const float GROUND_TOP = 650.0f;
const int   BASE_COUNT = 6;
const float CELL_SIZE = 90.0f;

void setup(World& world) {
    world.addBody(Body(WIDTH * 0.5f, GROUND_TOP + 20.0f,
                       std::make_shared<PolygonShape>(PolygonShape::box(WIDTH * 0.5f - 20, 20)), 0));

    std::vector<Body*> targets;
    float rowH = 2.0f * BOX_HH + 1.0f;
    float pitch = 2.0f * BOX_HW + 4.0f;
    for (int row = 0; row < BASE_COUNT; ++row) {
        int count = BASE_COUNT - row;
        float y = GROUND_TOP - BOX_HH - row * rowH;
        float rowWidth = (count - 1) * pitch;
        float startX = WIDTH * 0.5f - rowWidth * 0.5f;
        for (int i = 0; i < count; ++i) {
            Body b(startX + i * pitch, y,
                   std::make_shared<PolygonShape>(PolygonShape::box(BOX_HW, BOX_HH)), 1.0f);
            b.setFriction(0.6f);
            targets.push_back(&world.addBody(b));
        }
    }

    world.addForce(std::make_unique<GravityForce>(Vec2(0.0f, GRAVITY), targets));
}

void step(World& world) {
    auto& bodies = world.getBodies();
    auto pairs = Broadphase::computePairs(bodies, CELL_SIZE);
    for (int it = 0; it < 14; ++it)
        for (auto& p : pairs)
            Collision::resolve(bodies[p.first], bodies[p.second], 0.0f);
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Box Stack");
    setup(engine.getWorld());
    engine.userLogic = [](World& w, Renderer&, float) { step(w); };
    engine.run();
}
