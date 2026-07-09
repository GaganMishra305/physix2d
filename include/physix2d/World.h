#pragma once

#include <deque>
#include <vector>
#include <functional>
#include <unordered_set>
#include <cstdint>
#include "physix2d/Vec2.h"
#include "physix2d/Body.h"
#include "physix2d/Force.h"
#include "physix2d/Joint.h"
#include <memory>

namespace physix2d {

using ContactCallback = std::function<void(Body&, Body&)>;

class World{
private:
    std::deque<Body> bodies;          // push dont reallocate-memory -> so force still have access to the bodies
    std::vector<std::unique_ptr<Force>> forces;
    std::vector<std::unique_ptr<Joint>> joints;
    int jointIterations = 8;          // constraint solver passes per step
    std::unordered_set<uint64_t> activeContacts; // pairs touching last step
    void detectContacts();

public:
    // Optional collision event hooks. When either is set, World runs a
    // narrow-phase pass each step to fire begin/end contact events.
    ContactCallback onBeginContact;
    ContactCallback onEndContact;
    float contactCellSize = 64.0f;    // broad-phase cell for contact detection

    void update(float dt);
    Body& addBody(const Body& body);
    void addForce(std::unique_ptr<Force> force);
    void addJoint(std::unique_ptr<Joint> joint);
    void setJointIterations(int n);
    std::deque<Body>& getBodies();    
};

}