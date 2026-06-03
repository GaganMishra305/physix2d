#pragma once

#include "physix2d/Joint.h"
#include "physix2d/Body.h"

namespace physix2d {

// Keeps two bodies at a fixed distance (center-to-center). Velocity-impulse
// formulation with Baumgarte position stabilization, so it plays nicely with
// the engine's velocity integration (no position-vs-velocity desync).
class DistanceJoint : public Joint {
public:
    Body* a;
    Body* b;
    float restLength; // if < 0, captured from the current separation
    float beta;       // Baumgarte position-correction factor [0..1]

    DistanceJoint(Body* a_, Body* b_, float restLength_ = -1.0f, float beta_ = 0.2f);
    void solve(float dt) override;
};

}
