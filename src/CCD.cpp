#include "physix2d/CCD.h"
#include "physix2d/CircleShape.h"
#include <cmath>

namespace physix2d {

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

}
