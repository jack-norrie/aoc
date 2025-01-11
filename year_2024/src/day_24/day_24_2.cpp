#include <algorithm>
#include <fstream>
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

void perform_swap(
    std::unordered_map<std::string,
                       std::tuple<std::string, std::string, std::string>>
        &dependency_graph,
    const std::string &u, const std::string &v) {
  std::swap(dependency_graph.at(u), dependency_graph.at(v));
}

check_output_valid(
    const std::unordered_map<std::string,
                             std::tuple<std::string, std::string, std::string>>
        &dependency_graph) {}

int main() {
  std::ifstream input_file("data/day_24/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not open file.");
  }

  auto x_and_y = get_input(input_file);
  auto x = std::get<0>(x_and_y);
  auto y = std::get<1>(x_and_y);

  auto dependency_graph = get_dependency_graph(input_file);

  std::vector<std::pair<std::string, std::string>> swaps = {};
  std::vector<std::string> swapped_gates = {};

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
