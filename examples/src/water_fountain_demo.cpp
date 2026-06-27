#include <random>

#include "physix2d/Engine.h"
#include "physix2d/ParticleSystem.h"

using namespace physix2d;

// Water fountain: a nozzle jets particles upward with spread; gravity arcs them
// back down and they pool/slosh in the basin. Light contact iterations give the
// pooled water some body without going full SPH.

const int WIDTH = 800, HEIGHT = 600;
const float DT = 1.0f / 60.0f;

ParticleSystem water;
std::mt19937 rng(2024);

void setup() {
    water.setGravity(Vec2(0.0f, 700.0f));
    water.setBounds(AABB{ Vec2(6, 6), Vec2(WIDTH - 6, HEIGHT - 6) }, 0.15f);
    water.damping = 0.05f;
}

void step() {
    // Emit a burst from the nozzle each frame (capped population).
    if (water.aliveCount() < 1400) {
        std::uniform_real_distribution<float> spread(-90.0f, 90.0f);
        std::uniform_real_distribution<float> up(-620.0f, -520.0f);
        std::uniform_real_distribution<float> life(2.5f, 4.0f);
        for (int i = 0; i < 8; ++i) {
            Particle p;
            p.pos = Vec2(WIDTH * 0.5f, HEIGHT - 20.0f);
            p.vel = Vec2(spread(rng), up(rng));
            p.radius = 4.0f;
            p.maxLife = p.life = life(rng);
            p.r = 60; p.g = 150; p.b = 255;
            water.emit(p);
        }
    }

    water.step(DT);
    for (int it = 0; it < 2; ++it) { water.resolveContacts(0.4f); water.clampToBounds(); }
}

void draw(sf::RenderWindow& window, World&) {
    for (const Particle& p : water.particles) {
        sf::CircleShape c(p.radius);
        c.setPosition(p.pos.x - p.radius, p.pos.y - p.radius);
        c.setFillColor(sf::Color(p.r, p.g, p.b));
        window.draw(c);
    }
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Water Fountain");
    setup();
    engine.userLogic = [](World&, Renderer&, float) { step(); };
    engine.getRenderer().setDrawCallback(draw);
    engine.run();
}
