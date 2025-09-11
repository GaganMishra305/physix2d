#include "physix2d/Body.h"

namespace physix2d {

Body::Body(float x, float y, float r, float m)  : pos(x, y), vel(0.0f, 0.0f), acc(0.0f, 0.0f), radius(r), mass(m) {}


void Body::applyForce(Vec2 force) {
    // F = ma, so a = F/m
    Vec2 acceleration = Vec2(force.getX() / mass, force.getY() / mass);
    acc = acc.add(acceleration);
}

void Body::update() {
    vel = vel.add(acc);
    pos = pos.add(vel);
    acc = Vec2(0.0f, 0.0f);
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

}