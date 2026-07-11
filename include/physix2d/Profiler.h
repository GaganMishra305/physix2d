#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>
#include <chrono>

namespace physix2d {

// Lightweight sampling profiler. Accumulates wall-clock time and call counts
// per named section; use the ScopedTimer / PHYSIX_PROFILE macro to time a scope.
class Profiler {
public:
    struct Entry {
        double totalMs = 0.0;
        uint64_t calls = 0;
    };

    void add(const std::string& name, double ms);
    const std::unordered_map<std::string, Entry>& sections() const { return entries; }
    std::string report() const;   // human-readable, sorted by total time
    void reset();

    static Profiler& instance();

private:
    std::unordered_map<std::string, Entry> entries;
};

// RAII timer: records elapsed time into a Profiler on scope exit.
class ScopedTimer {
public:
    ScopedTimer(const char* name, Profiler* prof = &Profiler::instance())
        : name_(name), prof_(prof), start_(std::chrono::high_resolution_clock::now()) {}
    ~ScopedTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start_).count();
        prof_->add(name_, ms);
    }
private:
    const char* name_;
    Profiler* prof_;
    std::chrono::high_resolution_clock::time_point start_;
};

#define PHYSIX_PROFILE(name) ::physix2d::ScopedTimer _physix_timer_##__LINE__(name)

}
