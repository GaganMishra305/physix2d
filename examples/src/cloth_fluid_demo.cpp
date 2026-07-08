#include <random>
#include <cmath>

#include "physix2d/Engine.h"
#include "physix2d/SoftBody.h"
#include "physix2d/ParticleSystem.h"

using namespace physix2d;

// Cloth + fluid: water is poured onto a hanging cloth net (pinned at its two top
// corners). Two-way coupling -- particles push the cloth down, the cloth catches
// and deflects the particles. The cloth's Verlet integration turns the position
// pushes into real motion automatically.

const int WIDTH = 900, HEIGHT = 700;
const float DT = 1.0f / 60.0f;
const int COLS = 22, ROWS = 5;
const float CLOTH_R = 7.0f;

SoftBody net;
ParticleSystem water;
std::mt19937 rng(2026);

void setup() {
    net = SoftBody::cloth(Vec2(180, 180), 520, 90, COLS, ROWS, 1.0f);
    net.gravity = Vec2(0.0f, 500.0f);
    net.damping = 0.02f;
    net.iterations = 20;
    net.pin(0);                 // top-left
    net.pin(COLS);              // top-right

    water.setGravity(Vec2(0.0f, 900.0f));
    water.setBounds(AABB{ Vec2(6, 6), Vec2(WIDTH - 6, HEIGHT - 6) }, 0.1f);
    water.damping = 0.04f;
}

// Push particles out of cloth points; the cloth (Verlet) gains motion in turn.
void couple() {
    for (Particle& p : water.particles) {
        for (PointMass& cp : net.points) {
            Vec2 d = p.pos - cp.pos;
            float rSum = p.radius + CLOTH_R;
            float dist2 = d.lengthSq();
            if (dist2 >= rSum * rSum || dist2 < 1e-6f) continue;
            float dist = std::sqrt(dist2);
            Vec2 n = d * (1.0f / dist);
            float overlap = rSum - dist;
            // Split correction: particle moves most, pinned cloth points don't move.
            float clothShare = (cp.invMass > 0.0f) ? 0.5f : 0.0f;
            p.pos = p.pos + n * (overlap * (1.0f - clothShare));
            cp.pos = cp.pos - n * (overlap * clothShare);
            // Kill inward normal velocity of the particle.
            float vn = p.vel.dot(n);
            if (vn < 0.0f) p.vel = p.vel - n * vn;
        }
    }
}

void step() {
    if (water.aliveCount() < 700) {
        std::uniform_real_distribution<float> jx(-40.0f, 40.0f);
        for (int i = 0; i < 5; ++i) {
            Particle p;
            p.pos = Vec2(WIDTH * 0.5f + jx(rng), 60.0f);
            p.vel = Vec2(0.0f, 60.0f);
            p.radius = 4.0f;
            p.maxLife = p.life = 6.0f;
            p.r = 60; p.g = 150; p.b = 255;
            water.emit(p);
        }
    }

    net.step(DT);
    water.step(DT);
    for (int it = 0; it < 4; ++it) {
        water.resolveContacts(0.4f);
        couple();
        water.clampToBounds();
    }
}

void draw(sf::RenderWindow& window, World&) {
    // Water first (so the net draws on top).
    for (const Particle& p : water.particles) {
        sf::CircleShape c(p.radius);
        c.setPosition(p.pos.x - p.radius, p.pos.y - p.radius);
        c.setFillColor(sf::Color(p.r, p.g, p.b));
        window.draw(c);
    }
    // Net constraints as lines.
    for (const auto& c : net.constraints) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(net.points[c.a].pos.x, net.points[c.a].pos.y), sf::Color(220,220,180)),
            sf::Vertex(sf::Vector2f(net.points[c.b].pos.x, net.points[c.b].pos.y), sf::Color(220,220,180))
        };
        window.draw(line, 2, sf::Lines);
    }
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Cloth + Fluid");
    setup();
    engine.userLogic = [](World&, Renderer&, float) { step(); };
    engine.getRenderer().setDrawCallback(draw);
    engine.run();
}
