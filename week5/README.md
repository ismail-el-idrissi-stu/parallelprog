# Week-5 Lab: Spatial Hashing & Global Sum Precision

## Contents
- `neighbors2d_hashcpu.c` → builds `bin/neighbors2d` (O(N) neighbor finder using spatial hashing)
- `sum_bench.c` → builds `bin/sum_bench` (compares summation techniques)

## Build
```bash
cd week5/src
make

Run

../bin/neighbors2d
../bin/sum_bench 1000000 alternating 5

Results

    CSV files: week5/results/*.csv

    Screenshots: week5/screenshots/*.png

    Google Sheet: https://docs.google.com/spreadsheets/d/1vkmcB0mYV5CswAoBglL4AFORrpngiBLOU-OVulDlxgo/edit?gid=0#gid=0

Explanation

Floating-point addition isn’t associative ⇒ parallel reductions give different rounding results.
Kahan / Knuth compensate lost low-order bits → more accurate.
Pairwise (tree) → better for parallelism.
Long double → reference accuracy.

Parallel reproducibility requires deterministic reduction order + compensation or higher precision.