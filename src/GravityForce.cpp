#include "physix2d/GravityForce.h"

namespace physix2d {

GravityForce::GravityForce(const Vec2& gravity, const std::vector<Body*>& targets): bodies(targets), g(gravity) {}

void GravityForce::apply(float) {
    for (Body* b : bodies) {
        b->applyForce(g * b->getMass());
    }
}

void GravityForce::addBody(Body* body) {
    bodies.push_back(body);
}

}
