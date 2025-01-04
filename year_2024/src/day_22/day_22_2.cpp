#include <deque>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::vector<size_t> read_secrets() {
  std::ifstream input_file("data/day_22/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not opne file.");
  }

  std::vector<size_t> secrets = {};
  std::string line = {};
  while (std::getline(input_file, line)) {
    secrets.push_back(std::stoi(line));
  }

  return secrets;
}

size_t mix(const size_t &secret, const size_t &mixer) { return secret ^ mixer; }

size_t prune(const size_t &secret) { return secret % 16777216; }

size_t evolve_secret(size_t &secret) {
  secret = prune(mix(secret, secret << 6));
  secret = prune(mix(secret, secret >> 5));
  secret = prune(mix(secret, secret << 11));
  return secret;
}

using MemoKey = size_t;
inline size_t get_key(const std::deque<int> &deltas) {
  size_t res = 0;
  for (size_t i = 0; i < deltas.size(); i++) {
    res ^= (deltas[i] & ((1 << 8) - 1)) << (8 * i);
  }
  return res;
}

int main() {
  auto secrets = read_secrets();

  std::unordered_map<size_t, size_t> sequence_total_profits = {};
  for (size_t &secret : secrets) {
    std::deque<int> deltas = {};
    std::unordered_set<size_t> triggered = {};
    for (size_t i = 0; i < 2000; i++) {
      size_t prev_secret = secret;
      secret = evolve_secret(secret);

      int prev_price = prev_secret % 10;
      int price = secret % 10;
      int delta = price - prev_price;

      deltas.push_back(delta);
      if (deltas.size() > 4) {
        deltas.pop_front();
      }
      if (deltas.size() == 4) {
        size_t deltas_key = get_key(deltas);
        if (!triggered.contains(deltas_key)) {
          sequence_total_profits[deltas_key] += price;
        }
        triggered.insert(deltas_key);
      }
    }
  }

  size_t max_profit = 0;
  for (const auto &it : sequence_total_profits) {
    if (it.second > max_profit) {
      max_profit = it.second;
    }
  }

  std::cout << max_profit << std::endl;

  return 0;
}
