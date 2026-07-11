#include "physix2d/Profiler.h"
#include <vector>
#include <algorithm>
#include <cstdio>

namespace physix2d {

void Profiler::add(const std::string& name, double ms) {
    Entry& e = entries[name];
    e.totalMs += ms;
    e.calls += 1;
}

void Profiler::reset() {
    entries.clear();
}

Profiler& Profiler::instance() {
    static Profiler p;
    return p;
}

std::string Profiler::report() const {
    std::vector<std::pair<std::string, Entry>> rows(entries.begin(), entries.end());
    std::sort(rows.begin(), rows.end(),
              [](const auto& a, const auto& b) { return a.second.totalMs > b.second.totalMs; });

    std::string out = "-- Profiler report --\n";
    char line[256];
    for (const auto& r : rows) {
        double avg = r.second.calls ? r.second.totalMs / r.second.calls : 0.0;
        std::snprintf(line, sizeof(line), "%-24s total=%9.3f ms  calls=%-8llu avg=%.4f ms\n",
                      r.first.c_str(), r.second.totalMs,
                      static_cast<unsigned long long>(r.second.calls), avg);
        out += line;
    }
    return out;
}

}
