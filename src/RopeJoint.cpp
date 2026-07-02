#include "physix2d/RopeJoint.h"

namespace physix2d {

RopeJoint::RopeJoint(Body* a_, Body* b_, float maxLength_, float beta_)
    : a(a_), b(b_), maxLength(maxLength_), beta(beta_) {
    if (maxLength < 0.0f) maxLength = Vec2::distance(a->pos, b->pos);
}

void RopeJoint::solve(float dt) {
    Vec2 d = b->pos - a->pos;
    float dist = d.getMag();
    if (dist < 1e-6f) return;

    float C = dist - maxLength;
    if (C < 0.0f) return; // slack -> rope exerts no force

    Vec2 n = d * (1.0f / dist);
    float invSum = a->getInvMass() + b->getInvMass();
    if (invSum <= 0.0f) return;

    float relVel = (b->vel - a->vel).dot(n);
    float bias = (beta / dt) * C;
    // Only pull inward (tension); never push apart.
    float lambda = -(relVel + bias) / invSum;
    if (lambda > 0.0f) lambda = 0.0f; // a positive impulse would push them apart

    Vec2 impulse = n * lambda;
    a->vel = a->vel - impulse * a->getInvMass();
    b->vel = b->vel + impulse * b->getInvMass();
}

}
