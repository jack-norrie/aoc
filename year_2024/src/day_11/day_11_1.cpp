#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>

long long simulate_stone(std::string stone, size_t num_iterations) {
  if (num_iterations == 0) {
    return 1;
  }

  long long res = 0;
  if (stone == "0") {
    res += simulate_stone("1", num_iterations - 1);
  } else if (stone.size() % 2 == 0) {
    long long mid = stone.size() / 2;
    res += simulate_stone(stone.substr(0, mid), num_iterations - 1);
    res += simulate_stone(std::to_string(std::stoll(stone.substr(mid, mid))),
                          num_iterations - 1);
  } else {
    stone = std::to_string(std::stoll(stone) * 2024);
    res += simulate_stone(stone, num_iterations - 1);
  }

  return res;
}

int main() {
  long long res = 0;

  std::ifstream input_file("year_2024/data/day_11/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Error: Could not open file.");
  }

  std::string stone;
  while (input_file >> stone) {
    res += simulate_stone(stone, 25);
  }

  std::cout << res << std::endl;
  return 0;
}
