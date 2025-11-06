# Example 03 – Vectorization and SIMD Performance (Intel i3-2120)

## Objective
The goal of this lab is to understand how **vectorization** exploits SIMD (Single Instruction, Multiple Data) hardware to execute multiple floating-point operations simultaneously.  
We test three optimization levels (`timestep_opt1.c`, `timestep_opt2.c`, `timestep_opt3.c`) and compare them against the baseline implementation.

---

## ⚙️ Environment
**CPU:** Intel® Core™ i3-2120 @ 3.30 GHz (Sandy Bridge, 2 cores)  
**Vector ISA:** SSE / AVX (128-bit = 16 bytes)  
**OS:** Linux (Ubuntu 22.04 Lab Environment)  
**Compiler:** GCC 11 with flags:  

-O3 -march=native -ftree-vectorize -fopt-info-vec-optimized
-fopenmp -mprefer-vector-width=128 -lm

**Performance tool:** LIKWID (`sudo likwid-perfctr -C 0 -f -g FLOPS_DP`)

---

## Compiler Feedback and LIKWID Results

| Version | Compiler Feedback | DP [MFLOP/s] | Vectorization [%] | Notes |
|----------|------------------|---------------|--------------------|-------|
| **Baseline** (`timestep.c`) | *loop not vectorized (scalar code)* | 186.86 | 0 | Simple triad loop, scalar execution |
| **opt1** (`#pragma omp simd`) | *loop vectorized using 16 byte vectors* | 188.94 | 0 | Compiler applied SIMD, minor gain |
| **opt2** (`restrict` + pragma) | *loop vectorized using 16 byte vectors* | 188.69 | 0 | Alias checks removed; similar performance |
| **opt3** (`restrict` + fast-math) | *loop vectorized using 16 byte vectors* | **229.59** | **≈ 100 %** | Full vectorization achieved, fastest run |

---

## Excerpt from LIKWID (opt3)

| FP_COMP_OPS_EXE_SSE_FP_PACKED_DOUBLE | 45172126 |
| FP_COMP_OPS_EXE_SSE_FP_SCALAR_DOUBLE | 84 |
| SIMD_FP_256_PACKED_DOUBLE | 0 |
| DP [MFLOP/s] | 229.5909 |
| Vectorization ratio [%] | 99.9998 |

➡️ **Interpretation:**  
- Packed SSE double-precision operations executed (128-bit SIMD).  
- Scalar operations almost eliminated.  
- CPU reached ~230 MFLOP/s with full 128-bit vectorization.

---

## Discussion
- The baseline version ran purely scalar, limited by memory bandwidth.  
- Adding `#pragma omp simd` (opt1) allowed GCC to emit SIMD loops, but measurable gain was small due to low arithmetic intensity.  
- Using the `restrict` keyword (opt2) removed aliasing concerns, ensuring the compiler could safely apply vectorization.  
- The final build with `-fno-trapping-math -fno-math-errno` (opt3) allowed more aggressive math optimizations, fully utilizing the SIMD units.  
- **Sandy Bridge supports 128-bit vectors only**, so `256b` counters remain zero.  
- The performance increase from 186 → 229 MFLOP/s confirms successful vectorization.

---

## Conclusion
All optimization steps show progressive improvement.  
The final version (`timestep_opt3.c`) achieved:
- **100 % vectorization ratio**
- **~23 % performance gain**
- **Verified SIMD usage (SSE 128-bit)**

This demonstrates how compiler directives, aliasing hints, and math flags collectively enable efficient SIMD execution and maximize FLOPs on available hardware.