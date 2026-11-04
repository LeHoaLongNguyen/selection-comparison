// Benchmark harness: writes one CSV row per timed run to stdout.
// Columns: algorithm,family,n,k,rep,input_seed,ns,comparisons
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#include "generators.hpp"
#include "selection/baseline.hpp"
#include "selection/median_of_medians.hpp"
#include "selection/quickselect.hpp"

using clock_type = std::chrono::steady_clock;

const int warmup = 3, reps = 15;
const std::uint64_t input_seed = 12345;
long long sink = 0;  // stops the optimiser deleting the work

// algo(first, kth, last, comp, seed) is timed with a plain comparison,
// then run again untimed with a counting comparison.
template <class Algo>
void bench(const char* name, Algo algo, const std::string& fam, std::size_t n,
           const std::vector<int>& pristine) {
    const std::size_t k = n / 2;
    std::vector<int> a;
    for (int rep = -warmup; rep < reps; ++rep) {
        std::uint64_t seed = rep + warmup + 1;  // quickselect gets a new seed each run

        a = pristine;  // copy outside the timed region
        auto t0 = clock_type::now();
        algo(a.begin(), a.begin() + k, a.end(), [](int x, int y) { return x < y; }, seed);
        auto t1 = clock_type::now();
        sink += a[k];
        if (rep < 0) continue;

        long long comparisons = 0;
        a = pristine;
        algo(a.begin(), a.begin() + k, a.end(),
             [&comparisons](int x, int y) { ++comparisons; return x < y; }, seed);

        long long ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
        std::printf("%s,%s,%zu,%zu,%d,%llu,%lld,%lld\n", name, fam.c_str(), n, k, rep,
                    (unsigned long long)input_seed, ns, comparisons);
    }
}

int main() {
    const std::vector<std::string> families = {"random", "sorted", "reversed", "few_distinct", "all_equal"};
    const std::vector<std::size_t> sizes = {1000, 10000, 100000, 1000000};

    std::printf("algorithm,family,n,k,rep,input_seed,ns,comparisons\n");
    for (const auto& fam : families)
        for (std::size_t n : sizes) {
            const std::vector<int> pristine = gen::make(fam, n, input_seed);

            bench("baseline", [](auto f, auto k, auto l, auto c, std::uint64_t) {
                selection::baseline_select(f, k, l, c);
            }, fam, n, pristine);

            bench("median_of_medians", [](auto f, auto k, auto l, auto c, std::uint64_t) {
                selection::median_of_medians(f, k, l, c);
            }, fam, n, pristine);

            bench("quickselect", [](auto f, auto k, auto l, auto c, std::uint64_t seed) {
                selection::quickselect(f, k, l, c, seed);
            }, fam, n, pristine);
        }
    std::fprintf(stderr, "checksum %lld\n", sink);
}