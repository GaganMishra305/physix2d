#include <memory>
#include <vector>
#include <limits>

#include "physix2d/Engine.h"
#include "physix2d/Body.h"
#include "physix2d/PolygonShape.h"
#include "physix2d/Collision.h"
#include "physix2d/SweepAndPrune.h"
#include "physix2d/CCD.h"
#include "physix2d/GravityForce.h"

using namespace physix2d;

// Bullet through stack: a small, very fast circle is fired at a wall of boxes.
// A discrete solver would let it tunnel straight through in a single step; the
// CCD time-of-impact query catches it at the surface so it actually hits.

const int   WIDTH = 1200, HEIGHT = 700;
const float GRAVITY = 700.0f;
const float BOX_HW = 18.0f, BOX_HH = 22.0f;
const int   STACK_H = 8;
const float GROUND_TOP = 640.0f;
const float BULLET_SPEED = 4000.0f;

Body* bullet = nullptr;

void setup(World& world) {
    // Ground.
    world.addBody(Body(WIDTH * 0.5f, GROUND_TOP + 20.0f,
                       std::make_shared<PolygonShape>(PolygonShape::box(WIDTH * 0.5f - 20, 20)), 0));

    // A vertical wall/stack of boxes.
    std::vector<Body*> targets;
    float wallX = 820.0f;
    for (int i = 0; i < STACK_H; ++i) {
        Body b(wallX, GROUND_TOP - BOX_HH - i * (2 * BOX_HH + 1),
               std::make_shared<PolygonShape>(PolygonShape::box(BOX_HW, BOX_HH)), 1.0f);
        b.setFriction(0.5f);
        targets.push_back(&world.addBody(b));
    }

    // Fast bullet aimed at the middle of the stack. No gravity -> flat trajectory.
    Body b(60.0f, GROUND_TOP - BOX_HH - 3 * (2 * BOX_HH + 1), 5.0f, 3.0f);
    b.setVel(Vec2(BULLET_SPEED, 0.0f));
    b.allowSleep = false;
    bullet = &world.addBody(b);

    world.addForce(std::make_unique<GravityForce>(Vec2(0.0f, GRAVITY), targets));
}

void step(World& world) {
    auto& bodies = world.getBodies();

    // --- CCD for the bullet: catch it at the earliest time-of-impact ---
    if (bullet) {
        float best = CCD::NO_HIT;
        for (auto& b : bodies) {
            if (&b == bullet) continue;
            float toi = CCD::NO_HIT;
            if (b.getShape() && b.getShape()->getType() == ShapeType::Polygon)
                toi = CCD::circleVsPolygon(*bullet, b);
            else
                toi = CCD::circleVsCircle(*bullet, b);
            if (toi < best) best = toi;
        }
        if (best <= 1.0f) {
            Vec2 sweep = bullet->pos - bullet->prev_pos;
            bullet->pos = bullet->prev_pos + sweep * best; // rewind to surface
        }
    }

    // --- discrete broad + narrow phase (sweep-and-prune) ---
    auto pairs = SweepAndPrune::computePairs(bodies);
    for (int it = 0; it < 12; ++it)
        for (auto& p : pairs)
            Collision::resolve(bodies[p.first], bodies[p.second], 0.1f);

    for (Body& b : bodies)
        Collision::resolveWallCollisions(b, 5, WIDTH - 5, 5, HEIGHT - 5, 0.2f);
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Bullet Through Stack (CCD)");
    setup(engine.getWorld());
    engine.userLogic = [](World& w, Renderer&, float) { step(w); };
    engine.run();
}
