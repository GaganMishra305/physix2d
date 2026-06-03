#include "physix2d/DistanceJoint.h"
#include <cmath>

namespace physix2d {

DistanceJoint::DistanceJoint(Body* a_, Body* b_, float restLength_, float beta_)
    : a(a_), b(b_), restLength(restLength_), beta(beta_) {
    if (restLength < 0.0f) {
        restLength = Vec2::distance(a->pos, b->pos);
    }
}

void DistanceJoint::solve(float dt) {
    Vec2 d = b->pos - a->pos;
    float dist = d.getMag();
    if (dist < 1e-6f) return;

    Vec2 n = d * (1.0f / dist);
    float invSum = a->getInvMass() + b->getInvMass();
    if (invSum <= 0.0f) return; // both static

    // C = dist - restLength (position error). We drive Cdot + (beta/dt)*C -> 0.
    float C = dist - restLength;
    float relVel = (b->vel - a->vel).dot(n);
    float bias = (beta / dt) * C;
    float lambda = -(relVel + bias) / invSum;

    Vec2 impulse = n * lambda;
    a->vel = a->vel - impulse * a->getInvMass();
    b->vel = b->vel + impulse * b->getInvMass();
}

}
