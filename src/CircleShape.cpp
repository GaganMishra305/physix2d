#include "physix2d/CircleShape.h"

namespace physix2d {

CircleShape::CircleShape(float r) : radius(r) {}

ShapeType CircleShape::getType() const {
    return ShapeType::Circle;
}

AABB CircleShape::computeAABB(const Vec2& center, float /*angle*/) const {
    return AABB{
        Vec2(center.x - radius, center.y - radius),
        Vec2(center.x + radius, center.y + radius)
    };
}

float CircleShape::computeInertia(float mass) const {
    // Solid disc about its center: I = 1/2 * m * r^2
    return 0.5f * mass * radius * radius;
}

}
