#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>

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

int main() {
  std::ifstream input_file("data/day_24/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not open file.");
  }

  std::unordered_map<std::string, bool> memo = {};
  std::string line = {};
  std::getline(input_file, line);
  std::regex initial_state_pattern("(.*): (\\d)");
  std::smatch initial_state_match = {};
  while (line != "") {
    std::regex_search(line, initial_state_match, initial_state_pattern);
    memo[initial_state_match[1]] = (initial_state_match[2] == "1");
    std::getline(input_file, line);
  }

  std::unordered_map<std::string,
                     std::tuple<std::string, std::string, std::string>>
      dependency_graph = {};
  std::regex dependency_pattern("(.*) (.*) (.*) -> (.*)");
  std::smatch dependency_match = {};
  while (std::getline(input_file, line)) {
    std::regex_search(line, dependency_match, dependency_pattern);
    dependency_graph[dependency_match[4]] = std::tuple{
        dependency_match[1], dependency_match[2], dependency_match[3]};
  }

  long res = 0;
  size_t i = 0;
  while (true) {
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
    } else {
    }

    ++i;
  }

  std::cout << res << std::endl;

  return 0;
}
