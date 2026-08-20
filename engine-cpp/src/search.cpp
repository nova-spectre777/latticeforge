#include "latticeforge/search.hpp"
#include <algorithm>

namespace latticeforge {

bool dominates(const Evaluation& a, const Evaluation& b) {
  const bool no_worse = a.security_bits >= b.security_bits &&
                        a.failure_log2 <= b.failure_log2 &&
                        a.public_key_bytes <= b.public_key_bytes &&
                        a.ciphertext_bytes <= b.ciphertext_bytes;
  const bool strictly = a.security_bits > b.security_bits ||
                        a.failure_log2 < b.failure_log2 ||
                        a.public_key_bytes < b.public_key_bytes ||
                        a.ciphertext_bytes < b.ciphertext_bytes;
  return no_worse && strictly;
}

std::vector<Evaluation> pareto_frontier(const std::vector<Evaluation>& xs) {
  std::vector<Evaluation> out;
  for (std::size_t i = 0; i < xs.size(); ++i) {
    bool dominated = false;
    for (std::size_t j = 0; j < xs.size(); ++j) {
      if (i != j && dominates(xs[j], xs[i])) { dominated = true; break; }
    }
    if (!dominated) out.push_back(xs[i]);
  }
  std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) {
    if (a.security_bits != b.security_bits) return a.security_bits > b.security_bits;
    if (a.public_key_bytes != b.public_key_bytes) return a.public_key_bytes < b.public_key_bytes;
    return a.ciphertext_bytes < b.ciphertext_bytes;
  });
  return out;
}

SearchResult search(const Domain& d, const Targets& t) {
  SearchResult r;
  for (int n : d.n_values)
  for (int q : d.q_values)
  for (int k : d.k_values)
  for (double sigma : d.sigma_values)
  for (int eta : d.eta_values) {
    ++r.tested;
    auto e = evaluate({n,q,k,sigma,eta}, t);
    if (e.status == "VALID_UNDER_DECLARED_MODEL") r.feasible.push_back(e);
    else ++r.rejected;
  }
  r.pareto = pareto_frontier(r.feasible);
  return r;
}

} // namespace latticeforge
