#pragma once

#include "physix2d/Body.h"

namespace physix2d {

// Continuous Collision Detection: time-of-impact (TOI) queries for fast-moving
// bodies that would otherwise tunnel through thin/other geometry in one step.
// TOI is returned in [0,1] as a fraction of the body's sweep this step
// (prev_pos -> pos). A return value > 1 means "no impact this step".
class CCD {
public:
    static constexpr float NO_HIT = 2.0f;

    // Swept `mover` (prev_pos -> pos) vs a stationary `target` circle.
    static float circleVsCircle(const Body& mover, const Body& target);
};

}
