#pragma once
#include "Force.h"
#include "Body.h"
#include "Vec2.h"
#include <vector>

namespace physix2d {

class GravityForce : public Force {
    std::vector<Body*> bodies;
    Vec2 g;

public:
    GravityForce(const Vec2& gravity, const std::vector<Body*>& targets);
    void apply(float dt) override;
    void addBody(Body* body);
};

}
