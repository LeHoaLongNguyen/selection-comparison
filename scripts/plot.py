"""Plots for the report. Usage: python scripts/plot.py results/results.csv
Writes figures and summary.csv to results/figures/."""
import os
import sys

import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv(sys.argv[1])
out = "results/figures"
os.makedirs(out, exist_ok=True)
families = ["random", "sorted", "reversed", "few_distinct", "all_equal"]

# 1. median time vs n, one panel per input family
med = df.groupby(["algorithm", "family", "n"])["ns"].median().reset_index()
fig, axes = plt.subplots(1, len(families), figsize=(18, 4), sharey=True)
for ax, fam in zip(axes, families):
    for alg, g in med[med.family == fam].groupby("algorithm"):
        ax.plot(g.n, g.ns / 1e6, marker="o", label=alg)
    ax.set(title=fam, xscale="log", yscale="log", xlabel="n")
axes[0].set_ylabel("median time (ms)")
axes[0].legend()
fig.tight_layout()
fig.savefig(f"{out}/time_vs_n.png", dpi=150)

# 2. spread of run times at the largest n, random input
big = df[(df.family == "random") & (df.n == df.n.max())]
algs = sorted(big.algorithm.unique())
fig, ax = plt.subplots(figsize=(6, 4))
ax.boxplot([big[big.algorithm == a].ns / 1e6 for a in algs])
ax.set_xticks(range(1, len(algs) + 1), algs)
ax.set(ylabel="time (ms)", title=f"run-to-run spread, random, n={df.n.max():,}")
fig.tight_layout()
fig.savefig(f"{out}/spread.png", dpi=150)

# 3. summary table: median ms and comparisons per element
summary = df.groupby(["family", "n", "algorithm"]).agg(
    median_ms=("ns", lambda s: s.median() / 1e6),
    max_over_median=("ns", lambda s: s.max() / s.median()),
    comparisons_per_n=("comparisons", "median"),
).reset_index()
summary["comparisons_per_n"] /= summary["n"]
summary.round(3).to_csv(f"{out}/summary.csv", index=False)
print(summary[summary.n == df.n.max()].round(3).to_string(index=False))