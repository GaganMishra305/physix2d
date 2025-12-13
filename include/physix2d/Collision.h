#pragma once
#include "physix2d/Body.h"

namespace physix2d {

class Collision {
public:
    // Detect + resolve in one call
    static bool resolveCircleVsCircle(Body& a, Body& b, float restitution = 0.8f);
    
    // Wall collision with positional + velocity correction
    static void resolveWallCollisions(Body& b, float minX, float maxX, float minY, float maxY, 
                                      float restitution = 0.8f);
};

}