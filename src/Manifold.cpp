#include "physix2d/Manifold.h"
#include "physix2d/CircleShape.h"
#include "physix2d/PolygonShape.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>

namespace physix2d {

namespace {

// Transform a polygon's local vertices into world space (rotate + translate).
std::vector<Vec2> worldVerts(const Body& body, const PolygonShape* poly) {
    float c = std::cos(body.angle), s = std::sin(body.angle);
    std::vector<Vec2> out;
    out.reserve(poly->vertices.size());
    for (const Vec2& v : poly->vertices) {
        Vec2 r(v.x * c - v.y * s, v.x * s + v.y * c);
        out.push_back(body.pos + r);
    }
    return out;
}

// Rotate a polygon's local edge normals into world space.
std::vector<Vec2> worldNormals(const Body& body, const PolygonShape* poly) {
    float c = std::cos(body.angle), s = std::sin(body.angle);
    std::vector<Vec2> out;
    auto ns = poly->normals();
    out.reserve(ns.size());
    for (const Vec2& n : ns)
        out.push_back(Vec2(n.x * c - n.y * s, n.x * s + n.y * c));
    return out;
}

// Support point: the vertex farthest along dir.
Vec2 support(const std::vector<Vec2>& verts, const Vec2& dir) {
    float best = -std::numeric_limits<float>::max();
    Vec2 bestV = verts[0];
    for (const Vec2& v : verts) {
        float d = v.dot(dir);
        if (d > best) { best = d; bestV = v; }
    }
    return bestV;
}

// Largest separation of B from A's faces. Returns separation and face index.
float maxSeparation(const std::vector<Vec2>& vA, const std::vector<Vec2>& nA,
                    const std::vector<Vec2>& vB, int& faceIndex) {
    float best = -std::numeric_limits<float>::max();
    faceIndex = 0;
    for (size_t i = 0; i < vA.size(); ++i) {
        Vec2 n = nA[i];
        Vec2 s = support(vB, -n);       // deepest point of B into face i
        float sep = n.dot(s - vA[i]);
        if (sep > best) { best = sep; faceIndex = static_cast<int>(i); }
    }
    return best;
}

// Clip segment [a,b] to the half-plane n.dot(p) <= offset. Returns kept points.
int clip(Vec2 n, float offset, Vec2& a, Vec2& b) {
    Vec2 out[2];
    int count = 0;
    float da = n.dot(a) - offset;
    float db = n.dot(b) - offset;
    if (da <= 0.0f) out[count++] = a;
    if (db <= 0.0f) out[count++] = b;
    if (da * db < 0.0f) {              // crossing -> add intersection
        float t = da / (da - db);
        out[count++] = a + (b - a) * t;
    }
    if (count >= 1) a = out[0];
    if (count >= 2) b = out[1];
    return count > 2 ? 2 : count;
}

// ---- circle vs circle ----
Manifold circleVsCircle(Body& A, Body& B) {
    Manifold m;
    auto* ca = static_cast<CircleShape*>(A.getShape());
    auto* cb = static_cast<CircleShape*>(B.getShape());
    Vec2 d = B.pos - A.pos;
    float r = ca->radius + cb->radius;
    float dist2 = d.lengthSq();
    if (dist2 >= r * r) return m;
    float dist = std::sqrt(std::max(dist2, 1e-12f));
    m.hit = true;
    m.normal = (dist > 1e-6f) ? d * (1.0f / dist) : Vec2(1.0f, 0.0f);
    m.penetration = r - dist;
    m.contacts[0] = A.pos + m.normal * ca->radius;
    m.contactCount = 1;
    return m;
}

// ---- circle vs polygon (returns normal from circle -> polygon) ----
Manifold circleVsPolygon(Body& C, Body& P) {
    Manifold m;
    auto* circle = static_cast<CircleShape*>(C.getShape());
    auto* poly = static_cast<PolygonShape*>(P.getShape());
    auto verts = worldVerts(P, poly);
    auto normals = worldNormals(P, poly);
    Vec2 center = C.pos;
    float radius = circle->radius;

    // Face with max separation.
    int face = 0;
    float sep = -std::numeric_limits<float>::max();
    size_t n = verts.size();
    for (size_t i = 0; i < n; ++i) {
        float s = normals[i].dot(center - verts[i]);
        if (s > radius) return m;       // fully separated
        if (s > sep) { sep = s; face = static_cast<int>(i); }
    }

    Vec2 v1 = verts[face];
    Vec2 v2 = verts[(face + 1) % n];

    if (sep < 1e-6f) {
        // Center inside the polygon.
        m.hit = true;
        m.normal = -normals[face];      // circle -> polygon
        m.penetration = radius - sep;
        m.contacts[0] = center + normals[face] * radius;
        m.contactCount = 1;
        return m;
    }

    // Determine Voronoi region of the closest edge.
    float dot1 = (center - v1).dot(v2 - v1);
    float dot2 = (center - v2).dot(v1 - v2);
    Vec2 pushOut; // from polygon surface toward circle center
    if (dot1 <= 0.0f) {
        float d = Vec2::distance(center, v1);
        if (d > radius) return m;
        m.hit = true;
        pushOut = (center - v1) * (1.0f / std::max(d, 1e-6f));
        m.penetration = radius - d;
        m.contacts[0] = v1;
    } else if (dot2 <= 0.0f) {
        float d = Vec2::distance(center, v2);
        if (d > radius) return m;
        m.hit = true;
        pushOut = (center - v2) * (1.0f / std::max(d, 1e-6f));
        m.penetration = radius - d;
        m.contacts[0] = v2;
    } else {
        // Closest to the face interior.
        m.hit = true;
        pushOut = normals[face];
        m.penetration = radius - sep;
        m.contacts[0] = center - normals[face] * radius;
    }
    m.normal = -pushOut;                 // circle -> polygon
    m.contactCount = 1;
    return m;
}

// ---- polygon vs polygon (SAT + face clipping) ----
Manifold polygonVsPolygon(Body& A, Body& B) {
    Manifold m;
    auto* pa = static_cast<PolygonShape*>(A.getShape());
    auto* pb = static_cast<PolygonShape*>(B.getShape());
    auto vA = worldVerts(A, pa), nA = worldNormals(A, pa);
    auto vB = worldVerts(B, pb), nB = worldNormals(B, pb);

    int faceA = 0, faceB = 0;
    float sepA = maxSeparation(vA, nA, vB, faceA);
    if (sepA >= 0.0f) return m;
    float sepB = maxSeparation(vB, nB, vA, faceB);
    if (sepB >= 0.0f) return m;

    // Choose reference (bias to A to reduce flip-flop).
    bool flip;
    const std::vector<Vec2> *refV, *refN, *incV;
    int refFace;
    if (sepB > sepA + 0.001f) {
        refV = &vB; refN = &nB; incV = &vA; refFace = faceB; flip = true;
    } else {
        refV = &vA; refN = &nA; incV = &vB; refFace = faceA; flip = false;
    }

    Vec2 refNormal = (*refN)[refFace];
    size_t nInc = incV->size();

    // Incident face = the incident polygon's face most anti-parallel to refNormal.
    int incFace = 0;
    float minDot = std::numeric_limits<float>::max();
    // incident normals:
    const std::vector<Vec2>& incN = flip ? nA : nB;
    for (size_t i = 0; i < nInc; ++i) {
        float d = refNormal.dot(incN[i]);
        if (d < minDot) { minDot = d; incFace = static_cast<int>(i); }
    }

    Vec2 i1 = (*incV)[incFace];
    Vec2 i2 = (*incV)[(incFace + 1) % nInc];

    // Reference face endpoints + side tangent.
    size_t nRef = refV->size();
    Vec2 r1 = (*refV)[refFace];
    Vec2 r2 = (*refV)[(refFace + 1) % nRef];
    Vec2 tangent = (r2 - r1).normalized();

    // Clip incident segment to the reference face's side planes.
    if (clip(-tangent, (-tangent).dot(r1), i1, i2) < 2) return m;
    if (clip(tangent, tangent.dot(r2), i1, i2) < 2) return m;

    // Keep points behind the reference face; that's the penetration.
    float refOffset = refNormal.dot(r1);
    Vec2 pts[2] = { i1, i2 };
    m.contactCount = 0;
    float deepest = 0.0f;
    for (int i = 0; i < 2; ++i) {
        float sep = refNormal.dot(pts[i]) - refOffset;
        if (sep <= 0.0f) {
            m.contacts[m.contactCount++] = pts[i];
            deepest = std::max(deepest, -sep);
        }
    }
    if (m.contactCount == 0) return m;

    m.hit = true;
    m.penetration = deepest;
    // normal must point A -> B
    m.normal = flip ? -refNormal : refNormal;
    return m;
}

} // namespace

Manifold CollisionDetector::detect(Body& a, Body& b) {
    if (!a.getShape() || !b.getShape()) return Manifold{};
    ShapeType ta = a.getShape()->getType();
    ShapeType tb = b.getShape()->getType();

    if (ta == ShapeType::Circle && tb == ShapeType::Circle)
        return circleVsCircle(a, b);
    if (ta == ShapeType::Circle && tb == ShapeType::Polygon)
        return circleVsPolygon(a, b);
    if (ta == ShapeType::Polygon && tb == ShapeType::Circle) {
        Manifold m = circleVsPolygon(b, a); // normal: circle(b) -> polygon(a) == b -> a
        m.normal = -m.normal;               // flip to a -> b
        return m;
    }
    return polygonVsPolygon(a, b);
}

}
