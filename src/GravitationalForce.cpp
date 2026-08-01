#include "physix2d/GravitationalForce.h"
#include <cmath>

namespace physix2d {

GravitationalForce::GravitationalForce(const std::vector<Body*>& targets, float G_, float softening_)
    : bodies(targets), G(G_), softening(softening_) {}

void GravitationalForce::apply(float) {
    float eps2 = softening * softening;
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = i + 1; j < bodies.size(); ++j) {
            Body* a = bodies[i];
            Body* b = bodies[j];
            Vec2 d = b->pos - a->pos;
            float dist2 = d.lengthSq() + eps2;
            float invDist = 1.0f / std::sqrt(dist2);
            Vec2 n = d * invDist;                    // unit direction a -> b
            float mag = G * a->getMass() * b->getMass() / dist2;
            Vec2 f = n * mag;
            a->applyForce(f);        // a pulled toward b
            b->applyForce(f * -1.0f); // b pulled toward a
        }
    }
}

void GravitationalForce::addBody(Body* body) {
    bodies.push_back(body);
}

}
