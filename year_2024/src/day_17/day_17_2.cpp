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

std::vector<size_t>
strip_loop_logic_from_program(const std::vector<size_t> &program) {
  std::vector<size_t> program_trunc = {};

  for (size_t i = 0; i < program.size(); i += 2) {
    size_t operator_ = program[i];
    size_t operand = program[i + 1];

    if (operator_ == 0 || operator_ == 3) {
      continue;
    }

    program_trunc.push_back(operator_);
    program_trunc.push_back(operand);
  }

  return program_trunc;
}

size_t get_initial_register(const std::vector<size_t> &program,
                            size_t a_register, size_t program_idx) {
  std::vector<size_t> program_loop = strip_loop_logic_from_program(program);

  // Iterate over the program in reverse order.
  size_t target_output = program[program_idx];

  // If we are on the first block, then this value cannot be zero, else the
  // output will not be the required length.
  size_t j_start = 0;
  if (program_idx == program.size() - 1) {
    j_start += 1;
  }

  // Try each possible register value for this block.
  for (size_t j = j_start; j < 8; j++) {
    std::vector<size_t> registers = {a_register + j, 0, 0};
    size_t output = run_program(program_loop, registers)[0];

    if (output == target_output) {
      if (program_idx == 0) {
        // We have found a match for all program positions, return this non-zero
        // match.
        return a_register + j;
      } else {
        size_t potential_res = get_initial_register(
            program, (a_register + j) << 3, program_idx - 1);
        if (potential_res > 0) {
          return potential_res;
        }
      }
    }
  }
  return 0;
}

int main() {
  // Assumption - The programs are of the following general form:
  //  while A > 0:
  //    # At the start of each loop pull A and B values from A.
  //    # i.e. no "memory" of A and B registers from previous loops.
  //    B = f_B(A)
  //    C = f_C(A)
  //
  //    # Perform coupling processing on B and C
  //    B = g_B(A, B, C)
  //    C = g_C(A, B, C)
  //
  //    # Print one of the registers
  //    PRINT(h(A, B, C) % 8)
  //
  //    # Perform some monotonically descresing operation on A.
  //    A = f_A(A)
  //
  // Simplification - Since the B and C registers are reset/sourced from A at
  // the start each time we have:
  //  while A > 0:
  //    PRINT(f(A) % 8)
  //    A = d(A)
  //
  // The above simplification is important since it means that each loop can
  // be processed indepdently of the state associated with previously loops,
  // this will be important when we motivate a "backwards" passing algorithm
  // later.
  //
  // Further Simplifing Assumption - The decrement operation is always 3 bit
  // shifts:
  //  while A > 0:
  //    PRINT(f(A) % 8)
  //    A = A << 3
  //
  // This final assumption means that a length x ouput requires an initial A
  // register length of size (3(x-1), 3x] in binary.
  //
  // A brute force solution would therefore be to try all 2^3x possible
  // initial register values. Assuming the loop is a constant time operation
  // and we are performing x loops, then this would lead to a time complexity
  // of O(x8^x).
  //
  // This will take a very long time to compute, it would be smart to try and
  // eliminate some paths in our traversal using the program outputs.
  //
  // If we denote the print value at iteration t as v_t, then we want to solve
  // equations of the form:
  // * A_t = A_t-1 >> 3
  // * f(A_t) % 8 = v_t
  //
  // Trying to solve this problem in the forward direction will lead to
  // problems, since our values only let us solve for the least significant 3
  // bits. This prevents us evaluating f(A_t), since the majority of the bits
  // are unknown, i.e. we still have the same combinatorial complexity in
  // searching through the A register space.
  //
  // To overcome this issue of not knowing the starting bits, it would be smart
  // to maybe start from a position where we do know the a register value.
  // Fortunately, we know that the loop terminates, i.e. the program ends with a
  // A register value of 0. If we move to the point right before the final
  // decrement operation we are left with a register with only 3 bits. This
  // means we are free to solve the least significant 3 bit constraint without
  // having to know the previous bit values (there aren't any).
  //
  // Furthermore, we are now free to use the next constraint in reverse order
  // since we now know the previous bits for this register state via the
  // previous solution.
  //
  // We can continue like this until we have a a register value satisfying all
  // the constraints.
  //
  // A final caveat is that the function f(A_t) = v_t might have multiple
  // solutions. This means we are potentially going to explore a tree of
  // possible a register values, where by the currently found a value block
  // might get invalidated by a later constraint. This traversal can be
  // performed using a depth first search.

  std::ifstream input_file("data/day_17/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not open file.");
  }

  // This is still required to exhuast the file stream.
  std::vector<size_t> _ = get_registers(input_file);

  std::vector<size_t> program = get_program(input_file);

  size_t a_init = get_initial_register(program, 0, program.size() - 1);
  std::cout << "Initial A register: " << a_init << std::endl;

  // Verify that when the registers are set to those found
  // above, that the original program is returned
  std::vector<size_t> registers = {a_init, 0, 0};
  std::vector<size_t> outputs = run_program(program, registers);
  if (outputs != program) {
    throw std::runtime_error("Program did not return program as output.");
  }
  std::cout << "Program:" << std::endl;
  print_outputs(program);
  std::cout << "Output:" << std::endl;
  print_outputs(outputs);

  return 0;
}
