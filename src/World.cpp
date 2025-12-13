#include "physix2d/World.h"

namespace physix2d {

void World::update(float dt) {
    for (auto& b : bodies)
        b.clearForces();

    for (auto& f : forces)
        f->apply(dt);

    for (auto& b : bodies)
        b.update(dt);
}

Body& World::addBody(const Body& body) {
    bodies.push_back(body);
    return bodies.back();
}

void World::addForce(std::unique_ptr<Force> force) {
    forces.push_back(std::move(force));
}

std::deque<Body>& World::getBodies() {
    return bodies;
}

}