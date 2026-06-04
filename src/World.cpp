#include "physix2d/World.h"

namespace physix2d {

void World::update(float dt) {
    for (auto& b : bodies)
        b.clearForces();

    for (auto& f : forces)
        f->apply(dt);

    for (auto& b : bodies)
        b.update(dt);

    // Constraint solve (multiple iterations for stiffness/stability).
    for (int it = 0; it < jointIterations; ++it)
        for (auto& j : joints)
            j->solve(dt);
}

Body& World::addBody(const Body& body) {
    bodies.push_back(body);
    return bodies.back();
}

void World::addForce(std::unique_ptr<Force> force) {
    forces.push_back(std::move(force));
}

void World::addJoint(std::unique_ptr<Joint> joint) {
    joints.push_back(std::move(joint));
}

void World::setJointIterations(int n) {
    jointIterations = (n > 0) ? n : 1;
}

std::deque<Body>& World::getBodies() {
    return bodies;
}

}