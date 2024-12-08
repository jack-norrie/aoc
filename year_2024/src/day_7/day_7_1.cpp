#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>

std::stack<long long> get_operand_stack(std::string &operands) {
  // Push operands onto a temporary stack.
  std::stack<long long> tmp_stack;
  std::stringstream operand_stream(operands);
  std::string token;
  while (operand_stream >> token) {
    tmp_stack.push(std::stoi(token));
  }

  // Operands are operated on left to right,
  // meaning we need to reverse the stack.
  std::stack<long long> operand_stack;
  while (!tmp_stack.empty()) {
    operand_stack.push(tmp_stack.top());
    tmp_stack.pop();
  }

  return operand_stack;
}

bool check_if_operands_can_make_target(std::stack<long long> &operand_stack,
                                       const long long &target) {
  if (operand_stack.size() == 1) {
    return operand_stack.top() == target;
  }
  // Get top two operands from stack
  long long operand_1 = operand_stack.top();
  operand_stack.pop();
  long long operand_2 = operand_stack.top();
  operand_stack.pop();

  // Addition option
  operand_stack.push(operand_1 + operand_2);
  bool add_opt = check_if_operands_can_make_target(operand_stack, target);
  operand_stack.pop();

  // Multiplication option
  operand_stack.push(operand_1 * operand_2);
  bool mult_opt = check_if_operands_can_make_target(operand_stack, target);
  operand_stack.pop();

  // Reset stack.
  operand_stack.push(operand_2);
  operand_stack.push(operand_1);

  return add_opt || mult_opt;
}

int main() {
  long long res = 0;

  std::ifstream input_file("year_2024/data/day_7/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Error: Could not open file.");
  }

  std::string line;
  while (std::getline(input_file, line)) {
    size_t split_point = line.find(":");
    long long target = std::stoll(line.substr(0, split_point));
    std::string operands =
        line.substr(split_point + 2, line.size() - split_point);

    std::stack<long long> operand_stack = get_operand_stack(operands);

    if (check_if_operands_can_make_target(operand_stack, target)) {
      res += target;
    }
  }

  std::cout << res << std::endl;
  return 0;
}
