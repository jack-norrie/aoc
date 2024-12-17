#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

int main() {
  std::ifstream input_file("year_2024/data/day_14/test_case_1");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not open file.");
  }

  std::string line = {};
  while (std::getline(input_file, line)) {
    std::cout << line << std::endl;
  }
}
