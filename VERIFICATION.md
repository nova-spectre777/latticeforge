# LATTICEFORGE v0.1 verification

Verified locally on 2026-08-20.

## Passed

- C++23 configure/build
- C++ engine unit test executable
- counterexample-first rejection invariant
- parameter-domain validation
- Pareto dominance/frontier invariant
- deterministic portable certificate hash
- Python: 4/4 tests
- Python 216-candidate benchmark
- TypeScript strict production compile
- TypeScript: 2/2 tests
- example search JSON parsed and executed by the production C++ CLI

## Bundled search fixture

- tested candidates: 216
- feasible under declared v0.1 targets/models: 173
- Pareto frontier: 21

The security-bit values in this release come from **transparent reference surrogate models** implemented to test the SPECTRA-Ω compiler/search architecture. They are not deployment-grade lattice-security estimates and must not be represented as such.

## Scientific boundary

The strongest supported statement is:

> candidate X is `VALID_UNDER_DECLARED_MODEL` for the exact estimator versions, assumptions, correctness bound, and constraints recorded in its certificate.

The software does not output `PROVEN_SECURE`.

## Package hygiene

- source/test lines (C++/Python/TypeScript): 450
- generated C++/TypeScript build directories removed before packaging
- no private keys, seed phrases, credentials, or target-specific attack code
