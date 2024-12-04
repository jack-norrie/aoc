#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
  std::ifstream input_file("data/day_1/input");

  if (!input_file.is_open()) {
    std::cerr << "Error: Could not open file" << std::endl;
    return 1;
  }

  std::vector<int> column1;
  std::vector<int> column2;
  std::string line;

  while (std::getline(input_file, line)) {
    uint split_point = line.find("   ");

    uint val1 = std::stoi(line.substr(0, split_point));
    column1.push_back(val1);

    uint val2 = std::stoi(line.substr(split_point + 1));
    column2.push_back(val2);
  }

  uint n = column1.size();
  std::unordered_map<uint, uint> column2_counts;
  for (int i = 0; i < n; i++) {
    uint val2 = column2[i];
    column2_counts[val2] += 1;
  }

  uint res = 0;
  for (int i = 0; i < n; i++) {
    uint val1 = column1[i];
    res += val1 * column2_counts[val1];
  }

  std::cout << res << std::endl;

  return 0;
}
