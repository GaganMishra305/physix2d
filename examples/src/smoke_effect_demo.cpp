#include <random>
#include <cmath>

#include "physix2d/Engine.h"
#include "physix2d/ParticleSystem.h"

using namespace physix2d;

// Smoke: buoyant particles rise (upward "gravity"), expand as they age, wobble
// with a bit of turbulence, and fade out via alpha before dying. No collisions.

const int WIDTH = 800, HEIGHT = 600;
const float DT = 1.0f / 60.0f;

ParticleSystem smoke;
std::mt19937 rng(7);

void setup() {
    smoke.setGravity(Vec2(0.0f, -55.0f)); // buoyancy: rises
    smoke.damping = 0.6f;                  // air drag
}

void step() {
    if (smoke.aliveCount() < 900) {
        std::uniform_real_distribution<float> jx(-10.0f, 10.0f);
        std::uniform_real_distribution<float> vx(-25.0f, 25.0f);
        std::uniform_real_distribution<float> life(3.0f, 5.0f);
        for (int i = 0; i < 4; ++i) {
            Particle p;
            p.pos = Vec2(WIDTH * 0.5f + jx(rng), HEIGHT - 30.0f);
            p.vel = Vec2(vx(rng), -80.0f);
            p.radius = 6.0f;
            p.maxLife = p.life = life(rng);
            p.r = 200; p.g = 200; p.b = 205;
            smoke.emit(p);
        }
    }

    smoke.step(DT);

    // Turbulence + expansion (particles is public data -> tweak per effect).
    std::uniform_real_distribution<float> turb(-30.0f, 30.0f);
    for (Particle& p : smoke.particles) {
        p.vel.x += turb(rng) * DT * 6.0f;
        p.radius += 14.0f * DT;            // puff grows as it ages
    }
}

void draw(sf::RenderWindow& window, World&) {
    for (const Particle& p : smoke.particles) {
        float a = 1.0f - p.ageFraction();  // fade out
        sf::CircleShape c(p.radius);
        c.setPosition(p.pos.x - p.radius, p.pos.y - p.radius);
        c.setFillColor(sf::Color(p.r, p.g, p.b, static_cast<unsigned char>(140.0f * a)));
        window.draw(c);
    }
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Smoke");
    setup();
    engine.userLogic = [](World&, Renderer&, float) { step(); };
    engine.getRenderer().setDrawCallback(draw);
    engine.run();
}
