#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

std::string get_wire_name(const std::string &c, const int &i) {
  if (i < 10) {
    return c + "0" + std::to_string(i);
  } else {
    return c + std::to_string(i);
  }
}

bool validate_input_xor(
    const std::unordered_map<std::string,
                             std::tuple<std::string, std::string, std::string>>
        &dependency_graph,
    const std::string &wire, const int &i,
    std::unordered_set<std::string> &validated_wires,
    std::unordered_set<std::string> &seen_wires) {
  if (validated_wires.contains(wire)) {
    return true;
  } else {
    seen_wires.insert(wire);
  }

  if (!dependency_graph.contains(wire)) {
    // Implies we have an input, not a gate output.
    return false;
  }

  auto [operand_1, operator_, operand_2] = dependency_graph.at(wire);
  if (operator_ != "XOR") {
    return false;
  }
  // Check input wires match z wire.
  std::string xwire = get_wire_name("x", i);
  std::string ywire = get_wire_name("y", i);
  bool valid_option_1 = operand_1 == xwire && operand_2 == ywire;
  bool valid_option_2 = operand_2 == xwire && operand_1 == ywire;
  return (valid_option_1 || valid_option_2);
}

bool validate_input_and(
    const std::unordered_map<std::string,
                             std::tuple<std::string, std::string, std::string>>
        &dependency_graph,
    const std::string &wire, const int &i,
    std::unordered_set<std::string> &validated_wires,
    std::unordered_set<std::string> &seen_wires) {
  if (validated_wires.contains(wire)) {
    return true;
  } else {
    seen_wires.insert(wire);
  }

  if (!dependency_graph.contains(wire)) {
    // Implies we have an input, not a gate output.
    return false;
  }

  auto [operand_1, operator_, operand_2] = dependency_graph.at(wire);
  if (operator_ != "AND") {
    return false;
  }
  std::string xwire = get_wire_name("x", i);
  std::string ywire = get_wire_name("y", i);
  bool valid_option_1 = operand_1 == xwire && operand_2 == ywire;
  bool valid_option_2 = operand_2 == xwire && operand_1 == ywire;
  return (valid_option_1 || valid_option_2);
}

// Function prototype - since validate_carry and validate_intermediate_and
// both depend on each other.
bool validate_carry(
    const std::unordered_map<std::string,
                             std::tuple<std::string, std::string, std::string>>
        &dependency_graph,
    const std::string &wire, const int &i,
    std::unordered_set<std::string> &validated_wires,
    std::unordered_set<std::string> &seen_wires);

bool validate_intermediate_and(
    const std::unordered_map<std::string,
                             std::tuple<std::string, std::string, std::string>>
        &dependency_graph,
    const std::string &wire, const int &i,
    std::unordered_set<std::string> &validated_wires,
    std::unordered_set<std::string> &seen_wires) {
  if (validated_wires.contains(wire)) {
    return true;
  } else {
    seen_wires.insert(wire);
  }

  if (!dependency_graph.contains(wire)) {
    // Implies we have an input, not a gate output.
    return false;
  }
  auto [operand_1, operator_, operand_2] = dependency_graph.at(wire);
  if (operator_ != "AND") {
    return false;
  }
  bool valid_option_1 = validate_input_xor(dependency_graph, operand_1, i,
                                           validated_wires, seen_wires) &&
                        validate_carry(dependency_graph, operand_2, i - 1,
                                       validated_wires, seen_wires);
  bool valid_option_2 = validate_input_xor(dependency_graph, operand_2, i,
                                           validated_wires, seen_wires) &&
                        validate_carry(dependency_graph, operand_1, i - 1,
                                       validated_wires, seen_wires);
  return (valid_option_1 || valid_option_2);
}

bool validate_carry(
    const std::unordered_map<std::string,
                             std::tuple<std::string, std::string, std::string>>
        &dependency_graph,
    const std::string &wire, const int &i,
    std::unordered_set<std::string> &validated_wires,
    std::unordered_set<std::string> &seen_wires) {
  if (validated_wires.contains(wire)) {
    return true;
  } else {
    seen_wires.insert(wire);
  }

  // Base case i == 0: in which case we have a half-adder and our
  // wire should be equal to the first and input gate.
  if (i == 0) {
    return validate_input_and(dependency_graph, wire, i, validated_wires,
                              seen_wires);
  }

  if (!dependency_graph.contains(wire)) {
    // Implies we have an input, not a gate output.
    return false;
  }

  auto [operand_1, operator_, operand_2] = dependency_graph.at(wire);
  if (operator_ != "OR") {
    return false;
  }
  bool valid_option_1 =
      validate_input_and(dependency_graph, operand_1, i, validated_wires,
                         seen_wires) &&
      validate_intermediate_and(dependency_graph, operand_2, i, validated_wires,
                                seen_wires);
  bool valid_option_2 =
      validate_input_and(dependency_graph, operand_2, i, validated_wires,
                         seen_wires) &&
      validate_intermediate_and(dependency_graph, operand_1, i, validated_wires,
                                seen_wires);
  return (valid_option_1 || valid_option_2);
}

