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
    void addBody(Body body);
    void addForce(Vec2 force);
    std::vector<Body>& getBodies();
    void update(float dt);
};

}