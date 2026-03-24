#pragma once

#include "physix2d/Vec2.h"

namespace physix2d {

// A rigid body: dumb data + integration. Mass <= 0 marks it STATIC
// (infinite mass / immovable), which is how we model floors, walls, etc.
class Body{
public:
    Vec2 pos, vel, acc;
    Vec2 prev_pos;
    Vec2 forceAccumulator;
    float radius;
    float mass;
    float invMass;   // 0 for static bodies -- precomputed to keep the solver branch-free
    float linearDamping = 0.0f; // velocity decay per second; tames the collision jitter

    Body(float x, float y, float r, float m);
    Vec2 getPosition() const;
    Vec2 getVel() const;
    float getRadius() const;
    float getMass() const;
    float getInvMass() const;
    bool isStatic() const;
    void setVel(Vec2 v);
    void setPos(Vec2 pos_);
    void setMass(float m);
    void setDamping(float d);

    // changing the state of a body
    void applyForce(const Vec2& f);
    void clearForces();
    void update(float dt);
};

}
