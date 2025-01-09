#include <algorithm>
#include <bitset>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

bool check_on(
    const std::string &wire,
    const std::unordered_map<std::string,
                             std::tuple<std::string, std::string, std::string>>
        &dependency_graph,
    std::unordered_map<std::string, bool> &memo) {

  if (memo.contains(wire)) {
    return memo.at(wire);
  }

  std::tuple<std::string, std::string, std::string> depedency_relation =
      dependency_graph.at(wire);
  std::string operand_1 = std::get<0>(depedency_relation);
  std::string operator_ = std::get<1>(depedency_relation);
  std::string operand_2 = std::get<2>(depedency_relation);

  bool res = {};
  if (operator_ == "AND") {
    res = check_on(operand_1, dependency_graph, memo) &&
          check_on(operand_2, dependency_graph, memo);
  } else if (operator_ == "OR") {
    res = check_on(operand_1, dependency_graph, memo) ||
          check_on(operand_2, dependency_graph, memo);
  } else if (operator_ == "XOR") {
    res = check_on(operand_1, dependency_graph, memo) ^
          check_on(operand_2, dependency_graph, memo);
  } else {
    throw std::runtime_error("Invalid operator.");
  }

  memo[wire] = res;
  return res;
}

std::tuple<std::vector<bool>, std::vector<bool>>
get_input(std::ifstream &input_file) {
  std::vector<bool> x = {};
  std::vector<bool> y = {};

  std::regex initial_state_pattern("([xy]\\d{2}): (\\d)");
  std::smatch initial_state_match = {};

  std::string line = {};
  std::getline(input_file, line);
  while (line != "") {
    std::regex_search(line, initial_state_match, initial_state_pattern);
    if (line[0] == 'x') {
      x.push_back(initial_state_match[2] == "1");
    } else {
      y.push_back(initial_state_match[2] == "1");
    }
    std::getline(input_file, line);
  }

  return std::tuple{x, y};
}

std::unordered_map<std::string,
                   std::tuple<std::string, std::string, std::string>>
get_dependency_graph(std::ifstream &input_file) {
  std::unordered_map<std::string,
                     std::tuple<std::string, std::string, std::string>>
      dependency_graph = {};

  std::regex dependency_pattern("(.*) (.*) (.*) -> (.*)");
  std::smatch dependency_match = {};

  std::string line = {};
  while (std::getline(input_file, line)) {
    std::regex_search(line, dependency_match, dependency_pattern);
    dependency_graph[dependency_match[4]] = std::tuple{
        dependency_match[1], dependency_match[2], dependency_match[3]};
  }
  return dependency_graph;
}

size_t get_num_bits(
    const std::unordered_map<std::string,
                             std::tuple<std::string, std::string, std::string>>
        &dependency_graph) {
  size_t num_bits = 0;
  for (const auto &it : dependency_graph) {
    auto wire = it.first;
    if (wire[0] == 'z') {
      size_t wire_num = std::stoi(wire.substr(1, wire.size() - 1));
      num_bits = std::max(num_bits, wire_num);
    }
  }
  return num_bits;
}

long add_numbers(
    const std::unordered_map<std::string,
                             std::tuple<std::string, std::string, std::string>>
        &dependency_graph,
    const std::vector<bool> &x, const std::vector<bool> &y) {
  long res = 0;

  std::unordered_map<std::string, bool> memo = {};
  for (size_t i = 0; i <= x.size() + 1; i++) {
    std::string wire_x = "x";
    std::string wire_y = "y";
    if (i < 10) {
      wire_x += '0' + std::to_string(i);
      wire_y += '0' + std::to_string(i);
    } else {
      wire_x += std::to_string(i);
      wire_y += std::to_string(i);
    }
    memo[wire_x] = x[i];
    memo[wire_y] = y[i];
  }

  for (size_t i = 0; i <= x.size() + 1; i++) {
    std::string wire = "z";
    if (i < 10) {
      wire += '0' + std::to_string(i);
    } else {
      wire += std::to_string(i);
    }
    if (!dependency_graph.contains(wire)) {
      break;
    }

    if (check_on(wire, dependency_graph, memo)) {
      res += 1L << i;
    }
  }

  return res;
}

