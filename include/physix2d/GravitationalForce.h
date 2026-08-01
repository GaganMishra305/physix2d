#pragma once
#include "Force.h"
#include "Body.h"
#include "Vec2.h"
#include <vector>

namespace physix2d {

// Newtonian N-body gravity: every body in the set attracts every other with
// F = G * m1 * m2 / (r^2 + softening^2), applied along the line between them.
// The softening term keeps the force finite during close encounters so the
// integrator doesn't explode when two bodies nearly overlap.
class GravitationalForce : public Force {
    std::vector<Body*> bodies;
    float G;
    float softening;

public:
    GravitationalForce(const std::vector<Body*>& targets, float G_ = 6.674e-1f,
                       float softening_ = 8.0f);
    void apply(float dt) override;
    void addBody(Body* body);
};

}
