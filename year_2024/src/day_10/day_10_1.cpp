#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

namespace std {
template <> struct hash<tuple<int, int>> {
  size_t operator()(const tuple<int, int> t) const {
    return hash<int>()(get<0>(t) ^ get<1>(t) << 16);
  }
};
} // namespace std

bool check_in_bounds(const std::vector<std::vector<size_t>> &grid, const int &r,
                     const int &c) {
  return r >= 0 and r < grid.size() and c >= 0 and c < grid.size();
}

void get_trail_peaks(const std::vector<std::vector<size_t>> &grid,
                     std::unordered_set<std::tuple<int, int>> &peaks,
                     const int &r, const int &c, size_t target) {
  if (grid[r][c] != target) {
    return;
  } else if (target == 9) {
    peaks.insert(std::tuple(r, c));
    return;
  }

  std::vector<std::tuple<int, int>> directions = {
      {1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  for (std::tuple<int, int> direction : directions) {
    int d_r = std::get<0>(direction);
    int d_c = std::get<1>(direction);

    int n_r = r + d_r;
    int n_c = c + d_c;

    if (check_in_bounds(grid, n_r, n_c) &&
        !peaks.contains(std::tuple(n_r, n_c))) {
      get_trail_peaks(grid, peaks, n_r, n_c, target + 1);
    }
  }
}

size_t get_trail_head_score(const std::vector<std::vector<size_t>> &grid,
                            const int &r, const int &c) {
  std::unordered_set<std::tuple<int, int>> peaks;
  get_trail_peaks(grid, peaks, r, c, 0);
  return peaks.size();
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
      res += get_trail_head_score(grid, r, c);
    }
  }

  std::cout << res << std::endl;
  return 0;
}
