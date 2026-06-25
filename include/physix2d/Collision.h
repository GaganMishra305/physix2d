#pragma once
#include "physix2d/Body.h"
#include "physix2d/Manifold.h"

namespace physix2d {

class Collision {
public:
    // Shape-generic detect + resolve (circles & polygons, with rotation).
    static bool resolve(Body& a, Body& b, float restitution = 0.5f);
    // Resolve a pre-computed manifold.
    static void resolveManifold(Body& a, Body& b, const Manifold& m, float restitution);

    // Detect + resolve in one call (circle-only, no rotation -- kept for legacy demos)
    static bool resolveCircleVsCircle(Body& a, Body& b, float restitution = 0.8f);
    
    // Wall collision with positional + velocity correction
    static void resolveWallCollisions(Body& b, float minX, float maxX, float minY, float maxY, 
                                      float restitution = 0.8f);
};

}