#pragma once

#include <vector>
#include "physix2d/Vec2.h"
#include "physix2d/Shape.h"   // AABB

namespace physix2d {

// A point mass for Verlet integration. Velocity is implicit (pos - prev), which
// is exactly why position-based constraints stay stable here: any positional
// correction is automatically reflected in the next step's velocity.
struct PointMass {
    Vec2 pos;
    Vec2 prev;
    float invMass = 1.0f;  // 0 => pinned / immovable
    bool pinned = false;
};

// A distance constraint between two point masses (a spring at stiffness 1).
struct DistanceConstraint {
    int a, b;
    float rest;
    float stiffness; // 0..1
};

// Soft body: a mesh of point masses tied together by distance constraints,
// solved with position-based dynamics. Ropes, cloth, and jelly are all just
// different point/constraint layouts.
class SoftBody {
public:
    std::vector<PointMass> points;
    std::vector<DistanceConstraint> constraints;

    Vec2 gravity{0.0f, 0.0f};
    Vec2 wind{0.0f, 0.0f};       // treated as an acceleration
    float damping = 0.01f;       // velocity decay
    int iterations = 8;          // constraint relaxation passes
    bool hasBounds = false;
    AABB bounds{Vec2(0,0), Vec2(0,0)};

    int addPoint(const Vec2& p, float mass, bool pinned = false);
    void addConstraint(int a, int b, float stiffness = 1.0f, float rest = -1.0f);
    void pin(int index);

    void step(float dt);

    // Factories.
    static SoftBody rope(const Vec2& start, const Vec2& end, int segments, float mass);
    static SoftBody cloth(const Vec2& topLeft, float width, float height,
                          int cols, int rows, float mass);
};

}
