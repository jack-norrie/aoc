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

int main() {
  std::ifstream input_file("data/day_24/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not open file.");
  }

  auto x_and_y = get_input(input_file);
  auto x = std::get<0>(x_and_y);
  auto y = std::get<1>(x_and_y);

  auto dependency_graph = get_dependency_graph(input_file);

  auto res = add_numbers(dependency_graph, x, y);

  std::cout << res << std::endl;

  return 0;
}
