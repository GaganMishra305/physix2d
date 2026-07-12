#include <memory>
#include <vector>
#include <random>
#include <cmath>

#include "physix2d/Engine.h"
#include "physix2d/Body.h"
#include "physix2d/PolygonShape.h"
#include "physix2d/CircleShape.h"
#include "physix2d/Collision.h"
#include "physix2d/SweepAndPrune.h"
#include "physix2d/DistanceJoint.h"
#include "physix2d/GravityForce.h"
#include "physix2d/ParticleSystem.h"

using namespace physix2d;

// Mixed scene: a wrecking ball on a joint chain swings into a box pyramid, with
// contact callbacks spraying spark particles on impact. Rigid bodies + joints +
// particles + contact events all in one showcase.

const int WIDTH = 1200, HEIGHT = 700;
const float GRAVITY = 900.0f;
const float GROUND_TOP = 640.0f;

ParticleSystem sparks;
std::mt19937 rng(2026);
Body* wrecker = nullptr;

void spawnSparks(const Vec2& at, int n) {
    std::uniform_real_distribution<float> ang(0, 6.283f), spd(60, 320), life(0.3f, 0.8f);
    for (int i = 0; i < n; ++i) {
        float a = ang(rng), s = spd(rng);
        Particle p;
        p.pos = at;
        p.vel = Vec2(std::cos(a) * s, std::sin(a) * s - 120.0f);
        p.radius = 2.5f;
        p.maxLife = p.life = life(rng);
        p.r = 255; p.g = 180; p.b = 40;
        sparks.emit(p);
    }
}

void setup(World& world) {
    sparks.setGravity(Vec2(0, 700));
    sparks.damping = 0.02f;

    // Ground.
    world.addBody(Body(WIDTH * 0.5f, GROUND_TOP + 20.0f,
                       std::make_shared<PolygonShape>(PolygonShape::box(WIDTH * 0.5f - 20, 20)), 0));

    // Box pyramid on the right.
    std::vector<Body*> dyn;
    float hw = 24, hh = 20, px = 820, pitch = 2 * hw + 3, rowH = 2 * hh + 1;
    for (int row = 0; row < 5; ++row) {
        int count = 5 - row;
        float y = GROUND_TOP - hh - row * rowH;
        float startX = px - (count - 1) * pitch * 0.5f;
        for (int i = 0; i < count; ++i) {
            Body b(startX + i * pitch, y, std::make_shared<PolygonShape>(PolygonShape::box(hw, hh)), 1.0f);
            b.setFriction(0.6f);
            dyn.push_back(&world.addBody(b));
        }
    }

    // Wrecking ball on a distance-joint chain from a high pivot.
    Body pivot(300, 90, 6, 0);
    Body* prev = &world.addBody(pivot);
    int links = 6; float seg = 42;
    for (int i = 1; i <= links; ++i) {
        bool last = (i == links);
        float r = last ? 34.0f : 6.0f;
        float m = last ? 12.0f : 1.0f;
        Body node(300 + i * seg, 90, r, m); // laid out horizontally -> swings down
        node.setFriction(0.5f);
        Body* cur = &world.addBody(node);
        world.addJoint(std::make_unique<DistanceJoint>(prev, cur, seg));
        dyn.push_back(cur);
        if (last) wrecker = cur;
        prev = cur;
    }
    world.setJointIterations(30);
    world.addForce(std::make_unique<GravityForce>(Vec2(0, GRAVITY), dyn));

    // Spark burst whenever the wrecking ball starts touching something.
    world.onBeginContact = [](Body& a, Body& b) {
        if (&a == wrecker || &b == wrecker) {
            Vec2 mid = (a.getPosition() + b.getPosition()) * 0.5f;
            spawnSparks(mid, 10);
        }
    };
}

void drawBody(sf::RenderWindow& win, const Body& b) {
    Shape* s = b.getShape();
    if (s && s->getType() == ShapeType::Polygon) {
        auto* poly = static_cast<PolygonShape*>(s);
        sf::ConvexShape cs; cs.setPointCount(poly->vertices.size());
        float c = std::cos(b.getAngle()), sn = std::sin(b.getAngle());
        Vec2 p = b.getPosition();
        for (size_t i = 0; i < poly->vertices.size(); ++i) {
            const Vec2& v = poly->vertices[i];
            cs.setPoint(i, sf::Vector2f(p.x + v.x*c - v.y*sn, p.y + v.x*sn + v.y*c));
        }
        cs.setFillColor(sf::Color(150, 170, 210));
        cs.setOutlineThickness(1.0f); cs.setOutlineColor(sf::Color::White);
        win.draw(cs);
    } else {
        float r = b.getRadius();
        sf::CircleShape c(r);
        c.setPosition(b.getPosition().x - r, b.getPosition().y - r);
        c.setFillColor(b.isStatic() ? sf::Color(120,120,120) : sf::Color(230, 120, 90));
        win.draw(c);
    }
}

void draw(sf::RenderWindow& win, World& world) {
    for (const auto& b : world.getBodies()) drawBody(win, b);
    for (const auto& p : sparks.particles) {
        sf::CircleShape c(p.radius);
        c.setPosition(p.pos.x - p.radius, p.pos.y - p.radius);
        c.setFillColor(sf::Color(p.r, p.g, p.b));
        win.draw(c);
    }
}

void step(World& world) {
    auto& bodies = world.getBodies();
    auto pairs = SweepAndPrune::computePairs(bodies);
    for (int it = 0; it < 12; ++it)
        for (auto& p : pairs)
            Collision::resolve(bodies[p.first], bodies[p.second], 0.1f);
    for (Body& b : bodies)
        Collision::resolveWallCollisions(b, 5, WIDTH - 5, 5, HEIGHT - 5, 0.2f);
    sparks.step(1.0f / 60.0f);
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Mixed Scene");
    setup(engine.getWorld());
    engine.userLogic = [](World& w, Renderer&, float) { step(w); };
    engine.getRenderer().setDrawCallback(draw);
    engine.run();
}
