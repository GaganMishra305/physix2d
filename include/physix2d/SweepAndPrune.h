#pragma once

#include <deque>
#include <vector>
#include <utility>
#include "physix2d/Body.h"

namespace physix2d {

// Sweep-and-prune broad phase. Projects each body's world AABB onto the x-axis,
// sorts by the interval's left edge, then sweeps once keeping an "active" set of
// intervals still open. Two bodies are candidates only if their x-intervals
// overlap AND their y-intervals overlap. Scales well when objects are spread
// along an axis (unlike a fixed-cell grid).
class SweepAndPrune {
public:
    static std::vector<std::pair<size_t, size_t>> computePairs(std::deque<Body>& bodies);
};

}
