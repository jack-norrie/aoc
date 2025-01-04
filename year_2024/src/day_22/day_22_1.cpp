#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
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

int main() {
  auto secrets = read_secrets();

  size_t res = 0;
  for (const size_t &start_secret : secrets) {
    size_t secret = start_secret;
    for (size_t i = 0; i < 2000; i++) {
      secret = evolve_secret(secret);
    }
    std::cout << start_secret << ": " << secret << std::endl;
    res += secret;
  }
  std::cout << res << std::endl;

  return 0;
}
