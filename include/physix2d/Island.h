#pragma once

#include <deque>
#include <vector>
#include <utility>
#include "physix2d/Body.h"

namespace physix2d {

// Groups dynamic bodies into "islands" of mutually-connected bodies, where a
// connection is a contact or a joint. Islands can then be slept/woken as a
// unit (a whole resting stack sleeps together; one impact wakes the group).
//
// Key rule: STATIC bodies never bridge an island. The ground touching two
// separate stacks must NOT merge them into one island.
class IslandBuilder {
public:
    // Returns an island id per body (index-aligned with `bodies`).
    // Static bodies get id = -1 (they belong to no dynamic island).
    static std::vector<int> build(
        const std::deque<Body>& bodies,
        const std::vector<std::pair<size_t, size_t>>& edges);
};

}
