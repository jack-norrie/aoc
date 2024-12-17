#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

bool check_in_bounds(const std::vector<std::vector<size_t>> &grid, const int &r,
                     const int &c) {
  return r >= 0 and r < grid.size() and c >= 0 and c < grid.size();
}

size_t get_trail_head_score(const std::vector<std::vector<size_t>> &grid,
                            const int &r, const int &c, size_t target) {
  size_t res = 0;

  if (grid[r][c] != target) {
    return 0;
  } else if (target == 9) {
    return 1;
  }

  std::vector<std::tuple<int, int>> directions = {
      {1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  for (std::tuple<int, int> direction : directions) {
    int d_r = std::get<0>(direction);
    int d_c = std::get<1>(direction);

    int n_r = r + d_r;
    int n_c = c + d_c;

    if (check_in_bounds(grid, n_r, n_c)) {
      res += get_trail_head_score(grid, n_r, n_c, target + 1);
    }
  }
  return res;
}

int main() {
  int res = 0;

  std::ifstream input_file("data/day_10/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Error: Could not open file.");
    return 1;
  }

  std::string line;
  std::vector<std::vector<size_t>> grid;
  while (std::getline(input_file, line)) {
    std::vector<size_t> row;
    for (char c : line) {
      row.push_back(c - '0');
    }
    grid.push_back(row);
  }

  for (int r = 0; r < grid.size(); r++) {
    for (int c = 0; c < grid[0].size(); c++) {
      res += get_trail_head_score(grid, r, c, 0);
    }
  }

  std::cout << res << std::endl;
  return 0;
}
