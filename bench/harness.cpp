// Benchmark harness: writes one CSV row per timed run to stdout.
//
// Columns: algorithm,family,n,k,rep,input_seed,ns
//
// This is a minimal starting point that only times the baseline, to prove the
// pipeline works end to end. TODO as a group:
//   - register quickselect and median_of_medians once they pass the tests
//   - separate counting runs (CountingCompare) and add a comparisons column
//   - batch repetitions for small n where one call is too fast to time
//   - quickselect tail experiment: same input, many algorithm seeds
//   - command-line options for sizes / families / repetitions
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <string>
#include <vector>

#include "generators.hpp"
#include "selection/baseline.hpp"

int main() {
    using clock = std::chrono::steady_clock;
    const std::vector<std::string> families = {"random", "sorted", "few_distinct", "all_equal"};
    const std::vector<std::size_t> sizes = {1000, 10000, 100000, 1000000};
    const int warmup = 3, reps = 15;
    const std::uint64_t input_seed = 12345;

    long long sink = 0;  // stops the optimiser deleting the work
    std::printf("algorithm,family,n,k,rep,input_seed,ns\n");

    for (const auto& fam : families)
        for (std::size_t n : sizes) {
            const std::vector<int> pristine = gen::make(fam, n, input_seed);
            const std::size_t k = n / 2;  // median
            std::vector<int> a;
            for (int rep = -warmup; rep < reps; ++rep) {
                a = pristine;  // copy OUTSIDE the timed region
                auto t0 = clock::now();
                selection::baseline_select(a.begin(), a.begin() + k, a.end(), std::less<int>());
                auto t1 = clock::now();
                sink += a[k];
                if (rep < 0) continue;  // warm-up runs are not recorded
                long long ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
                std::printf("baseline,%s,%zu,%zu,%d,%llu,%lld\n",
                            fam.c_str(), n, k, rep, (unsigned long long)input_seed, ns);
            }
        }
    std::fprintf(stderr, "checksum %lld\n", sink);
}