bool validate_output_wire(
    const std::unordered_map<std::string,
                             std::tuple<std::string, std::string, std::string>>
        &dependency_graph,
    const int &i, std::unordered_set<std::string> &validated_wires,
    std::unordered_set<std::string> &seen_wires) {

  std::string wire = get_wire_name("z", i);
  if (validated_wires.contains(wire)) {
    return true;
  } else {
    seen_wires.insert(wire);
  }

  // Base case i == 0: in which case we have a half-adder and our
  // wire should be equal to the first xor input gate.
  if (i == 0) {
    return validate_input_xor(dependency_graph, wire, i, validated_wires,
                              seen_wires);
  }

  if (!dependency_graph.contains(wire)) {
    // Implies we have an input, not a gate output.
    return false;
  }

  auto [operand_1, operator_, operand_2] = dependency_graph.at(wire);
  if (operator_ != "XOR") {
    return false;
  }
  bool valid_option_1 = validate_input_xor(dependency_graph, operand_1, i,
                                           validated_wires, seen_wires) &&
                        validate_carry(dependency_graph, operand_2, i - 1,
                                       validated_wires, seen_wires);
  bool valid_option_2 = validate_input_xor(dependency_graph, operand_2, i,
                                           validated_wires, seen_wires) &&
                        validate_carry(dependency_graph, operand_1, i - 1,
                                       validated_wires, seen_wires);
  return (valid_option_1 || valid_option_2);
}

std::unordered_set<std::string>
set_difference(const std::unordered_set<std::string> &a,
               const std::unordered_set<std::string> &b) {
  std::unordered_set<std::string> c = {};
  for (const auto &x : a)
    if (!b.contains(x)) {
      c.insert(x);
    }
  return c;
}

std::vector<std::string> correct_circuit(
    std::unordered_map<std::string,
                       std::tuple<std::string, std::string, std::string>>
        &dependency_graph) {
  // This approach is O(kn^2)
  // - n: Number of bits in the adder.
  // - k: Number of wire wires swapped.
  std::vector<std::string> swapped_gates = {};

  size_t b = get_num_bits(dependency_graph);

  std::unordered_set<std::string> wire_set = {};
  for (const auto &it : dependency_graph) {
    wire_set.insert(it.first);
  }

  std::unordered_set<std::string> validated_wires = {};
  // First if block is O(n) and is entered n-k times, meanwhile second block is
  // O(n) and is enetered k times over the course of this loop. This leading to
  // an overall time complexity of O((n-k)n + kn^2) = O(kn^2)
  for (size_t i = 0; i < b; i++) {
    // O(n)
    std::unordered_set<std::string> seen_wires = validated_wires;

    // O(1) - Each iteration evaluates a fixed size full adder, i.e. previously
    // validated are not recomputed.
    if (validate_output_wire(dependency_graph, i, validated_wires,
                             seen_wires)) {
      validated_wires = seen_wires;
    } else {
      // Both of these set difference operations are O(n), but the size of the
      // resulting sets are:
      // O(1) - Size of full adder is fixed.
      auto bad_set_1 = set_difference(seen_wires, validated_wires);
      // O(n) _ size of remaining unvalidated circuit is linear.
      auto bad_set_2 = set_difference(wire_set, validated_wires);

      bool valid = false;
      // O(1 * n * n) = O(n^2)
      for (auto const &wire_1 : bad_set_1) {
        for (auto const &wire_2 : bad_set_2) {
          if (wire_1 != wire_2) {
            // O(n)
            std::unordered_set<std::string> seen_wires = validated_wires;
            perform_swap(dependency_graph, wire_1, wire_2);
            if (validate_output_wire(dependency_graph, i, validated_wires,
                                     seen_wires)) {
              valid = true;
              validated_wires = seen_wires;
              swapped_gates.push_back(wire_1);
              swapped_gates.push_back(wire_2);
              break;
            } else {
              perform_swap(dependency_graph, wire_1, wire_2);
            }
          }
        }
        if (valid) {
          break;
        }
      }
      if (!valid) {
        throw std::runtime_error(
            "Could not find swap to make dependency_graph valid.");
      }
    }
  }

  // Note: We do not need to check the last carry bit (i.e. our loop is <b not
  // <=b) since the only gates not validated are the final or gate and its
  // previous and gate. These two gates cannot be swapped without introducing a
  // cycle, which cannot be done by the contrants of the problem.
  return swapped_gates;
}

int main() {
  // This solution involves iteratively checking each full-adder circuit, i.e.
  // seeing if it conforms to the full-adder expected structure. If a problem
  // was found with a full-adder then it's gates were swapped with all the
  // remaining unvalidated full adder gates until the full adder was valid. This
  // procedure was continued until all full adders were valid.

  std::ifstream input_file("data/day_24/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not open file.");
  }

  auto x_and_y = get_input(input_file);

  auto dependency_graph = get_dependency_graph(input_file);

  auto swapped_gates = correct_circuit(dependency_graph);

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
