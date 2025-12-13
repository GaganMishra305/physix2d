#include "physix2d/Collision.h"
#include <cmath>
#include <algorithm>

namespace physix2d {

bool Collision::resolveCircleVsCircle(Body& a, Body& b, float restitution) {
    Vec2 d = b.getPosition() - a.getPosition();
    float dist2 = d.x*d.x + d.y*d.y;
    float r = a.getRadius() + b.getRadius();
    if (dist2 >= r*r) return false;

    float dist = std::sqrt(std::max(dist2, 1e-6f));
    Vec2 n(d.x / dist, d.y / dist);
    float penetration = r - dist;

    // Positional correction (Baumgarte)
    const float percent = 0.6f;
    const float slop = 0.01f;
    float invA = 1.0f / a.getMass();
    float invB = 1.0f / b.getMass();
    float corrMag = std::max(penetration - slop, 0.0f) * percent / (invA + invB);
    Vec2 correction(n.x * corrMag, n.y * corrMag);
    Vec2 aPos = a.getPosition();
    Vec2 bPos = b.getPosition();
    a.setPos(Vec2(aPos.x - correction.x * invA, aPos.y - correction.y * invA));
    b.setPos(Vec2(bPos.x + correction.x * invB, bPos.y + correction.y * invB));

    // Velocity impulse
    Vec2 rv(b.getVel().x - a.getVel().x, b.getVel().y - a.getVel().y);
    float velAlongNormal = rv.x*n.x + rv.y*n.y;
    if (velAlongNormal > 0) return true;
    float j = -(1.0f + restitution) * velAlongNormal / (invA + invB);
    Vec2 impulse(n.x*j, n.y*j);
    a.setVel(Vec2(a.getVel().x - impulse.x * invA,  a.getVel().y - impulse.y * invA));
    b.setVel(Vec2(b.getVel().x + impulse.x * invB,  b.getVel().y + impulse.y * invB));
    return true;
}

void Collision::resolveWallCollisions(Body& b, float minX, float maxX, float minY, float maxY, float restitution) {
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