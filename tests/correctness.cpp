// Correctness test for every selection algorithm.
// Algorithms that still throw "not implemented" are reported as SKIPPED.
#include <algorithm>
#include <cstdio>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

#include "generators.hpp"
#include "selection/baseline.hpp"
#include "selection/median_of_medians.hpp"
#include "selection/quickselect.hpp"

using Vec = std::vector<int>;
using Algo = std::function<void(Vec&, std::size_t)>;

// Checks the postcondition from common.hpp.
static bool check(const Vec& original, const Vec& result, std::size_t k) {
    Vec sorted = original;
    std::sort(sorted.begin(), sorted.end());
    if (result[k] != sorted[k]) return false;
    for (std::size_t i = 0; i < k; ++i) if (result[i] > result[k]) return false;
    for (std::size_t i = k + 1; i < result.size(); ++i) if (result[i] < result[k]) return false;
    Vec r = result;  // also check nothing was lost or duplicated
    std::sort(r.begin(), r.end());
    return r == sorted;
}

// Returns: 1 pass, 0 fail, -1 not implemented
static int run_suite(const std::string& name, const Algo& algo) {
    const char* families[] = {"random", "sorted", "reversed", "few_distinct", "all_equal"};
    std::size_t sizes[] = {1, 2, 3, 4, 5, 7, 10, 31, 100, 1000};
    int cases = 0;
    try {
        for (const char* fam : families)
            for (std::size_t n : sizes)
                for (std::uint64_t seed = 1; seed <= 3; ++seed) {
                    Vec input = gen::make(fam, n, seed);
                    // every k for small n, a spread of k for large n
                    std::size_t step = n <= 100 ? 1 : n / 20;
                    for (std::size_t k = 0; k < n; k += step) {
                        Vec a = input;
                        algo(a, k);
                        ++cases;
                        if (!check(input, a, k)) {
                            std::printf("FAIL  %-18s family=%s n=%zu seed=%llu k=%zu\n",
                                        name.c_str(), fam, n, (unsigned long long)seed, k);
                            return 0;
                        }
                    }
                }
    } catch (const std::logic_error& e) {
        std::printf("SKIP  %-18s (%s)\n", name.c_str(), e.what());
        return -1;
    }
    std::printf("PASS  %-18s %d cases\n", name.c_str(), cases);
    return 1;
}

int main() {
    std::less<int> lt;
    bool ok = true;
    ok &= run_suite("baseline", [&](Vec& a, std::size_t k) {
        selection::baseline_select(a.begin(), a.begin() + k, a.end(), lt); }) != 0;
    ok &= run_suite("quickselect", [&](Vec& a, std::size_t k) {
        selection::quickselect(a.begin(), a.begin() + k, a.end(), lt); }) != 0;
    ok &= run_suite("median_of_medians", [&](Vec& a, std::size_t k) {
        selection::median_of_medians(a.begin(), a.begin() + k, a.end(), lt); }) != 0;
    return ok ? 0 : 1;
}
