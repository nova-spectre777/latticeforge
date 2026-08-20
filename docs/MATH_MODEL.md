# v0.1 mathematical model

This release deliberately separates **architecture correctness** from **cryptanalytic authority**.

## Correctness reference bound

For an aggregate Gaussian-like error variable with standard deviation `sigma_eff`, v0.1 uses the two-sided tail bound:

`Pr[|X| >= t] <= 2 exp(-t^2 / (2 sigma_eff^2))`.

The engine derives a conservative effective deviation from `sigma`, `eta`, `k`, and `n`, and uses a decoding margin derived from `q`.

This is a reference bound for the compiler pipeline, not a scheme-specific decryption-failure proof.

## Attack reference models

v0.1 includes transparent monotone surrogate models named `primal-reference`, `dual-reference`, and `hybrid-reference`. They intentionally depend on dimension, modulus/noise ratio, module rank, and hybrid guessing dimension.

They are **not** substitutes for the Lattice Estimator or a cryptanalysis paper. Their job is to provide stable, reproducible, independently testable estimator interfaces so stronger models can be plugged in later.

## Formal claim vocabulary

The engine may output:

- `VALID_UNDER_DECLARED_MODEL`
- `REJECTED`
- `UNKNOWN`

It never outputs `PROVEN_SECURE`.
