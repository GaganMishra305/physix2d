#pragma once

#include "physix2d/Body.h"

namespace physix2d {

// A contact manifold: everything the solver needs to resolve a collision.
// `normal` always points from body A towards body B.
struct Manifold {
    bool hit = false;
    Vec2 normal;
    float penetration = 0.0f;
    Vec2 contacts[2];
    int contactCount = 0;
};

// Narrow-phase collision detection. Dispatches on the bodies' shape types
// and produces a contact manifold (SAT + face clipping for polygons).
class CollisionDetector {
public:
    static Manifold detect(Body& a, Body& b);
};

}
