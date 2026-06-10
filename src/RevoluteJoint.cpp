#include "physix2d/RevoluteJoint.h"
#include <cmath>

namespace physix2d {

namespace {
Vec2 rotate(const Vec2& v, float angle) {
    float c = std::cos(angle), s = std::sin(angle);
    return Vec2(v.x * c - v.y * s, v.x * s + v.y * c);
}
Vec2 crossSV(float w, const Vec2& r) { return Vec2(-w * r.y, w * r.x); }
float crossVV(const Vec2& a, const Vec2& b) { return a.x * b.y - a.y * b.x; }
Vec2 invBodyPoint(const Body* body, const Vec2& world) {
    // world -> local: R^-1 * (world - pos)
    return rotate(world - body->pos, -body->angle);
}
}

RevoluteJoint::RevoluteJoint(Body* a_, Body* b_, const Vec2& worldAnchor, float beta_)
    : a(a_), b(b_), beta(beta_) {
    localAnchorA = invBodyPoint(a, worldAnchor);
    localAnchorB = invBodyPoint(b, worldAnchor);
}

void RevoluteJoint::solve(float dt) {
    float mA = a->getInvMass(), mB = b->getInvMass();
    float iA = a->getInvInertia(), iB = b->getInvInertia();
    if (mA + mB <= 0.0f) return;

    Vec2 rA = rotate(localAnchorA, a->angle);
    Vec2 rB = rotate(localAnchorB, b->angle);

    // Relative velocity at the pin.
    Vec2 vA = a->vel + crossSV(a->angularVel, rA);
    Vec2 vB = b->vel + crossSV(b->angularVel, rB);
    Vec2 cdot = vB - vA;

    // Baumgarte positional bias: pull coincident anchors back together.
    Vec2 anchorA = a->pos + rA;
    Vec2 anchorB = b->pos + rB;
    Vec2 posError = anchorB - anchorA;
    Vec2 bias = posError * (beta / dt);

    // 2x2 effective-mass matrix K.
    float k11 = mA + mB + iA * rA.y * rA.y + iB * rB.y * rB.y;
    float k12 = -iA * rA.x * rA.y - iB * rB.x * rB.y;
    float k22 = mA + mB + iA * rA.x * rA.x + iB * rB.x * rB.x;
    float det = k11 * k22 - k12 * k12;
    if (std::abs(det) < 1e-12f) return;
    float invDet = 1.0f / det;

    // Solve K * impulse = -(cdot + bias).
    Vec2 rhs = -(cdot + bias);
    Vec2 impulse(
        invDet * (k22 * rhs.x - k12 * rhs.y),
        invDet * (k11 * rhs.y - k12 * rhs.x)
    );

    a->vel = a->vel - impulse * mA;
    a->angularVel -= iA * crossVV(rA, impulse);
    b->vel = b->vel + impulse * mB;
    b->angularVel += iB * crossVV(rB, impulse);
}

}
