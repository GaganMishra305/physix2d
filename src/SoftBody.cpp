#include "physix2d/SoftBody.h"
#include <cmath>

namespace physix2d {

int SoftBody::addPoint(const Vec2& p, float mass, bool pinned_) {
    PointMass pm;
    pm.pos = p;
    pm.prev = p;
    pm.pinned = pinned_;
    pm.invMass = (pinned_ || mass <= 0.0f) ? 0.0f : 1.0f / mass;
    points.push_back(pm);
    return static_cast<int>(points.size()) - 1;
}

void SoftBody::addConstraint(int a, int b, float stiffness, float rest) {
    if (rest < 0.0f) rest = Vec2::distance(points[a].pos, points[b].pos);
    constraints.push_back({a, b, rest, stiffness});
}

void SoftBody::pin(int index) {
    points[index].pinned = true;
    points[index].invMass = 0.0f;
    points[index].prev = points[index].pos;
}

void SoftBody::step(float dt) {
    Vec2 accel = gravity + wind;
    float damp = 1.0f - damping;
    float dt2 = dt * dt;

    // Verlet integration.
    for (PointMass& p : points) {
        if (p.invMass == 0.0f) { p.prev = p.pos; continue; }
        Vec2 velocity = (p.pos - p.prev) * damp;
        Vec2 next = p.pos + velocity + accel * dt2;
        p.prev = p.pos;
        p.pos = next;
    }

    // Constraint relaxation (Gauss-Seidel).
    for (int it = 0; it < iterations; ++it) {
        for (const DistanceConstraint& c : constraints) {
            PointMass& pa = points[c.a];
            PointMass& pb = points[c.b];
            Vec2 delta = pb.pos - pa.pos;
            float d = delta.getMag();
            if (d < 1e-6f) continue;
            float wSum = pa.invMass + pb.invMass;
            if (wSum <= 0.0f) continue;
            float diff = (d - c.rest) / d;
            Vec2 corr = delta * (c.stiffness * diff);
            pa.pos = pa.pos + corr * (pa.invMass / wSum);
            pb.pos = pb.pos - corr * (pb.invMass / wSum);
        }

        if (hasBounds) {
            for (PointMass& p : points) {
                if (p.pos.x < bounds.min.x) p.pos.x = bounds.min.x;
                if (p.pos.x > bounds.max.x) p.pos.x = bounds.max.x;
                if (p.pos.y < bounds.min.y) p.pos.y = bounds.min.y;
                if (p.pos.y > bounds.max.y) p.pos.y = bounds.max.y;
            }
        }
    }
}

SoftBody SoftBody::rope(const Vec2& start, const Vec2& end, int segments, float mass) {
    SoftBody sb;
    for (int i = 0; i <= segments; ++i) {
        float t = static_cast<float>(i) / segments;
        sb.addPoint(start + (end - start) * t, mass);
    }
    for (int i = 0; i < segments; ++i)
        sb.addConstraint(i, i + 1, 1.0f);
    return sb;
}

SoftBody SoftBody::cloth(const Vec2& topLeft, float width, float height,
                         int cols, int rows, float mass) {
    SoftBody sb;
    auto idx = [cols](int x, int y) { return y * (cols + 1) + x; };
    float dx = width / cols;
    float dy = height / rows;

    for (int y = 0; y <= rows; ++y)
        for (int x = 0; x <= cols; ++x)
            sb.addPoint(Vec2(topLeft.x + x * dx, topLeft.y + y * dy), mass);

    // Structural constraints (right & down neighbours).
    for (int y = 0; y <= rows; ++y)
        for (int x = 0; x <= cols; ++x) {
            if (x < cols) sb.addConstraint(idx(x, y), idx(x + 1, y), 1.0f);
            if (y < rows) sb.addConstraint(idx(x, y), idx(x, y + 1), 1.0f);
        }
    // Shear constraints (diagonals) so the sheet resists collapsing.
    for (int y = 0; y < rows; ++y)
        for (int x = 0; x < cols; ++x) {
            sb.addConstraint(idx(x, y), idx(x + 1, y + 1), 0.5f);
            sb.addConstraint(idx(x + 1, y), idx(x, y + 1), 0.5f);
        }
    return sb;
}

}
