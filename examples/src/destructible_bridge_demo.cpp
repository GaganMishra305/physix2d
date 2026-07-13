#include <memory>
#include <vector>
#include <cmath>

#include "physix2d/Engine.h"
#include "physix2d/Body.h"
#include "physix2d/PolygonShape.h"
#include "physix2d/Collision.h"
#include "physix2d/SweepAndPrune.h"
#include "physix2d/DistanceJoint.h"
#include "physix2d/GravityForce.h"

using namespace physix2d;

// Destructible bridge: a chain of planks held by distance-joint links anchored
// at both ends. A heavy ball is dropped on it; links that stretch past a break
// threshold SNAP, and the bridge gives way. Joints are managed here (not in
// World) so we can remove them at runtime.

const int WIDTH = 1200, HEIGHT = 700;
const float GRAVITY = 900.0f;
const int N_PLANKS = 16;
const float BREAK_FACTOR = 1.9f; // link snaps when stretched past rest * this

struct Link { int a, b; float rest; bool broken = false; };

std::vector<Body*> nodes;
std::vector<Link> links;
Body* ball = nullptr;

void setup(World& world) {
    float y = 260.0f;
    float x0 = 200.0f, x1 = 1000.0f;
    float dx = (x1 - x0) / N_PLANKS;

    std::vector<Body*> dyn;
    for (int i = 0; i <= N_PLANKS; ++i) {
        bool anchor = (i == 0 || i == N_PLANKS);
        Body plank(x0 + i * dx, y,
                   std::make_shared<PolygonShape>(PolygonShape::box(dx * 0.5f - 1, 6)),
                   anchor ? 0.0f : 1.0f);
        plank.setFriction(0.6f);
        Body* ref = &world.addBody(plank);
        nodes.push_back(ref);
        if (!anchor) dyn.push_back(ref);
    }
    for (int i = 0; i < N_PLANKS; ++i)
        links.push_back({ i, i + 1, Vec2::distance(nodes[i]->pos, nodes[i + 1]->pos) });

    // Heavy ball dropped onto the middle of the bridge.
    Body b(x0 + (x1 - x0) * 0.5f, 60.0f, 34.0f, 40.0f);
    b.setFriction(0.5f);
    ball = &world.addBody(b);
    dyn.push_back(ball);

    world.addForce(std::make_unique<GravityForce>(Vec2(0, GRAVITY), dyn));
}

void step(World& world) {
    auto& bodies = world.getBodies();

    // Collisions (ball vs planks).
    auto pairs = SweepAndPrune::computePairs(bodies);
    for (int it = 0; it < 8; ++it)
        for (auto& p : pairs)
            Collision::resolve(bodies[p.first], bodies[p.second], 0.05f);

    // Solve + break bridge links.
    float dt = 1.0f / 60.0f;
    for (int it = 0; it < 20; ++it) {
        for (Link& L : links) {
            if (L.broken) continue;
            DistanceJoint dj(nodes[L.a], nodes[L.b], L.rest, 0.3f);
            dj.solve(dt);
        }
    }
    for (Link& L : links) {
        if (L.broken) continue;
        if (Vec2::distance(nodes[L.a]->pos, nodes[L.b]->pos) > L.rest * BREAK_FACTOR)
            L.broken = true; // SNAP
    }
}

void draw(sf::RenderWindow& win, World&) {
    // Intact links.
    for (const Link& L : links) {
        if (L.broken) continue;
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(nodes[L.a]->pos.x, nodes[L.a]->pos.y), sf::Color(180,140,90)),
            sf::Vertex(sf::Vector2f(nodes[L.b]->pos.x, nodes[L.b]->pos.y), sf::Color(180,140,90))
        };
        win.draw(line, 2, sf::Lines);
    }
    // Planks.
    for (size_t i = 0; i < nodes.size(); ++i) {
        Body* n = nodes[i];
        float r = 5.0f;
        sf::CircleShape c(r);
        c.setPosition(n->pos.x - r, n->pos.y - r);
        c.setFillColor(n->isStatic() ? sf::Color(120,120,120) : sf::Color(200,160,100));
        win.draw(c);
    }
    // Ball.
    float r = ball->getRadius();
    sf::CircleShape c(r);
    c.setPosition(ball->pos.x - r, ball->pos.y - r);
    c.setFillColor(sf::Color(220, 80, 80));
    win.draw(c);
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Destructible Bridge");
    setup(engine.getWorld());
    engine.userLogic = [](World& w, Renderer&, float) { step(w); };
    engine.getRenderer().setDrawCallback(draw);
    engine.run();
}
