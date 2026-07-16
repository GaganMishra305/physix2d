#include <cmath>

#include "physix2d/Engine.h"
#include "physix2d/SoftBody.h"

using namespace physix2d;

// clothe_simulation -- THE REDEMPTION.
//
// This file was deleted at the very start of Phase 3: the original version bolted
// positional "stick" constraints onto the engine's Euler velocity integration,
// so the constraint edits and the velocity state desynced and it blew up.
//
// This rewrite uses SoftBody -- Verlet integration (velocity IS pos - prev) plus
// position-based distance constraints -- which is the correct foundation. Same
// idea the original wanted; stable this time. A gentle wind makes it ripple.

const int WIDTH = 1000, HEIGHT = 800;
const float DT = 1.0f / 60.0f;
const int COLS = 28, ROWS = 22;

SoftBody cloth;
float t = 0.0f;

void setup() {
    cloth = SoftBody::cloth(Vec2(220, 90), 560, 460, COLS, ROWS, 1.0f);
    cloth.gravity = Vec2(0.0f, 600.0f);
    cloth.damping = 0.015f;
    cloth.iterations = 18;
    // Pin the top row (the curtain rail).
    for (int x = 0; x <= COLS; ++x)
        cloth.pin(x);
}

void step() {
    t += DT;
    cloth.wind = Vec2(220.0f * std::sin(t * 1.3f) + 120.0f, 40.0f * std::sin(t * 3.0f));
    cloth.step(DT);
}

void draw(sf::RenderWindow& window, World&) {
    auto idx = [](int x, int y) { return y * (COLS + 1) + x; };
    // Filled shaded mesh.
    for (int y = 0; y < ROWS; ++y)
        for (int x = 0; x < COLS; ++x) {
            Vec2 a = cloth.points[idx(x, y)].pos;
            Vec2 b = cloth.points[idx(x + 1, y)].pos;
            Vec2 c = cloth.points[idx(x + 1, y + 1)].pos;
            Vec2 d = cloth.points[idx(x, y + 1)].pos;
            // Shade by how stretched/tilted the cell is (fakes lighting).
            float shade = 150.0f + 80.0f * std::sin((x + y) * 0.5f + t);
            unsigned char s = static_cast<unsigned char>(std::fmin(255.0f, std::fmax(60.0f, shade)));
            sf::Color col(static_cast<unsigned char>(s * 0.45f),
                          static_cast<unsigned char>(s * 0.6f), s);
            sf::Vertex quad[] = {
                sf::Vertex(sf::Vector2f(a.x, a.y), col),
                sf::Vertex(sf::Vector2f(b.x, b.y), col),
                sf::Vertex(sf::Vector2f(c.x, c.y), col),
                sf::Vertex(sf::Vector2f(d.x, d.y), col)
            };
            window.draw(quad, 4, sf::Quads);
        }
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Cloth Simulation (redeemed)");
    setup();
    engine.userLogic = [](World&, Renderer&, float) { step(); };
    engine.getRenderer().setDrawCallback(draw);
    engine.run();
}
