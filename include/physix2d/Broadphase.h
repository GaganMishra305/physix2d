#pragma once

#include <deque>
#include <vector>
#include <utility>
#include "physix2d/Body.h"

namespace physix2d {

// Uniform spatial-hash broad phase. Turns the naive O(n^2) all-pairs
// collision scan into a near O(n) candidate generation by only pairing
// bodies that share a grid cell. Detection/resolution still happens in
// the narrow phase (Collision); this just prunes obviously-distant pairs.
class Broadphase {
public:
    // Candidate index pairs (i < j) into the given bodies container.
    static std::vector<std::pair<size_t, size_t>>
    computePairs(std::deque<Body>& bodies, float cellSize);
};

}
