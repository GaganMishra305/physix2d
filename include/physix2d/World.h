#pragma once

#include <vector>
#include "physix2d/Vec2.h"
#include "physix2d/Body.h"


namespace physix2d {

class World{
private:
    std::vector<Body> bodies;
    std::vector<Vec2> forces;

public:
    std::vector<Body>& getBodies();

    // change the world
    void addBody(Body body);
    void addForce(Vec2 force);
    void update(float dt);
};

}