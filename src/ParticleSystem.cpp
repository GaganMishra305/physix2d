#include "physix2d/ParticleSystem.h"
#include <algorithm>

namespace physix2d {

void ParticleSystem::setGravity(const Vec2& g) { gravity = g; }

void ParticleSystem::setBounds(const AABB& b, float restitution_) {
    bounds = b;
    restitution = restitution_;
    hasBounds = true;
}

Particle& ParticleSystem::emit(const Particle& p) {
    particles.push_back(p);
    return particles.back();
}

void ParticleSystem::step(float dt) {
    float damp = (damping > 0.0f) ? (1.0f / (1.0f + damping * dt)) : 1.0f;

    for (Particle& p : particles) {
        p.vel = p.vel + gravity * dt;
        if (damp != 1.0f) p.vel = p.vel * damp;
        p.pos = p.pos + p.vel * dt;
        p.life -= dt;

        if (hasBounds) {
            float r = p.radius;
            if (p.pos.x - r < bounds.min.x) { p.pos.x = bounds.min.x + r; if (p.vel.x < 0) p.vel.x = -p.vel.x * restitution; }
            if (p.pos.x + r > bounds.max.x) { p.pos.x = bounds.max.x - r; if (p.vel.x > 0) p.vel.x = -p.vel.x * restitution; }
            if (p.pos.y - r < bounds.min.y) { p.pos.y = bounds.min.y + r; if (p.vel.y < 0) p.vel.y = -p.vel.y * restitution; }
            if (p.pos.y + r > bounds.max.y) { p.pos.y = bounds.max.y - r; if (p.vel.y > 0) p.vel.y = -p.vel.y * restitution; }
        }
    }
    clearDead();
}

void ParticleSystem::clearDead() {
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                       [](const Particle& p) { return !p.alive(); }),
        particles.end());
}

size_t ParticleSystem::aliveCount() const {
    return particles.size();
}

}
