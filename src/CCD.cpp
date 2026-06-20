#include "physix2d/CCD.h"
#include "physix2d/CircleShape.h"
#include "physix2d/PolygonShape.h"
#include <cmath>
#include <vector>
#include <limits>

namespace physix2d {

namespace {
// Earliest root of |start + t*s - p|^2 = r^2 in [0,1], else NO_HIT.
float sweptPoint(const Vec2& start, const Vec2& s, const Vec2& p, float r) {
    Vec2 e = start - p;
    float a = s.dot(s);
    float b = 2.0f * e.dot(s);
    float c = e.dot(e) - r * r;
    if (c <= 0.0f) return 0.0f;
    if (a <= 1e-12f) return CCD::NO_HIT;
    float disc = b * b - 4.0f * a * c;
    if (disc < 0.0f) return CCD::NO_HIT;
    float t = (-b - std::sqrt(disc)) / (2.0f * a);
    if (t < 0.0f || t > 1.0f) return CCD::NO_HIT;
    return t;
}
}

float CCD::circleVsCircle(const Body& mover, const Body& target) {
    // Sweep of the mover this step.
    Vec2 s = mover.pos - mover.prev_pos;
    float rSum = mover.getRadius() + target.getRadius();

    // Solve |prev + t*s - target|^2 = rSum^2 for the earliest t in [0,1].
    Vec2 e = mover.prev_pos - target.pos;
    float a = s.dot(s);
    float b = 2.0f * e.dot(s);
    float c = e.dot(e) - rSum * rSum;

    if (c <= 0.0f) return 0.0f;      // already overlapping at the start
    if (a <= 1e-12f) return NO_HIT;  // mover isn't moving -> no sweep

    float disc = b * b - 4.0f * a * c;
    if (disc < 0.0f) return NO_HIT;  // path never reaches the target

    float t = (-b - std::sqrt(disc)) / (2.0f * a); // earliest root
    if (t < 0.0f || t > 1.0f) return NO_HIT;
    return t;
}

float CCD::circleVsPolygon(const Body& mover, const Body& target) {
    auto* poly = dynamic_cast<PolygonShape*>(target.getShape());
    if (!poly) return NO_HIT;

    // World-space vertices and outward normals of the target polygon.
    float c = std::cos(target.angle), sn = std::sin(target.angle);
    size_t n = poly->vertices.size();
    std::vector<Vec2> v(n);
    for (size_t i = 0; i < n; ++i) {
        const Vec2& l = poly->vertices[i];
        v[i] = target.pos + Vec2(l.x * c - l.y * sn, l.x * sn + l.y * c);
    }
    auto localN = poly->normals();
    std::vector<Vec2> nrm(n);
    for (size_t i = 0; i < n; ++i)
        nrm[i] = Vec2(localN[i].x * c - localN[i].y * sn, localN[i].x * sn + localN[i].y * c);

    Vec2 start = mover.prev_pos;
    Vec2 s = mover.pos - mover.prev_pos;
    float r = mover.getRadius();
    float best = NO_HIT;

    // Face tests: cast against each edge plane pushed outward by r.
    for (size_t i = 0; i < n; ++i) {
        Vec2 v1 = v[i], v2 = v[(i + 1) % n];
        Vec2 nn = nrm[i];
        float denom = nn.dot(s);
        if (denom >= -1e-8f) continue; // parallel or moving away from this face
        float d = nn.dot(v1) + r;       // expanded plane offset
        float t = (d - nn.dot(start)) / denom;
        if (t < 0.0f || t > 1.0f) continue;
        Vec2 P = start + s * t;
        Vec2 edge = v2 - v1;
        float u = (P - v1).dot(edge) / edge.dot(edge);
        if (u >= 0.0f && u <= 1.0f && t < best) best = t;
    }

    // Vertex caps: swept circle vs each corner (rounds the face regions off).
    for (size_t i = 0; i < n; ++i) {
        float t = sweptPoint(start, s, v[i], r);
        if (t < best) best = t;
    }

    return best;
}

}
