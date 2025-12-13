#pragma once

#include <deque>
#include <vector>
#include "physix2d/Vec2.h"
#include "physix2d/Body.h"
#include "physix2d/Force.h"
#include <memory>

namespace physix2d {

class World{
private:
    std::deque<Body> bodies;          // push dont reallocate-memory -> so force still have access to the bodies
    std::vector<std::unique_ptr<Force>> forces;

public:
    void update(float dt);
    Body& addBody(const Body& body);
    void addForce(std::unique_ptr<Force> force);
    std::deque<Body>& getBodies();    
};

}