#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <tuple>

int find_end_of_next_mul_statement(std::string line, int i) {
  while (i < line.length()) {
    // Find starting candidate
    while (i < line.length() && line[i] != 'm') {
      i += 1;
    }

    if (i + 3 < line.length()) {
      if (line[i + 1] == 'u' && line[i + 2] == 'l') {
        return i + 3;
      } else {
        i += 1;
      }
    } else {
      return line.length();
    }
  }
  return i;
}

std::tuple<int, size_t> get_next_number(std::string line, size_t i) {
  size_t start = i;
  while (i < line.length() && std::isdigit(line[i])) {
    i += 1;
  }

  int num = std::stoi(line.substr(start, i - start));

  return std::tuple(num, i);
}

int main() {
  int res = 0;

  std::ifstream input_file("data/day_3/input");

  if (!input_file.is_open()) {
    std::cout << "Error: Could not open file" << std::endl;
  }

  // input should be a single line.
  std::string line;
  while (std::getline(input_file, line)) {

    size_t i = 0;
    while (i < line.length()) {
      int num1 = 0;
      int num2 = 0;

      std::cout << i << "/" << line.length() << std::endl;

      i = find_end_of_next_mul_statement(line, i);

      if (i < line.length() && line[i] == '(') {
        i += 1;
      } else {
        continue;
      }

      if (i < line.length() && std::isdigit(line[i])) {
        std::tuple<int, size_t> t = get_next_number(line, i);
        num1 = std::get<0>(t);
        i = std::get<1>(t);
      } else {
        continue;
      }

      if (i < line.length() && line[i] == ',') {
        i += 1;
      } else {
        continue;
      }

      if (i < line.length() && std::isdigit(line[i])) {
        std::tuple<int, size_t> t = get_next_number(line, i);
        num2 = std::get<0>(t);
        i = std::get<1>(t);
      } else {
        continue;
      }

      if (i < line.length() && line[i] == ')') {
        i += 1;
      } else {
        continue;
      }

      res += num1 * num2;
    }
  }
  std::cout << res << std::endl;
}
