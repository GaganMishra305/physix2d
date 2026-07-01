#include <cmath>

#include "physix2d/Engine.h"
#include "physix2d/ParticleSystem.h"

using namespace physix2d;

// Liquid in a box: a body of water that sloshes when we rock gravity sideways
// (as if tilting the container) and settles to a flat surface. Many contact
// iterations approximate incompressibility.

const int WIDTH = 800, HEIGHT = 600;
const float DT = 1.0f / 60.0f;

ParticleSystem liquid;
float t = 0.0f;

void setup() {
    liquid.setBounds(AABB{ Vec2(60, 60), Vec2(WIDTH - 60, HEIGHT - 20) }, 0.0f);
    liquid.damping = 0.08f;

    // Fill the lower half of the box with a grid of water particles.
    float r = 5.0f;
    for (float y = HEIGHT - 40; y > HEIGHT * 0.45f; y -= 2 * r)
        for (float x = 70; x < WIDTH - 70; x += 2 * r) {
            Particle p;
            p.pos = Vec2(x, y);
            p.radius = r;
            p.maxLife = p.life = 1e30f;
            p.r = 40; p.g = 130; p.b = 230;
            liquid.emit(p);
        }
}

void step() {
    t += DT;
    // Rock gravity left/right to slosh the liquid (container tilt).
    float gx = 500.0f * std::sin(t * 1.1f);
    liquid.setGravity(Vec2(gx, 800.0f));

    liquid.step(DT);
    // High iteration count -> incompressible-ish body of water.
    for (int it = 0; it < 8; ++it) { liquid.resolveContacts(0.7f); liquid.clampToBounds(); }
}

void draw(sf::RenderWindow& window, World&) {
    // Container outline.
    sf::RectangleShape box(sf::Vector2f(WIDTH - 120, HEIGHT - 80));
    box.setPosition(60, 60);
    box.setFillColor(sf::Color(20, 20, 30));
    box.setOutlineThickness(2.0f);
    box.setOutlineColor(sf::Color(120, 120, 140));
    window.draw(box);

    for (const Particle& p : liquid.particles) {
        sf::CircleShape c(p.radius);
        c.setPosition(p.pos.x - p.radius, p.pos.y - p.radius);
        c.setFillColor(sf::Color(p.r, p.g, p.b));
        window.draw(c);
    }
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Liquid in a Box");
    setup();
    engine.userLogic = [](World&, Renderer&, float) { step(); };
    engine.getRenderer().setDrawCallback(draw);
    engine.run();
}
