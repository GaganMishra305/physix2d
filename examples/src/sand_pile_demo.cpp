#include <random>

#include "physix2d/Engine.h"
#include "physix2d/ParticleSystem.h"

using namespace physix2d;

// Sand pile: grains trickle from above and heap up. Many contact iterations +
// near-zero restitution make the grains settle and stack instead of bouncing.

const int WIDTH = 800, HEIGHT = 600;
const float DT = 1.0f / 60.0f;

ParticleSystem sand;
std::mt19937 rng(99);

void setup() {
    sand.setGravity(Vec2(0.0f, 800.0f));
    sand.setBounds(AABB{ Vec2(6, 6), Vec2(WIDTH - 6, HEIGHT - 6) }, 0.0f);
    sand.damping = 0.02f;
}

void step() {
    if (sand.aliveCount() < 2500) {
        std::uniform_real_distribution<float> jitter(-14.0f, 14.0f);
        std::uniform_real_distribution<float> vx(-20.0f, 20.0f);
        for (int i = 0; i < 5; ++i) {
            Particle p;
            p.pos = Vec2(WIDTH * 0.5f + jitter(rng), 40.0f);
            p.vel = Vec2(vx(rng), 60.0f);
            p.radius = 4.0f;
            p.maxLife = p.life = 1e30f; // grains persist
            p.r = 224; p.g = 190; p.b = 110;
            sand.emit(p);
        }
    }

    sand.step(DT);
    // Stiff, many-iteration contacts -> grains stack into a heap.
    for (int it = 0; it < 6; ++it) { sand.resolveContacts(0.8f); sand.clampToBounds(); }
}

void draw(sf::RenderWindow& window, World&) {
    for (const Particle& p : sand.particles) {
        sf::CircleShape c(p.radius);
        c.setPosition(p.pos.x - p.radius, p.pos.y - p.radius);
        c.setFillColor(sf::Color(p.r, p.g, p.b));
        window.draw(c);
    }
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Sand Pile");
    setup();
    engine.userLogic = [](World&, Renderer&, float) { step(); };
    engine.getRenderer().setDrawCallback(draw);
    engine.run();
}
