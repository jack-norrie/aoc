#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

namespace std {
template <> struct hash<tuple<int, int>> {
  size_t operator()(const tuple<int, int> t) const {
    return get<0>(t) ^ get<1>(t);
  }
};
} // namespace std

std::vector<std::vector<char>> get_grid() {
  std::ifstream input_file("data/day_12/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Error: Could not open file");
  }

  std::vector<std::vector<char>> grid;
  std::string line;
  while (std::getline(input_file, line)) {
    std::vector<char> row;
    for (char c : line) {
      row.push_back(c);
    }
    grid.push_back(row);
  }

  return grid;
}

bool in_bounds(const std::vector<std::vector<char>> &grid, int r, int c) {
  return r >= 0 && r < grid.size() && c >= 0 && c < grid[0].size();
}

std::vector<std::tuple<int, int>> directions = {
    {1, 0}, {0, 1}, {-1, 0}, {0, -1}};

size_t get_sides_contribution(const std::vector<std::vector<char>> &grid, int r,
                              int c) {
  size_t res = 0;

  for (std::tuple<int, int> direction : directions) {
    int d_r = std::get<0>(direction);
    int d_c = std::get<1>(direction);

    int n_r = r + d_r;
    int n_c = c + d_c;

    if (!in_bounds(grid, n_r, n_c) || grid[n_r][n_c] != grid[r][c]) {
      ++res;
    }
  }

  return res;
}

void get_area_and_sides_dfs(const std::vector<std::vector<char>> &grid,
                            std::unordered_set<std::tuple<int, int>> &seen,
                            int r, int c, size_t &area, size_t &perimiter) {
  seen.insert(std::tuple(r, c));

  ++area;
  perimiter += get_sides_contribution(grid, r, c);

  for (std::tuple<int, int> direction : directions) {
    int d_r = std::get<0>(direction);
    int d_c = std::get<1>(direction);

    int n_r = r + d_r;
    int n_c = c + d_c;

    if (in_bounds(grid, n_r, n_c) && !seen.contains(std::tuple(n_r, n_c)) &&
        grid[n_r][n_c] == grid[r][c]) {
      get_area_and_sides_dfs(grid, seen, n_r, n_c, area, perimiter);
    }
  }
}

int get_cost(const std::vector<std::vector<char>> &grid) {
  int res = 0;

  std::unordered_set<std::tuple<int, int>> seen;

  size_t n_row = grid.size();
  size_t n_col = grid[0].size();

  for (size_t r = 0; r < n_row; r++) {
    for (size_t c = 0; c < n_col; c++) {
      if (!seen.contains(std::tuple(r, c))) {
        size_t area = 0;
        size_t perimiter = 0;

        get_area_and_sides_dfs(grid, seen, r, c, area, perimiter);

        res += area * perimiter;
      }
    }
  }

  return res;
}

int main() {
  std::vector<std::vector<char>> grid = get_grid();

  int res = get_cost(grid);

  std::cout << res << std::endl;

  return 0;
}
