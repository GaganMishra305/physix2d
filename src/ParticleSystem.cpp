#include "physix2d/ParticleSystem.h"
#include <algorithm>
#include <unordered_map>
#include <cmath>
#include <cstdint>

namespace physix2d {

namespace {
inline int64_t cellKey(int cx, int cy) {
    return (static_cast<int64_t>(cx) << 32) ^ static_cast<int64_t>(static_cast<uint32_t>(cy));
}
}

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

void ParticleSystem::clampToBounds() {
    if (!hasBounds) return;
    for (Particle& p : particles) {
        float r = p.radius;
        if (p.pos.x - r < bounds.min.x) { p.pos.x = bounds.min.x + r; if (p.vel.x < 0) p.vel.x *= -restitution; }
        if (p.pos.x + r > bounds.max.x) { p.pos.x = bounds.max.x - r; if (p.vel.x > 0) p.vel.x *= -restitution; }
        if (p.pos.y - r < bounds.min.y) { p.pos.y = bounds.min.y + r; if (p.vel.y < 0) p.vel.y *= -restitution; }
        if (p.pos.y + r > bounds.max.y) { p.pos.y = bounds.max.y - r; if (p.vel.y > 0) p.vel.y *= -restitution; }
    }
}

void ParticleSystem::resolveContacts(float stiffness, float cellSize) {
    size_t n = particles.size();
    if (n < 2) return;

    if (cellSize <= 0.0f) {
        float maxR = 0.0f;
        for (const Particle& p : particles) maxR = std::max(maxR, p.radius);
        cellSize = std::max(2.0f * maxR, 1.0f);
    }
    float inv = 1.0f / cellSize;

    // Bucket particle indices into a uniform grid.
    std::unordered_map<int64_t, std::vector<int>> grid;
    grid.reserve(n * 2);
    for (size_t i = 0; i < n; ++i) {
        int cx = static_cast<int>(std::floor(particles[i].pos.x * inv));
        int cy = static_cast<int>(std::floor(particles[i].pos.y * inv));
        grid[cellKey(cx, cy)].push_back(static_cast<int>(i));
    }

    // For each particle, test the 3x3 neighbourhood; resolve overlaps once.
    for (size_t i = 0; i < n; ++i) {
        Particle& a = particles[i];
        int cx = static_cast<int>(std::floor(a.pos.x * inv));
        int cy = static_cast<int>(std::floor(a.pos.y * inv));
        for (int ox = -1; ox <= 1; ++ox)
        for (int oy = -1; oy <= 1; ++oy) {
            auto it = grid.find(cellKey(cx + ox, cy + oy));
            if (it == grid.end()) continue;
            for (int j : it->second) {
                if (j <= static_cast<int>(i)) continue; // each pair once
                Particle& b = particles[j];
                Vec2 d = b.pos - a.pos;
                float rSum = a.radius + b.radius;
                float dist2 = d.lengthSq();
                if (dist2 >= rSum * rSum || dist2 < 1e-9f) continue;
                float dist = std::sqrt(dist2);
                Vec2 nrm = d * (1.0f / dist);
                float overlap = rSum - dist;
                Vec2 corr = nrm * (overlap * 0.5f * stiffness);
                a.pos = a.pos - corr;
                b.pos = b.pos + corr;
                // Damp the approaching normal velocity (viscosity-ish).
                Vec2 rv = b.vel - a.vel;
                float vn = rv.dot(nrm);
                if (vn < 0.0f) {
                    Vec2 imp = nrm * (vn * 0.5f * stiffness);
                    a.vel = a.vel + imp;
                    b.vel = b.vel - imp;
                }
            }
        }
    }
}

}
