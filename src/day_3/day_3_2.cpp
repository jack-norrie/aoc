#include <cctype>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <tuple>

int find_next_target(std::string line, int start, int end, std::string target) {
  int i = start;
  while (i < end) {
    // Find starting candidate
    while (i < end && line[i] != target[0]) {
      i += 1;
    }

    // Check that the rest matches
    int j = 1;
    bool match = true;
    while (match && i + j < end && j < target.length()) {
      if (line[i + j] != target[j]) {
        match = false;
      }
      j += 1;
    }

    if (match) {
      return i + j - 1;
    } else {
      i += 1;
    }
  }
  return i;
}

int find_next_do(std::string line, int start) {
  return find_next_target(line, start, line.length(), "do()");
}

int find_next_dont(std::string line, int start) {
  return find_next_target(line, start, line.length(), "don't()");
}

int find_next_mul(std::string line, int start, int end) {
  return find_next_target(line, start, end, "mul");
}

std::tuple<int, int> get_next_number(std::string line, int start, int end) {
  int i = start;
  while (i < end && std::isdigit(line[i])) {
    i += 1;
  }

  int num = std::stoi(line.substr(start, i - start));

  return std::tuple(num, i);
}

int compute_result_in_range(std::string line, int start, int end) {
  int res = 0;
  int i = start;
  while (i < end) {
    int num1 = 0;
    int num2 = 0;

    i = find_next_mul(line, i, end) + 1;

    if (i < end && line[i] == '(') {
      i += 1;
    } else {
      continue;
    }

    if (i < end && std::isdigit(line[i])) {
      std::tuple<int, int> t = get_next_number(line, i, end);
      num1 = std::get<0>(t);
      i = std::get<1>(t);
    } else {
      continue;
    }

    if (i < end && line[i] == ',') {
      i += 1;
    } else {
      continue;
    }

    if (i < end && std::isdigit(line[i])) {
      std::tuple<int, int> t = get_next_number(line, i, end);
      num2 = std::get<0>(t);
      i = std::get<1>(t);
    } else {
      continue;
    }

    if (i < end && line[i] == ')') {
      i += 1;
    } else {
      continue;
    }

    res += num1 * num2;
  }
  return res;
}

int main() {
  int res = 0;

  std::ifstream input_file("data/day_3/input");

  if (!input_file.is_open()) {
    std::cout << "Error: Could not open file" << std::endl;
  }

  bool enabled = true;

  std::string line;
  while (std::getline(input_file, line)) {
    int i = 0;
    while (i < line.length()) {
      if (enabled) {
        int next_dont = find_next_dont(line, i);

        res += compute_result_in_range(line, i, next_dont);
        i = next_dont + 1;

        // Check to see if we actually found the next don't point on this line,
        // else the search continues on the next line and we stay active.
        if (next_dont < line.length()) {
          enabled = false;
        }
      } else {
        int next_do = find_next_do(line, i);
        i = next_do + 1;

        // Check to see if we actually find the next do point on this line,
        // else the search continues on the next line and we stay inactive.
        if (next_do < line.length()) {
          enabled = true;
        }
      }
    }
  }
  std::cout << res << std::endl;
}
