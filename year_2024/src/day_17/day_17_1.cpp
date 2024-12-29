#include <cstddef>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<size_t> get_registers(std::ifstream &input_file) {
  std::vector<size_t> registers;
  std::string line = {};

  std::regex register_pattern("Register \\w: (\\d+)");
  std::smatch register_match = {};

  std::getline(input_file, line);
  while (line != "") {
    std::regex_search(line, register_match, register_pattern);
    registers.push_back(std::stoi(register_match[1]));

    std::getline(input_file, line);
  }
  return registers;
}

std::vector<size_t> get_program(std::ifstream &input_file) {
  std::vector<size_t> program;
  std::string line = {};

  std::getline(input_file, line);

  std::regex program_pattern("Program: (.+)");
  std::smatch program_match = {};
  std::regex_search(line, program_match, program_pattern);

  std::stringstream program_ss(program_match[1]);
  std::string instruction = {};
  while (std::getline(program_ss, instruction, ',')) {
    program.push_back(std::stoi(instruction));
  }
  return program;
}

size_t combo(const std::vector<size_t> &registers, const size_t &operand) {
  if (operand < 4) {
    return operand;
  } else if (operand < 7) {
    return registers[operand - 4];
  } else {
    throw std::runtime_error("Invalid operand in the context of combo.");
  }
}

size_t compute(std::vector<size_t> &registers, size_t &i,
               const size_t &operator_, const size_t &operand) {
  i += 2;

  if (operator_ == 0) {
    registers[0] = registers[0] >> combo(registers, operand);
  } else if (operator_ == 1) {
    registers[1] ^= operand;
  } else if (operator_ == 2) {
    registers[1] = combo(registers, operand) % 8;
  } else if (operator_ == 3) {
    if (registers[0] > 0) {
      i = operand;
    }
  } else if (operator_ == 4) {
    registers[1] ^= registers[2];
  } else if (operator_ == 5) {
    return combo(registers, operand) % 8;
  } else if (operator_ == 6) {
    registers[1] = registers[0] >> combo(registers, operand);
  } else if (operator_ == 7) {
    registers[2] = registers[0] >> combo(registers, operand);
  }
  return 8;
}

std::vector<size_t> run_program(const std::vector<size_t> &program,
                                std::vector<size_t> &registers) {
  std::vector<size_t> outputs = {};

  size_t i = 0;
  while (i < program.size()) {
    size_t operator_ = program[i];
    size_t operand = program[i + 1];

    size_t output = compute(registers, i, operator_, operand);

    if (output < 8) {
      outputs.push_back(output);
    }
  }

  return outputs;
}

void print_outputs(std::vector<size_t> outputs) {
  for (size_t i = 0; i < outputs.size(); i++) {
    std::cout << outputs[i];
    if (i < outputs.size() - 1) {
      std::cout << ',';
    }
  }
  std::cout << std::endl;
}

int main() {
  std::ifstream input_file("data/day_17/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not open file.");
  }

  std::vector<size_t> registers = get_registers(input_file);

  std::vector<size_t> program = get_program(input_file);

  std::vector<size_t> outputs = run_program(program, registers);

  print_outputs(outputs);

  return 0;
}
