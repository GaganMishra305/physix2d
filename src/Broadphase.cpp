#include "physix2d/Broadphase.h"
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <cstdint>

namespace physix2d {

namespace {
// Pack two 32-bit cell coords into one 64-bit key.
inline uint64_t cellKey(int32_t cx, int32_t cy) {
    return (static_cast<uint64_t>(static_cast<uint32_t>(cx)) << 32)
         |  static_cast<uint64_t>(static_cast<uint32_t>(cy));
}
inline uint64_t pairKey(size_t i, size_t j) {
    return (static_cast<uint64_t>(i) << 32) | static_cast<uint64_t>(j);
}
}

std::vector<std::pair<size_t, size_t>>
Broadphase::computePairs(std::deque<Body>& bodies, float cellSize) {
    std::vector<std::pair<size_t, size_t>> pairs;
    if (cellSize <= 0.0f || bodies.size() < 2) return pairs;

    std::unordered_map<uint64_t, std::vector<size_t>> grid;
    grid.reserve(bodies.size() * 2);

    const float inv = 1.0f / cellSize;
    for (size_t i = 0; i < bodies.size(); ++i) {
        const Body& b = bodies[i];
        float r = b.getRadius();
        Vec2 p = b.getPosition();
        int minX = static_cast<int>(std::floor((p.x - r) * inv));
        int maxX = static_cast<int>(std::floor((p.x + r) * inv));
        int minY = static_cast<int>(std::floor((p.y - r) * inv));
        int maxY = static_cast<int>(std::floor((p.y + r) * inv));
        for (int cx = minX; cx <= maxX; ++cx)
            for (int cy = minY; cy <= maxY; ++cy)
                grid[cellKey(cx, cy)].push_back(i);
    }

    // Emit unique pairs that share at least one cell.
    std::unordered_set<uint64_t> seen;
    for (auto& kv : grid) {
        auto& cell = kv.second;
        for (size_t a = 0; a < cell.size(); ++a)
            for (size_t b = a + 1; b < cell.size(); ++b) {
                size_t i = cell[a], j = cell[b];
                if (i > j) std::swap(i, j);
                if (seen.insert(pairKey(i, j)).second)
                    pairs.emplace_back(i, j);
            }
    }
    return pairs;
}

}
