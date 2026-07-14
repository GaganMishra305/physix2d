#include <random>
#include <cmath>

#include "physix2d/Engine.h"
#include "physix2d/ParticleSystem.h"

using namespace physix2d;

// Tornado: a wandering vortex applies a swirling force field to debris particles
// -- tangential spin + inward pull + upward lift near the core -- so they get
// sucked up and thrown around the funnel.

const int WIDTH = 1000, HEIGHT = 700;
const float DT = 1.0f / 60.0f;

ParticleSystem debris;
std::mt19937 rng(11);
float t = 0.0f;

void setup() {
    debris.setGravity(Vec2(0.0f, 240.0f)); // mild gravity; the vortex dominates
    debris.setBounds(AABB{ Vec2(6, 6), Vec2(WIDTH - 6, HEIGHT - 6) }, 0.2f);
    debris.damping = 0.25f;
}

void step() {
    t += DT;
    // Vortex core wanders side to side along the ground.
    Vec2 core(WIDTH * 0.5f + 180.0f * std::sin(t * 0.5f), HEIGHT - 80.0f);

    // Spawn debris along the ground.
    if (debris.aliveCount() < 1200) {
        std::uniform_real_distribution<float> x(40, WIDTH - 40), c(90, 160);
        for (int i = 0; i < 6; ++i) {
            Particle p;
            p.pos = Vec2(x(rng), HEIGHT - 20.0f);
            p.vel = Vec2(0, 0);
            p.radius = 3.0f;
            p.maxLife = p.life = 6.0f;
            unsigned char g = static_cast<unsigned char>(c(rng));
            p.r = g; p.g = static_cast<unsigned char>(g * 0.8f); p.b = static_cast<unsigned char>(g * 0.6f);
            debris.emit(p);
        }
    }

    // Apply the vortex force field.
    for (Particle& p : debris.particles) {
        Vec2 r = p.pos - core;
        float dist = r.getMag();
        if (dist < 1e-3f) continue;
        Vec2 rn = r * (1.0f / dist);
        Vec2 tangent(-rn.y, rn.x);                 // swirl direction

        float reach = 420.0f;
        if (dist < reach) {
            float falloff = 1.0f - dist / reach;   // stronger near the core
            float swirl = 900.0f * falloff;
            float pull  = 500.0f * falloff;
            float lift  = 1400.0f * falloff * falloff;
            Vec2 accel = tangent * swirl - rn * pull + Vec2(0.0f, -lift);
            p.vel = p.vel + accel * DT;
        }
    }

    debris.step(DT);
    debris.clampToBounds();
}

void draw(sf::RenderWindow& window, World&) {
    for (const Particle& p : debris.particles) {
        sf::CircleShape c(p.radius);
        c.setPosition(p.pos.x - p.radius, p.pos.y - p.radius);
        c.setFillColor(sf::Color(p.r, p.g, p.b));
        window.draw(c);
    }
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Tornado");
    setup();
    engine.userLogic = [](World&, Renderer&, float) { step(); };
    engine.getRenderer().setDrawCallback(draw);
    engine.run();
}
