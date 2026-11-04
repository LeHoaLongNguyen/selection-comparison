# Selection: Deterministic vs Randomised

Programming Assignment 2, Theme 2 (Deterministic vs Randomised).

**Problem:** given an unsorted array of n elements and an index k (0-indexed),
find the k-th smallest element.

| Algorithm | Type | Primary author | File |
|---|---|---|---|
| Randomised quickselect | Randomised (Las Vegas) | Le Hoa Long Nguyen | `include/selection/quickselect.hpp` |
| Median-of-medians (BFPRT) | Deterministic | Joshua Davis | `include/selection/median_of_medians.hpp` |
| `std::nth_element` | External baseline | — | `include/selection/baseline.hpp` |

All three share the same signature as `std::nth_element`
(`first, kth, last, comp`); see `include/selection/common.hpp` for the contract.

## Requirements

- A C++17 compiler. The reported results were built with MSVC 19.51 (Visual Studio 2026) on Windows 11.
- Python 3 with pandas and matplotlib for plotting: `pip install pandas matplotlib`

## Build and run (Windows, MSVC — used for the reported results)

Open a **Developer PowerShell for Visual Studio** (a normal PowerShell won't find `cl`),
or load the tools into an existing terminal:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\Launch-VsDevShell.ps1" -Arch amd64 -SkipAutomaticLocation
```

Then, from the repository root:

```powershell
mkdir build -Force

# 1. correctness tests: all three algorithms should print PASS
cl /std:c++17 /EHsc /O2 /Iinclude /Ibench tests\correctness.cpp /Fobuild\ /Febuild\correctness.exe
.\build\correctness.exe

# 2. benchmarks: writes one CSV row per timed run
cl /std:c++17 /EHsc /O2 /Iinclude /Ibench bench\harness.cpp /Fobuild\ /Febuild\harness.exe
.\build\harness.exe > results\results.csv

# 3. plots and summary table, written to results/figures/
python scripts\plot.py results\results.csv
```

## Build and run (Linux / macOS, GCC or Clang)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/correctness
./build/harness > results/results.csv
python3 scripts/plot.py results/results.csv
```

Or everything at once: `./scripts/run_all.sh`

Note: `CMakeLists.txt` uses GCC/Clang warning flags (`-Wextra -Wpedantic`), which MSVC
rejects, so the CMake build does not work with MSVC. Use the commands above on Windows.

## Layout

```
include/selection/   the algorithms (header-only templates) + shared contract
bench/               input generators and benchmark harness
tests/               correctness tests
scripts/             run and plot scripts
results/             benchmark CSVs; figures and summary table in results/figures/
docs/                predictions, AI-use log, report drafts
```

## Reproducibility

- Inputs come from our own splitmix64 generator (`bench/generators.hpp`), so the same
  seed produces the same array on every compiler and OS. The input seed is 12345.
- The input seed and quickselect's pivot seed are separate; quickselect gets a new seed
  on every timed run so its run-to-run variation is visible.
- Each configuration runs 3 warm-up runs (discarded) and 15 timed runs; the report uses the median.
- Timing uses `std::chrono::steady_clock`; arrays are copied outside the timed region.
- Comparison counts are measured on a separate, untimed run.
- Reported results: AMD Ryzen 7 7800X3D, 31 GB RAM, Windows 11 Home, MSVC `/O2`.