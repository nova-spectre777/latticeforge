#include "latticeforge/search.hpp"
#include <cassert>
#include <iostream>
using namespace latticeforge;

int main() {
  Params bad{0,2,0,0.0,0};
  assert(!validate_domain(bad));

  Params small{256,3329,2,3.2,2};
  Params large{1024,12289,4,3.2,2};
  auto as=estimate_attacks(small), al=estimate_attacks(large);
  assert(!as.empty() && !al.empty());
  auto es=evaluate(small, Targets{220,-20,100000,100000});
  assert(es.status=="REJECTED");
  assert(es.rejection_reason.find("counterexample")!=std::string::npos);

  auto el=evaluate(large, Targets{100,-5,100000,100000});
  assert(el.status=="VALID_UNDER_DECLARED_MODEL");
  assert(el.security_bits > es.security_bits);

  Domain d{{256,512,1024},{3329,12289},{2,4},{3.2},{2}};
  auto r=search(d,Targets{80,-5,100000,100000});
  assert(r.tested==12);
  assert(!r.feasible.empty());
  assert(!r.pareto.empty());
  for (const auto& a : r.pareto)
    for (const auto& b : r.pareto)
      if (&a != &b) assert(!dominates(a,b));

  auto h1=fnv1a64(canonical_candidate(el));
  auto h2=fnv1a64(canonical_candidate(el));
  assert(h1==h2 && h1!=0);
  std::cout << "latticeforge C++ tests passed\n";
}