void test_one_hot_x(
    const std::unordered_map<std::string,
                             std::tuple<std::string, std::string, std::string>>
        &dependency_graph,
    const size_t &b, const size_t &num_bits) {
  std::vector<bool> x(num_bits - 1, false);
  std::vector<bool> y(num_bits - 1, false);
  x[b] = true;

  auto actual = add_numbers(dependency_graph, x, y);
  size_t expected = 1L << b;
  if (actual != expected) {
    std::cout << "One hot x: " << std::endl;
    std::cout << std::setw(10) << "X: " << std::bitset<64>(1L << b)
              << std::endl;
    std::cout << std::setw(10) << "Y: " << std::bitset<64>(0) << std::endl;
    std::cout << std::setw(10) << "Actual: " << std::bitset<64>(actual)
              << std::endl;
    std::cout << std::setw(10) << "Expected: " << std::bitset<64>(expected)
              << std::endl;
  }
}

void test_one_hot_y(
    const std::unordered_map<std::string,
                             std::tuple<std::string, std::string, std::string>>
        &dependency_graph,
    const size_t &b, const size_t &num_bits) {
  std::vector<bool> x(num_bits - 1, false);
  std::vector<bool> y(num_bits - 1, false);
  y[b] = true;

  auto actual = add_numbers(dependency_graph, x, y);
  size_t expected = 1L << b;
  if (actual != expected) {
    std::cout << "One hot y: " << std::endl;
    std::cout << std::setw(10) << "X: " << std::bitset<64>(0) << std::endl;
    std::cout << std::setw(10) << "Y: " << std::bitset<64>(1L << b)
              << std::endl;
    std::cout << std::setw(10) << "Actual: " << std::bitset<64>(actual)
              << std::endl;
    std::cout << std::setw(10) << "Expected: " << std::bitset<64>(expected)
              << std::endl;
  }
}

void test_one_hot_xy(
    const std::unordered_map<std::string,
                             std::tuple<std::string, std::string, std::string>>
        &dependency_graph,
    const size_t &b, const size_t &num_bits) {
  std::vector<bool> x(num_bits - 1, false);
  std::vector<bool> y(num_bits - 1, false);
  x[b] = true;
  y[b] = true;

  auto actual = add_numbers(dependency_graph, x, y);
  size_t expected = 1L << (b + 1);
  if (actual != expected) {
    std::cout << "One hot x and y: " << std::endl;
    std::cout << std::setw(10) << "X: " << std::bitset<64>(1L << b)
              << std::endl;
    std::cout << std::setw(10) << "Y: " << std::bitset<64>(1L << b)
              << std::endl;
    std::cout << std::setw(10) << "Actual: " << std::bitset<64>(actual)
              << std::endl;
    std::cout << std::setw(10) << "Expected: " << std::bitset<64>(expected)
              << std::endl;
  }
}

void probe_gates(const std::unordered_map<
                 std::string, std::tuple<std::string, std::string, std::string>>
                     &dependency_graph) {
  size_t num_bits = get_num_bits(dependency_graph);

  for (size_t b = 0; b < num_bits; b++) {
    std::cout << "Gate: " << b << std::endl;
    test_one_hot_x(dependency_graph, b, num_bits);
    test_one_hot_y(dependency_graph, b, num_bits);
    test_one_hot_xy(dependency_graph, b, num_bits);
  }
}

void perform_swap(
    std::unordered_map<std::string,
                       std::tuple<std::string, std::string, std::string>>
        &dependency_graph,
    const std::string &u, const std::string &v) {
  std::swap(dependency_graph.at(u), dependency_graph.at(v));
}

int main() {
  std::ifstream input_file("data/day_24/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not open file.");
  }

  auto x_and_y = get_input(input_file);
  auto x = std::get<0>(x_and_y);
  auto y = std::get<1>(x_and_y);

  auto dependency_graph = get_dependency_graph(input_file);

  // Manually spot swaps until the gate probing returns no errors.
  std::vector<std::pair<std::string, std::string>> swaps = {
      {"kfp", "hbs"}, {"pdg", "z22"}, {"dhq", "z18"}, {"jcp", "z27"}};
  std::vector<std::string> swapped_gates = {};
  for (const auto &swap : swaps) {
    perform_swap(dependency_graph, swap.first, swap.second);
    swapped_gates.push_back(swap.first);
    swapped_gates.push_back(swap.second);
  }

  probe_gates(dependency_graph);

  std::sort(swapped_gates.begin(), swapped_gates.end());
  for (size_t i = 0; i < swapped_gates.size(); i++) {
    std::cout << swapped_gates[i];
    if (i < swapped_gates.size() - 1) {
      std::cout << ',';
    }
  }
  std::cout << std::endl;

  return 0;
}
