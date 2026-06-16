#include "physix2d/Island.h"
#include <unordered_map>

namespace physix2d {

namespace {
int findRoot(std::vector<int>& parent, int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]]; // path halving
        x = parent[x];
    }
    return x;
}
void unite(std::vector<int>& parent, int a, int b) {
    int ra = findRoot(parent, a), rb = findRoot(parent, b);
    if (ra != rb) parent[ra] = rb;
}
}

std::vector<int> IslandBuilder::build(
    const std::deque<Body>& bodies,
    const std::vector<std::pair<size_t, size_t>>& edges) {

    int n = static_cast<int>(bodies.size());
    std::vector<int> parent(n);
    for (int i = 0; i < n; ++i) parent[i] = i;

    // Union only across edges where BOTH endpoints are dynamic; static bodies
    // are cut points and must not transitively connect their neighbours.
    for (const auto& e : edges) {
        int i = static_cast<int>(e.first);
        int j = static_cast<int>(e.second);
        if (i < 0 || j < 0 || i >= n || j >= n) continue;
        if (bodies[i].isStatic() || bodies[j].isStatic()) continue;
        unite(parent, i, j);
    }

    // Compact roots into sequential island ids; static bodies -> -1.
    std::vector<int> islandId(n, -1);
    std::unordered_map<int, int> rootToId;
    int next = 0;
    for (int i = 0; i < n; ++i) {
        if (bodies[i].isStatic()) continue;
        int r = findRoot(parent, i);
        auto it = rootToId.find(r);
        if (it == rootToId.end()) {
            rootToId[r] = next;
            islandId[i] = next;
            ++next;
        } else {
            islandId[i] = it->second;
        }
    }
    return islandId;
}

}
