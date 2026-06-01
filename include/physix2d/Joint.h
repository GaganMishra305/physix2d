#pragma once

namespace physix2d {

// Base class for all constraints/joints. A joint restricts the relative
// motion of the bodies it connects. The World calls solve() each step
// (typically for several iterations) after integrating forces.
class Joint {
public:
    virtual ~Joint() = default;

    // Apply the constraint for this sub-step. dt is the fixed timestep.
    virtual void solve(float dt) = 0;
};

}
