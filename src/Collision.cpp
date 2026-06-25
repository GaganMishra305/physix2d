#include "physix2d/Collision.h"
#include <cmath>
#include <algorithm>

namespace physix2d {

namespace {
// scalar-cross-vector: w x r  =>  (-w*r.y, w*r.x)
inline Vec2 crossSV(float w, const Vec2& r) { return Vec2(-w * r.y, w * r.x); }
// vector-cross-vector (returns scalar z): a x b
inline float crossVV(const Vec2& a, const Vec2& b) { return a.x * b.y - a.y * b.x; }
}

bool Collision::resolve(Body& a, Body& b, float restitution) {
    Manifold m = CollisionDetector::detect(a, b);
    if (!m.hit) return false;
    resolveManifold(a, b, m, restitution);
    return true;
}

void Collision::resolveManifold(Body& a, Body& b, const Manifold& m, float restitution) {
    float invMassA = a.getInvMass(), invMassB = b.getInvMass();
    float invIA = a.getInvInertia(), invIB = b.getInvInertia();
    float invSum = invMassA + invMassB;
    if (invSum == 0.0f) return; // both static

    const Vec2& n = m.normal;
    int count = m.contactCount > 0 ? m.contactCount : 1;
    float mu = std::sqrt(a.friction * b.friction);

    for (int i = 0; i < m.contactCount; ++i) {
        Vec2 contact = m.contacts[i];
        Vec2 ra = contact - a.pos;
        Vec2 rb = contact - b.pos;

        // Relative velocity at the contact point.
        Vec2 va = a.vel + crossSV(a.angularVel, ra);
        Vec2 vb = b.vel + crossSV(b.angularVel, rb);
        Vec2 rv = vb - va;
        float velAlongNormal = rv.dot(n);
        if (velAlongNormal > 0.0f) continue; // separating

        float raCrossN = crossVV(ra, n);
        float rbCrossN = crossVV(rb, n);
        float denom = invSum + invIA * raCrossN * raCrossN + invIB * rbCrossN * rbCrossN;
        if (denom <= 1e-12f) continue;

        float j = -(1.0f + restitution) * velAlongNormal / denom;
        j /= count; // distribute across contacts
        Vec2 impulse = n * j;

        a.vel = a.vel - impulse * invMassA;
        a.angularVel -= invIA * crossVV(ra, impulse);
        b.vel = b.vel + impulse * invMassB;
        b.angularVel += invIB * crossVV(rb, impulse);

        // --- Friction (recompute rv after normal impulse) ---
        va = a.vel + crossSV(a.angularVel, ra);
        vb = b.vel + crossSV(b.angularVel, rb);
        rv = vb - va;
        Vec2 t = rv - n * rv.dot(n);
        if (t.lengthSq() <= 1e-8f) continue;
        t = t.normalized();
        float raCrossT = crossVV(ra, t);
        float rbCrossT = crossVV(rb, t);
        float denomT = invSum + invIA * raCrossT * raCrossT + invIB * rbCrossT * rbCrossT;
        if (denomT <= 1e-12f) continue;
        float jt = -rv.dot(t) / denomT;
        jt /= count;
        float maxF = mu * std::abs(j);
        jt = std::max(-maxF, std::min(jt, maxF));
        Vec2 frictionImpulse = t * jt;

        a.vel = a.vel - frictionImpulse * invMassA;
        a.angularVel -= invIA * crossVV(ra, frictionImpulse);
        b.vel = b.vel + frictionImpulse * invMassB;
        b.angularVel += invIB * crossVV(rb, frictionImpulse);
    }

    // Positional correction (Baumgarte), linear only.
    const float percent = 0.4f;
    const float slop = 0.02f;
    float corr = std::max(m.penetration - slop, 0.0f) / invSum * percent;
    Vec2 correction = n * corr;
    a.setPos(a.pos - correction * invMassA);
    b.setPos(b.pos + correction * invMassB);
}

bool Collision::resolveCircleVsCircle(Body& a, Body& b, float restitution) {
    Vec2 d = b.getPosition() - a.getPosition();
    float dist2 = d.x*d.x + d.y*d.y;
    float r = a.getRadius() + b.getRadius();
    if (dist2 >= r*r) return false;

    float dist = std::sqrt(std::max(dist2, 1e-6f));
    Vec2 n(d.x / dist, d.y / dist);
    float penetration = r - dist;

    float invA = a.getInvMass();
    float invB = b.getInvMass();
    float invSum = invA + invB;
    if (invSum == 0.0f) return true; // both static: nothing to resolve

    // Positional correction (Baumgarte)
    const float percent = 0.6f;
    const float slop = 0.01f;
    float corrMag = std::max(penetration - slop, 0.0f) * percent / invSum;
    Vec2 correction(n.x * corrMag, n.y * corrMag);
    Vec2 aPos = a.getPosition();
    Vec2 bPos = b.getPosition();
    a.setPos(Vec2(aPos.x - correction.x * invA, aPos.y - correction.y * invA));
    b.setPos(Vec2(bPos.x + correction.x * invB, bPos.y + correction.y * invB));

    // Velocity impulse
    Vec2 rv(b.getVel().x - a.getVel().x, b.getVel().y - a.getVel().y);
    float velAlongNormal = rv.x*n.x + rv.y*n.y;
    if (velAlongNormal > 0) return true;
    float j = -(1.0f + restitution) * velAlongNormal / invSum;
    Vec2 impulse = n * j;
    a.setVel(a.getVel() - impulse * invA);
    b.setVel(b.getVel() + impulse * invB);

    // Coulomb friction (tangential impulse), clamped to the friction cone.
    Vec2 rv2 = b.getVel() - a.getVel();
    Vec2 tangent = rv2 - n * rv2.dot(n);
    if (tangent.lengthSq() > 1e-8f) {
        tangent = tangent.normalized();
        float jt = -rv2.dot(tangent) / invSum;
        float mu = std::sqrt(a.friction * b.friction);
        float maxFriction = mu * std::abs(j);
        jt = std::max(-maxFriction, std::min(jt, maxFriction));
        Vec2 frictionImpulse = tangent * jt;
        a.setVel(a.getVel() - frictionImpulse * invA);
        b.setVel(b.getVel() + frictionImpulse * invB);
    }
    return true;
}

void Collision::resolveWallCollisions(Body& b, float minX, float maxX, float minY, float maxY, float restitution) {
    if (b.isStatic()) return; // walls can't push an immovable body
    Vec2 pos = b.getPosition();
    Vec2 vel = b.getVel();
    float r = b.getRadius();
    
    const float slop = 0.01f;
    const float percent = 0.8f;  // stronger correction for walls

    // Bottom wall (y + r > maxY)
    if (pos.y + r > maxY) {
        float penetration = (pos.y + r) - maxY;
        float correction = std::max(penetration - slop, 0.0f) * percent;
        b.setPos(Vec2(pos.x, pos.y - correction));
        
        if (vel.y > 0) {  // Only reflect if moving into wall
            b.setVel(Vec2(vel.x, -vel.y * restitution));
        }
    }

    // Top wall (y - r < minY)
    else if (pos.y - r < minY) {
        float penetration = minY - (pos.y - r);
        float correction = std::max(penetration - slop, 0.0f) * percent;
        b.setPos(Vec2(pos.x, pos.y + correction));
        
        if (vel.y < 0) {
            b.setVel(Vec2(vel.x, -vel.y * restitution));
        }
    }

    // Right wall (x + r > maxX)
    if (pos.x + r > maxX) {
        float penetration = (pos.x + r) - maxX;
        float correction = std::max(penetration - slop, 0.0f) * percent;
        b.setPos(Vec2(pos.x - correction, pos.y));
        
        if (vel.x > 0) {
            b.setVel(Vec2(-vel.x * restitution, vel.y));
        }
    }
    
    // Left wall (x - r < minX)
    else if (pos.x - r < minX) {
        float penetration = minX - (pos.x - r);
        float correction = std::max(penetration - slop, 0.0f) * percent;
        b.setPos(Vec2(pos.x + correction, pos.y));
        
        if (vel.x < 0) {
            b.setVel(Vec2(-vel.x * restitution, vel.y));
        }
    }
}

}