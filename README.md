<div align="center">

# LATTICEFORGE
### SPECTRA-Ω · Cryptographic Parameter Proof Compiler

**A math-first research platform for searching, stress-testing, and certifying declared lattice-cryptography parameter constraints.**

</div>

LATTICEFORGE explores a simple idea: a parameter set should not be accepted because it *looks standard*. It should survive explicit correctness constraints, multiple declared attack-cost models, size limits, and reproducible counterexample search.

> **Status:** v0.1 research foundation. The included reference estimators are educational/conservative models, not a replacement for peer-reviewed cryptanalysis or the Lattice Estimator. Output is `VALID UNDER DECLARED MODEL`, never an unconditional claim of security.

## SPECTRA-Ω

```text
parameter domain
 n, q, k, sigma, eta
       |
       v
mathematical constraints
       |
 +-----+-------------------+
 |                         |
 v                         v
correctness             attack models
Gaussian/tail           primal/dual/hybrid
bounds                    cost estimates
 |                         |
 +-----------+-------------+
             v
      constraint pruning
             v
      feasible candidates
             v
       Pareto frontier
             v
   reproducible certificate
```

SPECTRA-Ω is **counterexample-first**: if any enabled model violates a target, the candidate is rejected and the cheapest violating model is reported.

## v0.1 verified stack

| Layer | Language | Purpose |
|---|---|---|
| `engine-cpp/` | C++23 | parameter model, bounds, attack-model interface, pruning, Pareto frontier |
| `lab-python/` | Python | parameter-grid experiments, certificate validation, benchmark corpus |
| `studio-ts/` | TypeScript | browser-side frontier/constraint visualization primitives |

## What it computes

For a declared candidate, v0.1 evaluates:

- modular/algebraic validity (`q > 2`, positive dimensions/noise);
- a Gaussian-style correctness tail bound;
- reference primal, dual, and hybrid attack-cost estimates;
- key/ciphertext size estimates from explicit formulas;
- target security / failure / size constraints;
- the cheapest surviving attack as a counterexample;
- a Pareto frontier across security, failure bound, and size;
- a SHA-256-independent canonical certificate payload hash (FNV-1a in the portable C++ core; stronger signing can be layered externally).

## Example

```bash
./scripts/check_all.sh
cmake -S engine-cpp -B engine-cpp/build
cmake --build engine-cpp/build
./engine-cpp/build/latticeforge-search examples/search.json
```

Example result shape:

```json
{
  "status": "VALID_UNDER_DECLARED_MODEL",
  "candidate": {"n": 1024, "q": 12289, "k": 3, "sigma": 3.2, "eta": 2},
  "security_bits": 188.4,
  "failure_log2": -145.1,
  "public_key_bytes": 4640,
  "ciphertext_bytes": 4256,
  "cheapest_attack": "hybrid-reference"
}
```

Numbers above are illustrative; run the engine for actual v0.1 outputs.

## Safety and scientific honesty

LATTICEFORGE is defensive/research tooling. It does not generate wallet keys, steal secrets, crack deployed cryptosystems, or automate attacks against targets. Attack models operate on abstract parameter tuples only.

The reference cost models are intentionally transparent and replaceable. They are suitable for testing the **compiler architecture and constraint logic**, not for making deployment-grade security claims.

See `docs/ALGORITHM.md`, `docs/MATH_MODEL.md`, and `docs/ROADMAP.md`.
