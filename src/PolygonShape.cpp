#include "physix2d/PolygonShape.h"
#include <cmath>
#include <limits>

namespace physix2d {

namespace {
// Rotate a local-space point by angle (radians).
Vec2 rotate(const Vec2& v, float c, float s) {
    return Vec2(v.x * c - v.y * s, v.x * s + v.y * c);
}
}

PolygonShape::PolygonShape(const std::vector<Vec2>& verts) : vertices(verts) {}

PolygonShape PolygonShape::box(float hw, float hh) {
    return PolygonShape({
        Vec2(-hw, -hh),
        Vec2( hw, -hh),
        Vec2( hw,  hh),
        Vec2(-hw,  hh)
    });
}

ShapeType PolygonShape::getType() const {
    return ShapeType::Polygon;
}

AABB PolygonShape::computeAABB(const Vec2& center, float angle) const {
    float c = std::cos(angle), s = std::sin(angle);
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = -std::numeric_limits<float>::max();
    float maxY = -std::numeric_limits<float>::max();
    for (const Vec2& v : vertices) {
        Vec2 w = rotate(v, c, s);
        minX = std::min(minX, w.x);
        minY = std::min(minY, w.y);
        maxX = std::max(maxX, w.x);
        maxY = std::max(maxY, w.y);
    }
    return AABB{
        Vec2(center.x + minX, center.y + minY),
        Vec2(center.x + maxX, center.y + maxY)
    };
}

float PolygonShape::computeInertia(float mass) const {
    // Standard convex-polygon inertia about the centroid (vertices are
    // already centroid-relative). I = (m/6) * Σ cross(Pi,Pi+1) * (Pi·Pi + Pi·Pi+1 + Pi+1·Pi+1) / Σ cross(Pi,Pi+1)
    float numerator = 0.0f;
    float denominator = 0.0f;
    size_t n = vertices.size();
    for (size_t i = 0; i < n; ++i) {
        const Vec2& a = vertices[i];
        const Vec2& b = vertices[(i + 1) % n];
        float cross = std::abs(a.cross(b));
        numerator += cross * (a.dot(a) + a.dot(b) + b.dot(b));
        denominator += cross;
    }
    if (denominator <= 1e-8f) return 0.0f;
    return (mass / 6.0f) * (numerator / denominator);
}

std::vector<Vec2> PolygonShape::normals() const {
    std::vector<Vec2> ns;
    size_t n = vertices.size();
    ns.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        Vec2 edge = vertices[(i + 1) % n] - vertices[i];
        // Outward normal for CCW winding: (dy, -dx), normalized.
        ns.push_back(Vec2(edge.y, -edge.x).normalized());
    }
    return ns;
}

}
