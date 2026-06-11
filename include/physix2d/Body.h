#pragma once

#include "physix2d/Vec2.h"
#include "physix2d/Shape.h"
#include <memory>

namespace physix2d {

// A rigid body: dumb data + integration. Owns a collision Shape and carries
// both linear and angular state. Mass <= 0 marks it STATIC (infinite mass).
class Body{
public:
    Vec2 pos, vel, acc;
    Vec2 prev_pos;
    Vec2 forceAccumulator;

    // Angular state
    float angle = 0.0f;       // orientation in radians
    float angularVel = 0.0f;
    float torque = 0.0f;

    float radius;             // bounding radius (circle radius / polygon circumradius) for broad phase
    float mass;
    float invMass;            // 0 for static bodies
    float inertia = 0.0f;
    float invInertia = 0.0f;  // 0 for static or rotation-locked bodies
    float linearDamping = 0.0f;
    float angularDamping = 0.0f;
    float friction = 0.0f;    // Coulomb friction coefficient [0..1]

    std::shared_ptr<Shape> shape;

    // Circle body (legacy-friendly signature).
    Body(float x, float y, float r, float m);
    // Body with an arbitrary shape.
    Body(float x, float y, std::shared_ptr<Shape> shape_, float m);

    Vec2 getPosition() const;
    Vec2 getVel() const;
    float getRadius() const;
    float getMass() const;
    float getInvMass() const;
    float getInvInertia() const;
    float getAngle() const;
    bool isStatic() const;
    Shape* getShape() const;

    void setVel(Vec2 v);
    void setPos(Vec2 pos_);
    void setAngle(float a);
    void setMass(float m);
    void setDamping(float d);
    void setFriction(float f);

    // changing the state of a body
    void applyForce(const Vec2& f);
    void applyTorque(float t);
    void clearForces();
    void update(float dt);

private:
    void recomputeInertia();
    void recomputeBoundingRadius();
};

}
