#include <memory>
#include <vector>

#include "physix2d/Engine.h"
#include "physix2d/Body.h"
#include "physix2d/PolygonShape.h"
#include "physix2d/RevoluteJoint.h"
#include "physix2d/Collision.h"
#include "physix2d/Broadphase.h"
#include "physix2d/GravityForce.h"

using namespace physix2d;

// Rope bridge: a row of planks hinged end-to-end with RevoluteJoints, both
// ends pinned to static anchors. A heavy ball is dropped on it to show the
// bridge sag and recover. Showcases RevoluteJoint + polygon/circle contacts.

const int   WIDTH = 1200, HEIGHT = 700;
const float GRAVITY = 900.0f;
const int   PLANKS = 14;
const float PLANK_HW = 30.0f, PLANK_HH = 10.0f;   // thicker planks resist ball tunneling
const float ANCHOR_Y = 250.0f;
const float CELL_SIZE = 90.0f;

Body* ball = nullptr;

void setup(World& world) {
    float leftX = 180.0f;
    float plankW = PLANK_HW * 2.0f;

    // Static end anchors.
    Body la(leftX, ANCHOR_Y, 5.0f, 0.0f);
    Body ra(leftX + PLANKS * plankW, ANCHOR_Y, 5.0f, 0.0f);
    Body* laRef = &world.addBody(la);
    Body* raRef = &world.addBody(ra);

    // Planks.
    std::vector<Body*> planks;
    std::vector<Body*> gravityTargets;
    for (int i = 0; i < PLANKS; ++i) {
        float cx = leftX + PLANK_HW + i * plankW;
        Body p(cx, ANCHOR_Y, std::make_shared<PolygonShape>(PolygonShape::box(PLANK_HW, PLANK_HH)), 1.0f);
        p.setFriction(0.5f);
        Body& ref = world.addBody(p);
        planks.push_back(&ref);
        gravityTargets.push_back(&ref);
    }

    // Hinge left anchor -> first plank left end.
    world.addJoint(std::make_unique<RevoluteJoint>(laRef, planks[0], Vec2(leftX, ANCHOR_Y)));
    // Hinge consecutive planks at their shared edge.
    for (int i = 0; i < PLANKS - 1; ++i) {
        float jointX = leftX + (i + 1) * plankW;
        world.addJoint(std::make_unique<RevoluteJoint>(planks[i], planks[i + 1], Vec2(jointX, ANCHOR_Y)));
    }
    // Hinge last plank right end -> right anchor.
    world.addJoint(std::make_unique<RevoluteJoint>(planks[PLANKS - 1], raRef, Vec2(leftX + PLANKS * plankW, ANCHOR_Y)));

    // Heavy ball dropped onto the middle (start just above so it doesn't build
    // up tunneling speed against the thin bridge).
    Body b(leftX + PLANKS * plankW * 0.5f, ANCHOR_Y - 90.0f, 22.0f, 4.0f);
    b.setFriction(0.4f);
    ball = &world.addBody(b);
    gravityTargets.push_back(ball);

    world.addForce(std::make_unique<GravityForce>(Vec2(0.0f, GRAVITY), gravityTargets));
    world.setJointIterations(20);
}

void step(World& world) {
    auto& bodies = world.getBodies();
    auto pairs = Broadphase::computePairs(bodies, CELL_SIZE);
    for (int it = 0; it < 12; ++it)
        for (auto& p : pairs)
            Collision::resolve(bodies[p.first], bodies[p.second], 0.1f);
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Rope Bridge");
    setup(engine.getWorld());
    engine.userLogic = [](World& w, Renderer&, float) { step(w); };
    engine.run();
}
