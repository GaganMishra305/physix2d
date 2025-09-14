#include "physix2d/World.h"

namespace physix2d {

void World::update(float dt) {
    for (size_t i = 0; i < bodies.size() && i < forces.size(); ++i) {
        bodies[i].applyForce(forces[i]);
    }
    for (auto& body : bodies) {
        body.update();
    }
    forces.clear();
}

void World::addBody(Body &body) {
    bodies.push_back(body);
}

void World::addForce(const Vec2 &force) {
    forces.push_back(force);
}

std::vector<Body>& World::getBodies() {
    return bodies;
}

}