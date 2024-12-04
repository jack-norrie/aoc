#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

int main() {
  std::ifstream input_file("data/day_1/input");

  if (!input_file.is_open()) {
    std::cerr << "Error: Could not open input file" << std::endl;
    return 1;
  }

  std::vector<int> column1;
  std::vector<int> column2;
  std::string line;
  while (std::getline(input_file, line)) {
    size_t split_pos = line.find("   ");

    size_t val1 = std::stoi(line.substr(0, split_pos));
    column1.push_back(val1);

    size_t val2 = std::stoi(line.substr(split_pos + 1));
    column2.push_back(val2);
  }

  std::sort(column1.begin(), column1.end());
  std::sort(column2.begin(), column2.end());

  size_t n = column1.size();
  int res = 0;
  for (size_t i = 0; i < n; i++) {
    res = res + std::abs(column1[i] - column2[i]);
  }

  std::cout << res << std::endl;

  return 0;
}
