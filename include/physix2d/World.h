#pragma once

#include <deque>
#include <vector>
#include "physix2d/Vec2.h"
#include "physix2d/Body.h"
#include "physix2d/Force.h"
#include "physix2d/Joint.h"
#include <memory>

namespace physix2d {

class World{
private:
    std::deque<Body> bodies;          // push dont reallocate-memory -> so force still have access to the bodies
    std::vector<std::unique_ptr<Force>> forces;
    std::vector<std::unique_ptr<Joint>> joints;
    int jointIterations = 8;          // constraint solver passes per step

public:
    void update(float dt);
    Body& addBody(const Body& body);
    void addForce(std::unique_ptr<Force> force);
    void addJoint(std::unique_ptr<Joint> joint);
    void setJointIterations(int n);
    std::deque<Body>& getBodies();    
};

}