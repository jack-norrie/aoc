#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>

namespace std {
template <> struct hash<tuple<string, size_t>> {
  size_t operator()(const tuple<string, size_t> t) const {
    size_t string_hash = hash<string>()(get<0>(t));
    size_t int_hash = get<1>(t);
    return int_hash | string_hash << 8;
  }
};
} // namespace std

long long simulate_stone(
    std::string stone, size_t num_iterations,
    std::unordered_map<std::tuple<std::string, size_t>, long long> &memo) {
  std::tuple<std::string, size_t> memo_state =
      std::tuple(stone, num_iterations);
  if (memo.contains(memo_state)) {
    return memo[memo_state];
  }

  if (num_iterations == 0) {
    return 1;
  }

  long long res = 0;
  if (stone == "0") {
    res += simulate_stone("1", num_iterations - 1, memo);
  } else if (stone.size() % 2 == 0) {
    long long mid = stone.size() / 2;
    res += simulate_stone(stone.substr(0, mid), num_iterations - 1, memo);
    res += simulate_stone(std::to_string(std::stoll(stone.substr(mid, mid))),
                          num_iterations - 1, memo);
  } else {
    stone = std::to_string(std::stoll(stone) * 2024);
    res += simulate_stone(stone, num_iterations - 1, memo);
  }

  memo[memo_state] = res;

  return res;
}

int main() {
  long long res = 0;

  std::ifstream input_file("year_2024/data/day_11/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Error: Could not open file.");
  }

  std::string stone;
  std::unordered_map<std::tuple<std::string, size_t>, long long> memo;
  while (input_file >> stone) {
    res += simulate_stone(stone, 75, memo);
  }

  std::cout << res << std::endl;
  return 0;
}
