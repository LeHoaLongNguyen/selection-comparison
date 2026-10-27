# Selection: Deterministic vs Randomised

Programming Assignment 2, Theme 2 (Deterministic vs Randomised).

**Problem:** given an unsorted array of n elements and an index k (0-indexed),
find the k-th smallest element.

| Algorithm | Type | Primary author | File |
|---|---|---|---|
| Randomised quickselect | Randomised (Las Vegas) | Andy Nguyen | `include/selection/quickselect.hpp` |
| Median-of-medians (BFPRT) | Deterministic | Joshua | `include/selection/median_of_medians.hpp` |
| `std::nth_element` | External baseline | — | `include/selection/baseline.hpp` |

All three share the same signature as `std::nth_element`
(`first, kth, last, comp`); see `include/selection/common.hpp` for the contract.

## Requirements

- CMake ≥ 3.16
- A C++17 compiler (developed with GCC 13)
- Python 3 for plotting

## Build and run

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/correctness          # correctness tests for every algorithm
./build/harness > results/baseline.csv
```

Or everything at once: `./scripts/run_all.sh`

## Layout

```
include/selection/   the algorithms (header-only templates) + shared contract
bench/               input generators and benchmark harness
tests/               correctness tests
scripts/             run and plot scripts
results/             benchmark CSVs and figures
docs/                predictions, AI-use log, report drafts
```

## Reproducibility

- Inputs come from our own splitmix64 generator (`bench/generators.hpp`), so the same
  seed produces the same array on every compiler and OS.
- The input seed and quickselect's pivot seed are separate.
- Timing uses `std::chrono::steady_clock`; arrays are copied outside the timed region.
