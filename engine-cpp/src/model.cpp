#include "latticeforge/model.hpp"
#include <algorithm>
#include <bit>
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>

namespace latticeforge {

bool validate_domain(const Params& p) {
  return p.n >= 128 && p.n <= 16384 && p.q > 2 && p.k >= 1 && p.k <= 16 &&
         std::isfinite(p.sigma) && p.sigma > 0.0 && p.sigma < p.q / 2.0 &&
         p.eta >= 1 && p.eta <= 32;
}

static double log2_safe(double x) {
  if (x <= 0.0 || !std::isfinite(x)) return -std::numeric_limits<double>::infinity();
  return std::log2(x);
}

double correctness_failure_log2(const Params& p) {
  if (!validate_domain(p)) return 0.0;
  // Transparent reference bound. sigma_eff is intentionally conservative.
  const double sigma_eff = p.sigma * std::sqrt(static_cast<double>(p.k) * p.eta * p.n / 8.0);
  const double margin = static_cast<double>(p.q) / 4.0;
  const double exponent = -(margin * margin) / (2.0 * sigma_eff * sigma_eff);
  const double ln_prob = std::log(2.0) + exponent;
  return std::min(0.0, ln_prob / std::log(2.0));
}

std::uint64_t estimate_public_key_bytes(const Params& p) {
  if (!validate_domain(p)) return 0;
  const auto bits_q = static_cast<std::uint64_t>(std::ceil(std::log2(static_cast<double>(p.q))));
  const std::uint64_t coeffs = static_cast<std::uint64_t>(p.n) * static_cast<std::uint64_t>(p.k + 1);
  return (coeffs * bits_q + 7u) / 8u + 32u;
}

std::uint64_t estimate_ciphertext_bytes(const Params& p) {
  if (!validate_domain(p)) return 0;
  const auto bits_q = static_cast<std::uint64_t>(std::ceil(std::log2(static_cast<double>(p.q))));
  const std::uint64_t coeffs = static_cast<std::uint64_t>(p.n) * static_cast<std::uint64_t>(p.k + 1);
  return (coeffs * bits_q + 7u) / 8u + static_cast<std::uint64_t>(p.n / 8);
}

std::vector<AttackEstimate> estimate_attacks(const Params& p) {
  std::vector<AttackEstimate> out;
  if (!validate_domain(p)) return out;

  const double dim = static_cast<double>(p.n) * p.k;
  const double ratio = static_cast<double>(p.q) / p.sigma;
  const double hardness = std::max(1.0, log2_safe(ratio));
  const double eta_penalty = std::log2(static_cast<double>(p.eta) + 1.0);

  // Transparent monotone surrogate models. These numbers are not deployment-grade cryptanalysis.
  const double primal = 0.205 * dim + 5.0 * hardness - 2.0 * eta_penalty;
  const double dual = 0.190 * dim + 6.0 * hardness + 0.8 * p.k - 1.5 * eta_penalty;
  const double hybrid_guess = std::min(64.0, dim / 16.0);
  const double hybrid = 0.175 * (dim - hybrid_guess) + 0.55 * hybrid_guess + 4.5 * hardness - eta_penalty;

  out.push_back({"primal-reference", std::max(0.0, primal), "transparent v0.1 monotone surrogate"});
  out.push_back({"dual-reference", std::max(0.0, dual), "transparent v0.1 monotone surrogate"});
  out.push_back({"hybrid-reference", std::max(0.0, hybrid), "transparent v0.1 monotone surrogate"});
  return out;
}

Evaluation evaluate(const Params& p, const Targets& t) {
  Evaluation e;
  e.params = p;
  e.valid_domain = validate_domain(p);
  if (!e.valid_domain) {
    e.status = "REJECTED";
    e.rejection_reason = "invalid parameter domain";
    return e;
  }
  e.failure_log2 = correctness_failure_log2(p);
  e.public_key_bytes = estimate_public_key_bytes(p);
  e.ciphertext_bytes = estimate_ciphertext_bytes(p);
  e.attacks = estimate_attacks(p);
  if (e.attacks.empty()) {
    e.status = "UNKNOWN";
    e.rejection_reason = "no attack model available";
    return e;
  }
  auto cheapest = std::min_element(e.attacks.begin(), e.attacks.end(), [](const auto& a, const auto& b){ return a.cost_bits < b.cost_bits; });
  e.security_bits = cheapest->cost_bits;
  e.cheapest_attack = cheapest->name;

  if (e.security_bits < t.security_bits) {
    e.status = "REJECTED";
    e.rejection_reason = "counterexample attack below target: " + e.cheapest_attack;
  } else if (e.failure_log2 > t.max_failure_log2) {
    e.status = "REJECTED";
    e.rejection_reason = "correctness failure bound exceeds target";
  } else if (e.public_key_bytes > t.max_public_key_bytes) {
    e.status = "REJECTED";
    e.rejection_reason = "public-key size exceeds target";
  } else if (e.ciphertext_bytes > t.max_ciphertext_bytes) {
    e.status = "REJECTED";
    e.rejection_reason = "ciphertext size exceeds target";
  } else {
    e.status = "VALID_UNDER_DECLARED_MODEL";
  }
  return e;
}

std::string canonical_candidate(const Evaluation& e) {
  std::ostringstream os;
  os << std::fixed << std::setprecision(6)
     << e.params.n << '|' << e.params.q << '|' << e.params.k << '|'
     << e.params.sigma << '|' << e.params.eta << '|'
     << e.security_bits << '|' << e.failure_log2 << '|'
     << e.public_key_bytes << '|' << e.ciphertext_bytes << '|'
     << e.cheapest_attack << '|' << e.status;
  return os.str();
}

std::uint64_t fnv1a64(const std::string& s) {
  std::uint64_t hash = 14695981039346656037ull;
  for (unsigned char c : s) {
    hash ^= c;
    hash *= 1099511628211ull;
  }
  return hash;
}

} // namespace latticeforge
