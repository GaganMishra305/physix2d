#pragma once

#include "physix2d/Joint.h"
#include "physix2d/Body.h"

namespace physix2d {

// Pins two bodies together at a shared world anchor point. The bodies may
// rotate freely about the pin, but the anchor points are held coincident.
// (If one body is static, the other swings about the pin like a hinge.)
class RevoluteJoint : public Joint {
public:
    Body* a;
    Body* b;
    Vec2 localAnchorA; // anchor in A's local frame
    Vec2 localAnchorB; // anchor in B's local frame
    float beta;        // Baumgarte position-correction factor

    // worldAnchor is the pin location in world space at setup time.
    RevoluteJoint(Body* a_, Body* b_, const Vec2& worldAnchor, float beta_ = 0.2f);
    void solve(float dt) override;
};

}
