#include "physix2d/Collision.h"
#include <cmath>
#include <algorithm>

namespace physix2d {

bool Collision::resolveCircleVsCircle(Body& a, Body& b, float restitution) {
    Vec2 d(b.getPosition().getX() - a.getPosition().getX(),
           b.getPosition().getY() - a.getPosition().getY());
    float dist2 = d.getX()*d.getX() + d.getY()*d.getY();
    float r = a.getRadius() + b.getRadius();
    if (dist2 >= r*r) return false;

    float dist = std::sqrt(std::max(dist2, 1e-6f));
    Vec2 n(d.getX() / dist, d.getY() / dist);
    float penetration = r - dist;

    // Positional correction (Baumgarte)
    const float percent = 0.6f;
    const float slop = 0.01f;
    float invA = 1.0f / a.getMass();
    float invB = 1.0f / b.getMass();
    float corrMag = std::max(penetration - slop, 0.0f) * percent / (invA + invB);
    Vec2 correction(n.getX() * corrMag, n.getY() * corrMag);

    Vec2 aPos = a.getPosition();
    Vec2 bPos = b.getPosition();
    a.setPos(Vec2(aPos.getX() - correction.getX() * invA,
                       aPos.getY() - correction.getY() * invA));
    b.setPos(Vec2(bPos.getX() + correction.getX() * invB,
                       bPos.getY() + correction.getY() * invB));

    // Velocity impulse
    Vec2 rv(b.getVel().getX() - a.getVel().getX(),
            b.getVel().getY() - a.getVel().getY());
    float velAlongNormal = rv.getX()*n.getX() + rv.getY()*n.getY();
    if (velAlongNormal > 0) return true;

    float j = -(1.0f + restitution) * velAlongNormal / (invA + invB);
    Vec2 impulse(n.getX()*j, n.getY()*j);
    a.setVel(Vec2(a.getVel().getX() - impulse.getX() * invA,
                  a.getVel().getY() - impulse.getY() * invA));
    b.setVel(Vec2(b.getVel().getX() + impulse.getX() * invB,
                  b.getVel().getY() + impulse.getY() * invB));
    return true;
}

void Collision::resolveWallCollisions(Body& b, float minX, float maxX, float minY, float maxY, float restitution) {
    Vec2 pos = b.getPosition();
    Vec2 vel = b.getVel();
    float r = b.getRadius();
    
    const float slop = 0.01f;
    const float percent = 0.8f;  // stronger correction for walls

    // Bottom wall (y + r > maxY)
    if (pos.getY() + r > maxY) {
        float penetration = (pos.getY() + r) - maxY;
        float correction = std::max(penetration - slop, 0.0f) * percent;
        b.setPos(Vec2(pos.getX(), pos.getY() - correction));
        
        if (vel.getY() > 0) {  // Only reflect if moving into wall
            b.setVel(Vec2(vel.getX(), -vel.getY() * restitution));
        }
    }
    // Top wall (y - r < minY)
    else if (pos.getY() - r < minY) {
        float penetration = minY - (pos.getY() - r);
        float correction = std::max(penetration - slop, 0.0f) * percent;
        b.setPos(Vec2(pos.getX(), pos.getY() + correction));
        
        if (vel.getY() < 0) {
            b.setVel(Vec2(vel.getX(), -vel.getY() * restitution));
        }
    }

    // Right wall (x + r > maxX)
    if (pos.getX() + r > maxX) {
        float penetration = (pos.getX() + r) - maxX;
        float correction = std::max(penetration - slop, 0.0f) * percent;
        b.setPos(Vec2(pos.getX() - correction, pos.getY()));
        
        if (vel.getX() > 0) {
            b.setVel(Vec2(-vel.getX() * restitution, vel.getY()));
        }
    }
    // Left wall (x - r < minX)
    else if (pos.getX() - r < minX) {
        float penetration = minX - (pos.getX() - r);
        float correction = std::max(penetration - slop, 0.0f) * percent;
        b.setPos(Vec2(pos.getX() + correction, pos.getY()));
        
        if (vel.getX() < 0) {
            b.setVel(Vec2(-vel.getX() * restitution, vel.getY()));
        }
    }
}

}