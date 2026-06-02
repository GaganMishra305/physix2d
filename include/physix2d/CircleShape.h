#pragma once

#include "physix2d/Shape.h"

namespace physix2d {

class CircleShape : public Shape {
public:
    float radius;

    explicit CircleShape(float r);
    ShapeType getType() const override;
    AABB computeAABB(const Vec2& center, float angle) const override;
    float computeInertia(float mass) const override;
};

}
