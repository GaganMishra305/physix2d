#include <memory>
#include <vector>

#include "physix2d/Engine.h"
#include "physix2d/Body.h"
#include "physix2d/PolygonShape.h"
#include "physix2d/RevoluteJoint.h"
#include "physix2d/Collision.h"
#include "physix2d/GravityForce.h"

using namespace physix2d;

// Ragdoll: a figure of boxes (torso, head, arms, legs) connected by
// RevoluteJoints at the neck/shoulders/hips, dropped onto the ground to flop.
// Self-collision between parts is intentionally disabled (no collision
// filtering yet -> that's Phase 4), so only part-vs-ground is resolved.

const int   WIDTH = 1000, HEIGHT = 800;
const float GRAVITY = 900.0f;

std::vector<Body*> parts;
Body* ground = nullptr;

static Body* addPart(World& w, float x, float y, float hw, float hh, std::vector<Body*>& grav) {
    Body b(x, y, std::make_shared<PolygonShape>(PolygonShape::box(hw, hh)), 1.0f);
    b.setFriction(0.5f);
    b.setDamping(0.3f);
    Body& r = w.addBody(b);
    parts.push_back(&r);
    grav.push_back(&r);
    return &r;
}

void setup(World& world) {
    Body gb(WIDTH * 0.5f, 720.0f, std::make_shared<PolygonShape>(PolygonShape::box(480, 20)), 0);
    gb.setFriction(0.6f);
    ground = &world.addBody(gb);

    std::vector<Body*> grav;
    float X = 500.0f, Y = 280.0f;
    Body* torso = addPart(world, X, Y, 12, 45, grav);
    Body* head  = addPart(world, X, Y - 45 - 16, 16, 16, grav);
    Body* lArm  = addPart(world, X - 12 - 30, Y - 35, 30, 7, grav);
    Body* rArm  = addPart(world, X + 12 + 30, Y - 35, 30, 7, grav);
    Body* lLeg  = addPart(world, X - 6, Y + 45 + 40, 8, 40, grav);
    Body* rLeg  = addPart(world, X + 6, Y + 45 + 40, 8, 40, grav);

    world.addJoint(std::make_unique<RevoluteJoint>(torso, head, Vec2(X, Y - 45)));
    world.addJoint(std::make_unique<RevoluteJoint>(torso, lArm, Vec2(X - 12, Y - 35)));
    world.addJoint(std::make_unique<RevoluteJoint>(torso, rArm, Vec2(X + 12, Y - 35)));
    world.addJoint(std::make_unique<RevoluteJoint>(torso, lLeg, Vec2(X - 6, Y + 45)));
    world.addJoint(std::make_unique<RevoluteJoint>(torso, rLeg, Vec2(X + 6, Y + 45)));

    torso->angle = 0.4f; // tilt so it flops on landing

    world.addForce(std::make_unique<GravityForce>(Vec2(0.0f, GRAVITY), grav));
    world.setJointIterations(20);
}

void step(World&) {
    // Only part-vs-ground (no self-collision without collision filtering).
    for (int it = 0; it < 10; ++it)
        for (Body* p : parts)
            Collision::resolve(*p, *ground, 0.0f);
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Ragdoll");
    setup(engine.getWorld());
    engine.userLogic = [](World& w, Renderer&, float) { step(w); };
    engine.run();
}
