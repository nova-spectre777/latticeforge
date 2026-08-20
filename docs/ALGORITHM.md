# SPECTRA-Ω algorithm

SPECTRA-Ω treats parameter selection as a constrained multi-objective search.

For each candidate tuple `P = (n,q,k,sigma,eta)`:

1. validate algebraic domain constraints;
2. compute correctness/failure bounds;
3. query every enabled attack-cost estimator;
4. reject immediately when an enabled hard constraint fails;
5. retain a structured counterexample naming the failed constraint or cheapest attack;
6. retain feasible candidates;
7. remove dominated candidates using a Pareto comparison;
8. emit a canonical certificate carrying assumptions and model versions.

## Counterexample-first semantics

If 20 models satisfy a target but one enabled model estimates a lower cost than the target, the candidate is rejected. Estimates are never averaged into a single probability of safety.

## Dominance

Candidate A dominates B when A is no worse in all selected objectives and strictly better in at least one. v0.1 objectives are:

- maximize minimum estimated security bits;
- minimize public-key bytes;
- minimize ciphertext bytes;
- minimize failure probability.
