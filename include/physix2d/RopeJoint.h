#pragma once

#include "physix2d/Joint.h"
#include "physix2d/Body.h"

namespace physix2d {

// A rope: like DistanceJoint but tension-only. It does nothing while the bodies
// are closer than maxLength (rope is slack) and only applies a corrective
// impulse when they try to separate beyond it (rope goes taut).
class RopeJoint : public Joint {
public:
    Body* a;
    Body* b;
    float maxLength; // if < 0, captured from the current separation
    float beta;      // Baumgarte position-correction factor

    RopeJoint(Body* a_, Body* b_, float maxLength_ = -1.0f, float beta_ = 0.2f);
    void solve(float dt) override;
};

}
