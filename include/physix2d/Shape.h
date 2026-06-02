#pragma once

#include "physix2d/Vec2.h"

namespace physix2d {

enum class ShapeType { Circle, Polygon };

// Axis-aligned bounding box (world space) -- used by the broad phase & culling.
struct AABB {
    Vec2 min;
    Vec2 max;

    bool overlaps(const AABB& o) const {
        return min.x <= o.max.x && max.x >= o.min.x &&
               min.y <= o.max.y && max.y >= o.min.y;
    }
};

// Abstract collision shape. Geometry is defined in the body's local space;
// the owning Body supplies world position and orientation (radians).
class Shape {
public:
    virtual ~Shape() = default;
    virtual ShapeType getType() const = 0;

    // World-space AABB given the body's center and rotation.
    virtual AABB computeAABB(const Vec2& center, float angle) const = 0;

    // Moment of inertia about the shape's centroid for the given mass.
    virtual float computeInertia(float mass) const = 0;
};

}
