#include "physix2d/SweepAndPrune.h"
#include <algorithm>

namespace physix2d {

namespace {
struct Interval {
    float minX, maxX, minY, maxY;
    size_t index;
};
}

std::vector<std::pair<size_t, size_t>> SweepAndPrune::computePairs(std::deque<Body>& bodies) {
    std::vector<std::pair<size_t, size_t>> pairs;
    size_t n = bodies.size();
    if (n < 2) return pairs;

    std::vector<Interval> boxes;
    boxes.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        Body& b = bodies[i];
        AABB a;
        if (b.getShape())
            a = b.getShape()->computeAABB(b.pos, b.angle);
        else {
            float r = b.getRadius();
            a = AABB{ Vec2(b.pos.x - r, b.pos.y - r), Vec2(b.pos.x + r, b.pos.y + r) };
        }
        boxes.push_back({ a.min.x, a.max.x, a.min.y, a.max.y, i });
    }

    // Sort by left edge on the sweep axis.
    std::sort(boxes.begin(), boxes.end(),
              [](const Interval& l, const Interval& r) { return l.minX < r.minX; });

    // Sweep, maintaining a set of intervals whose maxX hasn't been passed yet.
    std::vector<Interval> active;
    active.reserve(n);
    for (const Interval& cur : boxes) {
        // Drop intervals that have closed before cur opens.
        active.erase(
            std::remove_if(active.begin(), active.end(),
                           [&](const Interval& a) { return a.maxX < cur.minX; }),
            active.end());

        for (const Interval& a : active) {
            // x already overlaps (guaranteed by sweep); check y.
            if (a.maxY >= cur.minY && a.minY <= cur.maxY) {
                size_t i = a.index, j = cur.index;
                if (i > j) std::swap(i, j);
                pairs.emplace_back(i, j);
            }
        }
        active.push_back(cur);
    }
    return pairs;
}

}
