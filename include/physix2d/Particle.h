#pragma once

#include "physix2d/Vec2.h"

namespace physix2d {

// A lightweight point particle -- much cheaper than a Body (no rotation, no
// shape, no inertia). Used in bulk for fluids, sand, smoke, sparks, etc.
struct Particle {
    Vec2 pos;
    Vec2 vel;
    float radius = 3.0f;
    float life = 1e30f;   // remaining seconds; <= 0 means dead (1e30 ~ "persistent")
    float maxLife = 1e30f;

    // Colour hint for the renderer (0..255).
    unsigned char r = 255, g = 255, b = 255;

    bool alive() const { return life > 0.0f; }
    float ageFraction() const {           // 0 at birth -> 1 at death
        if (maxLife >= 1e29f) return 0.0f;
        float f = 1.0f - life / maxLife;
        return f < 0.0f ? 0.0f : (f > 1.0f ? 1.0f : f);
    }
};

}
