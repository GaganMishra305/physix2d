#include "physix2d/World.h"
#include "physix2d/SweepAndPrune.h"
#include "physix2d/Manifold.h"

namespace physix2d {

namespace {
inline uint64_t pairKey(uint32_t i, uint32_t j) {
    return (static_cast<uint64_t>(i) << 32) | static_cast<uint64_t>(j);
}
}

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

    if (onBeginContact || onEndContact)
        detectContacts();
}

void World::detectContacts() {
    // Current touching pairs (SAT-confirmed), keyed by stable body indices.
    std::unordered_set<uint64_t> current;
    auto pairs = SweepAndPrune::computePairs(bodies);
    for (auto& p : pairs) {
        Manifold m = CollisionDetector::detect(bodies[p.first], bodies[p.second]);
        if (!m.hit) continue;
        uint64_t key = pairKey(static_cast<uint32_t>(p.first), static_cast<uint32_t>(p.second));
        current.insert(key);
        if (onBeginContact && activeContacts.find(key) == activeContacts.end())
            onBeginContact(bodies[p.first], bodies[p.second]);
    }
    if (onEndContact) {
        for (uint64_t key : activeContacts) {
            if (current.find(key) == current.end()) {
                uint32_t i = static_cast<uint32_t>(key >> 32);
                uint32_t j = static_cast<uint32_t>(key & 0xffffffff);
                onEndContact(bodies[i], bodies[j]);
            }
        }
    }
    activeContacts.swap(current);
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