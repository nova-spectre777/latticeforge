#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace latticeforge {

struct Params {
  int n{};
  int q{};
  int k{};
  double sigma{};
  int eta{};
};

struct Targets {
  double security_bits{128.0};
  double max_failure_log2{-96.0};
  std::uint64_t max_public_key_bytes{9000};
  std::uint64_t max_ciphertext_bytes{9000};
};

struct AttackEstimate {
  std::string name;
  double cost_bits{};
  std::string assumptions;
};

struct Evaluation {
  Params params;
  bool valid_domain{false};
  double failure_log2{0.0};
  std::uint64_t public_key_bytes{0};
  std::uint64_t ciphertext_bytes{0};
  std::vector<AttackEstimate> attacks;
  double security_bits{0.0};
  std::string cheapest_attack;
  std::string status;
  std::string rejection_reason;
};

bool validate_domain(const Params& p);
double correctness_failure_log2(const Params& p);
std::uint64_t estimate_public_key_bytes(const Params& p);
std::uint64_t estimate_ciphertext_bytes(const Params& p);
std::vector<AttackEstimate> estimate_attacks(const Params& p);
Evaluation evaluate(const Params& p, const Targets& t);
std::string canonical_candidate(const Evaluation& e);
std::uint64_t fnv1a64(const std::string& s);

} // namespace latticeforge
