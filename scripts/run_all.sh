#!/usr/bin/env bash
# Build, test, and run all benchmarks. Usage: ./scripts/run_all.sh
set -euo pipefail
cd "$(dirname "$0")/.."

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j

./build/correctness

mkdir -p results
./build/harness > results/baseline.csv
echo "Wrote results/baseline.csv"
