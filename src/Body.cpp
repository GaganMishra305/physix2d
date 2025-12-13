#include "physix2d/Body.h"

namespace physix2d {

Body::Body(float x, float y, float r, float m)  : pos(x, y), vel(0.0f, 0.0f), acc(0.0f, 0.0f), forceAccumulator(0.0f, 0.0f), radius(r), mass(m) {}

void Body::applyForce(const Vec2& f) {
    forceAccumulator = forceAccumulator + f;
}

//[TODO: add velocity dampening to avoid jittering]
void Body::update(float dt) {
    acc = forceAccumulator * (1 / mass);
    vel = vel + acc * dt;
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