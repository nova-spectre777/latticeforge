#pragma once
#include "latticeforge/model.hpp"
#include <vector>

namespace latticeforge {
struct Domain {
  std::vector<int> n_values;
  std::vector<int> q_values;
  std::vector<int> k_values;
  std::vector<double> sigma_values;
  std::vector<int> eta_values;
};

struct SearchResult {
  std::size_t tested{};
  std::size_t rejected{};
  std::vector<Evaluation> feasible;
  std::vector<Evaluation> pareto;
};

SearchResult search(const Domain& d, const Targets& t);
bool dominates(const Evaluation& a, const Evaluation& b);
std::vector<Evaluation> pareto_frontier(const std::vector<Evaluation>& xs);
} // namespace latticeforge
