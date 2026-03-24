#include "physix2d/Body.h"

namespace physix2d {

Body::Body(float x, float y, float r, float m)
    : pos(x, y), vel(0.0f, 0.0f), acc(0.0f, 0.0f),
      prev_pos(x, y), forceAccumulator(0.0f, 0.0f),
      radius(r), mass(m) {
    setMass(m);
}

void Body::setMass(float m) {
    mass = m;
    invMass = (m > 0.0f) ? (1.0f / m) : 0.0f;   // m <= 0 => static / infinite mass
}

void Body::setDamping(float d) {
    linearDamping = (d > 0.0f) ? d : 0.0f;
}

void Body::applyForce(const Vec2& f) {
    forceAccumulator += f;
}

void Body::update(float dt) {
    // Static bodies never move; just drop any accumulated force.
    if (invMass == 0.0f) {
        clearForces();
        return;
    }
    acc = forceAccumulator * invMass;
    vel = vel + acc * dt;
    // Frame-rate independent linear damping: stable exponential-ish decay.
    if (linearDamping > 0.0f) {
        vel = vel * (1.0f / (1.0f + linearDamping * dt));
    }
    prev_pos = pos;
    pos = pos + vel * dt;
    clearForces();
}

void Body::clearForces() {
    forceAccumulator = Vec2(0.0f, 0.0f);
}

Vec2 Body::getPosition() const {
    return pos;
}

float Body::getRadius() const {
    return radius;
}

float Body::getMass() const {
    return mass;
}

float Body::getInvMass() const {
    return invMass;
}

bool Body::isStatic() const {
    return invMass == 0.0f;
}

Vec2 Body::getVel() const {
    return vel;
}

void Body::setPos(Vec2 pos_) {
    pos = pos_;
}

void Body::setVel(Vec2 vel_) {
    vel = vel_;
}

}
