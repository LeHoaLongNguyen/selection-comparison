// Deterministic input generation.
//
// We use our own splitmix64 instead of std::uniform_int_distribution because
// the standard distributions are NOT guaranteed to produce the same numbers on
// GCC, Clang and MSVC, even with the same seed. With splitmix64, the same seed
// gives the same array on every machine.
#pragma once
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace gen {

struct SplitMix64 {
    std::uint64_t state;
    explicit SplitMix64(std::uint64_t seed) : state(seed) {}
    std::uint64_t next() {
        std::uint64_t z = (state += 0x9E3779B97F4A7C15ULL);
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        return z ^ (z >> 31);
    }
    // Value in [0, bound). Tiny modulo bias is irrelevant for benchmarking.
    std::uint64_t below(std::uint64_t bound) { return next() % bound; }
};

// Input families. Add more here (e.g. organ-pipe, adversarial for a specific pivot rule).
inline std::vector<int> make(const std::string& family, std::size_t n, std::uint64_t seed) {
    SplitMix64 rng(seed);
    std::vector<int> a(n);
    if (family == "random") {
        for (auto& x : a) x = static_cast<int>(rng.next() & 0x7FFFFFFF);
    } else if (family == "sorted") {
        for (std::size_t i = 0; i < n; ++i) a[i] = static_cast<int>(i);
    } else if (family == "reversed") {
        for (std::size_t i = 0; i < n; ++i) a[i] = static_cast<int>(n - i);
    } else if (family == "few_distinct") {      // only 10 distinct values
        for (auto& x : a) x = static_cast<int>(rng.below(10));
    } else if (family == "all_equal") {
        std::fill(a.begin(), a.end(), 42);
    } else {
        throw std::invalid_argument("unknown family: " + family);
    }
    return a;
}

} // namespace gen
