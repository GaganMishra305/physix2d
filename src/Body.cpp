#include "physix2d/Body.h"
#include "physix2d/CircleShape.h"
#include <cmath>
#include <algorithm>

namespace physix2d {

Body::Body(float x, float y, float r, float m)
    : pos(x, y), vel(0.0f, 0.0f), acc(0.0f, 0.0f),
      prev_pos(x, y), forceAccumulator(0.0f, 0.0f),
      radius(r), mass(m), shape(std::make_shared<CircleShape>(r)) {
    setMass(m);
}

Body::Body(float x, float y, std::shared_ptr<Shape> shape_, float m)
    : pos(x, y), vel(0.0f, 0.0f), acc(0.0f, 0.0f),
      prev_pos(x, y), forceAccumulator(0.0f, 0.0f),
      radius(0.0f), mass(m), shape(std::move(shape_)) {
    recomputeBoundingRadius();
    setMass(m);
}

void Body::recomputeBoundingRadius() {
    if (!shape) { radius = 0.0f; return; }
    AABB box = shape->computeAABB(Vec2(0.0f, 0.0f), 0.0f);
    float rx = std::max(std::abs(box.min.x), std::abs(box.max.x));
    float ry = std::max(std::abs(box.min.y), std::abs(box.max.y));
    radius = std::sqrt(rx * rx + ry * ry);
}

void Body::recomputeInertia() {
    if (shape && mass > 0.0f) {
        inertia = shape->computeInertia(mass);
        invInertia = (inertia > 0.0f) ? (1.0f / inertia) : 0.0f;
    } else {
        inertia = 0.0f;
        invInertia = 0.0f;
    }
}

void Body::setMass(float m) {
    mass = m;
    invMass = (m > 0.0f) ? (1.0f / m) : 0.0f;   // m <= 0 => static / infinite mass
    recomputeInertia();
}

void Body::setDamping(float d) {
    linearDamping = (d > 0.0f) ? d : 0.0f;
}

void Body::setFriction(float f) {
    friction = f < 0.0f ? 0.0f : (f > 1.0f ? 1.0f : f);
}

void Body::applyForce(const Vec2& f) {
    forceAccumulator += f;
}

void Body::applyTorque(float t) {
    torque += t;
}

void Body::update(float dt) {
    // Static bodies never move; just drop any accumulated force/torque.
    if (invMass == 0.0f) {
        clearForces();
        return;
    }
    // Linear integration
    acc = forceAccumulator * invMass;
    vel = vel + acc * dt;
    if (linearDamping > 0.0f) {
        vel = vel * (1.0f / (1.0f + linearDamping * dt));
    }
    prev_pos = pos;
    pos = pos + vel * dt;

    // Angular integration
    if (invInertia > 0.0f) {
        angularVel += torque * invInertia * dt;
        if (angularDamping > 0.0f) {
            angularVel *= (1.0f / (1.0f + angularDamping * dt));
        }
        angle += angularVel * dt;
    }

    clearForces();
}

void Body::clearForces() {
    forceAccumulator = Vec2(0.0f, 0.0f);
    torque = 0.0f;
}

Vec2 Body::getPosition() const { return pos; }
float Body::getRadius() const { return radius; }
float Body::getMass() const { return mass; }
float Body::getInvMass() const { return invMass; }
float Body::getInvInertia() const { return invInertia; }
float Body::getAngle() const { return angle; }
bool Body::isStatic() const { return invMass == 0.0f; }
Shape* Body::getShape() const { return shape.get(); }
Vec2 Body::getVel() const { return vel; }

void Body::setPos(Vec2 pos_) { pos = pos_; }
void Body::setVel(Vec2 vel_) { vel = vel_; }
void Body::setAngle(float a) { angle = a; }

}
