#pragma once

#include "physix2d/Vec2.h"

namespace physix2d {

class Body{
private:
    Vec2 pos, vel, acc;
    float radius;
    float mass;

public:
    Body(float x, float y, float r, float m);
    Vec2 getPosition() const;
    float getRadius() const;
    float getMass() const;
    
    void applyForce(Vec2 force);
    void update();
};

}