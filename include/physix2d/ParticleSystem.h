#pragma once

#include <vector>
#include "physix2d/Particle.h"
#include "physix2d/Shape.h"   // for AABB

namespace physix2d {

// Manages a pool of particles: emission, integration, lifetime, and bounds.
// Inter-particle contacts live in a separate pass (see resolveContacts) so the
// base step stays cheap for effects (smoke/sparks) that don't need collision.
class ParticleSystem {
public:
    std::vector<Particle> particles;

    Vec2 gravity{0.0f, 0.0f};
    float damping = 0.0f;        // velocity decay per second
    bool hasBounds = false;
    AABB bounds{Vec2(0,0), Vec2(0,0)};
    float restitution = 0.3f;    // bounce off bounds

    void setGravity(const Vec2& g);
    void setBounds(const AABB& b, float restitution_);

    Particle& emit(const Particle& p);
    void step(float dt);         // integrate + age + bounds
    void clearDead();
    size_t aliveCount() const;

    // Position-only clamp back inside bounds. Call AFTER resolveContacts so
    // repulsion can't push particles through the walls (PBD ordering).
    void clampToBounds();

    // Inter-particle repulsion via spatial hash (position-based, Gauss-Seidel).
    // stiffness in [0..1] controls how much overlap is removed per call;
    // cellSize<=0 auto-picks from the largest particle radius.
    void resolveContacts(float stiffness = 0.5f, float cellSize = 0.0f);
};

}
