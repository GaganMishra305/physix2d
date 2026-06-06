#pragma once

#include "physix2d/Shape.h"
#include <vector>

namespace physix2d {

// Convex polygon, vertices in local space (CCW), centered on the centroid.
class PolygonShape : public Shape {
public:
    std::vector<Vec2> vertices;

    explicit PolygonShape(const std::vector<Vec2>& verts);

    // Axis-aligned box centered at the origin.
    static PolygonShape box(float halfWidth, float halfHeight);

    ShapeType getType() const override;
    AABB computeAABB(const Vec2& center, float angle) const override;
    float computeInertia(float mass) const override;

    // Outward unit edge normals in local space (SAT axes).
    std::vector<Vec2> normals() const;
};

}
