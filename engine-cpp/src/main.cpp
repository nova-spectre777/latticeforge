#include "latticeforge/search.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

using namespace latticeforge;

static std::string read_all(const std::string& path) {
  std::ifstream f(path);
  std::ostringstream ss;
  ss << f.rdbuf();
  return ss.str();
}

template <typename T>
static std::vector<T> parse_array(const std::string& text, const std::string& key) {
  std::regex re("\\\"" + key + "\\\"\\s*:\\s*\\[([^\\]]*)\\]");
  std::smatch m;
  std::vector<T> out;
  if (!std::regex_search(text, m, re)) return out;
  std::stringstream ss(m[1].str());
  std::string item;
  while (std::getline(ss, item, ',')) {
    std::stringstream v(item); T x{}; if (v >> x) out.push_back(x);
  }
  return out;
}

static double parse_double(const std::string& text, const std::string& key, double def) {
  std::regex re("\\\"" + key + "\\\"\\s*:\\s*(-?[0-9]+(?:\\.[0-9]+)?)");
  std::smatch m; if (!std::regex_search(text,m,re)) return def; return std::stod(m[1].str());
}

static std::uint64_t parse_u64(const std::string& text, const std::string& key, std::uint64_t def) {
  std::regex re("\\\"" + key + "\\\"\\s*:\\s*([0-9]+)");
  std::smatch m; if (!std::regex_search(text,m,re)) return def; return std::stoull(m[1].str());
}

int main(int argc, char** argv) {
  Domain d{{256,512,768,1024},{3329,7681,12289},{2,3,4},{2.8,3.2,3.6},{2,3}};
  Targets t;
  if (argc > 1) {
    auto text = read_all(argv[1]);
    auto ns=parse_array<int>(text,"n_values"); if(!ns.empty()) d.n_values=ns;
    auto qs=parse_array<int>(text,"q_values"); if(!qs.empty()) d.q_values=qs;
    auto ks=parse_array<int>(text,"k_values"); if(!ks.empty()) d.k_values=ks;
    auto ss=parse_array<double>(text,"sigma_values"); if(!ss.empty()) d.sigma_values=ss;
    auto es=parse_array<int>(text,"eta_values"); if(!es.empty()) d.eta_values=es;
    t.security_bits=parse_double(text,"target_security_bits",t.security_bits);
    t.max_failure_log2=parse_double(text,"max_failure_log2",t.max_failure_log2);
    t.max_public_key_bytes=parse_u64(text,"max_public_key_bytes",t.max_public_key_bytes);
    t.max_ciphertext_bytes=parse_u64(text,"max_ciphertext_bytes",t.max_ciphertext_bytes);
  }

  auto r = search(d,t);
  std::cout << "{\n  \"algorithm\": \"SPECTRA-Omega\",\n  \"tested\": " << r.tested
            << ",\n  \"feasible\": " << r.feasible.size() << ",\n  \"pareto_count\": " << r.pareto.size() << ",\n  \"pareto\": [\n";
  for (std::size_t i=0;i<r.pareto.size();++i) {
    const auto& e=r.pareto[i];
    std::cout << "    {\"n\":"<<e.params.n<<",\"q\":"<<e.params.q<<",\"k\":"<<e.params.k
              <<",\"sigma\":"<<std::fixed<<std::setprecision(2)<<e.params.sigma<<",\"eta\":"<<e.params.eta
              <<",\"security_bits\":"<<std::setprecision(2)<<e.security_bits
              <<",\"failure_log2\":"<<e.failure_log2
              <<",\"public_key_bytes\":"<<e.public_key_bytes
              <<",\"ciphertext_bytes\":"<<e.ciphertext_bytes
              <<",\"cheapest_attack\":\""<<e.cheapest_attack<<"\""
              <<",\"certificate_hash\":\""<<std::hex<<fnv1a64(canonical_candidate(e))<<std::dec<<"\"}";
    if(i+1<r.pareto.size()) std::cout<<",";
    std::cout<<"\n";
  }
  std::cout << "  ]\n}\n";
  return 0;
}
