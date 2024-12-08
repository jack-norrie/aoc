#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace std {
template <> struct hash<tuple<int, int>> {
  size_t operator()(const tuple<int, int> &t) const {
    return hash<int>()(get<0>(t) ^ get<1>(t) << 16);
  }
};
} // namespace std

std::vector<std::vector<char>> get_grid() {
  std::ifstream input_file("year_2024/data/day_8/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Error: Could not open file.");
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

std::unordered_map<char, std::vector<std::tuple<size_t, size_t>>>
get_antena_sets(const std::vector<std::vector<char>> &grid) {
  std::unordered_map<char, std::vector<std::tuple<size_t, size_t>>> antena_sets;

  size_t n_rows = grid.size();
  size_t n_cols = grid[0].size();

  for (size_t i = 0; i < n_rows; i++) {
    for (size_t j = 0; j < n_cols; j++) {
      if (grid[i][j] != '.') {
        antena_sets[grid[i][j]].push_back(std::tuple(i, j));
      }
    }
  }
  return antena_sets;
}

bool check_in_bounds(const std::vector<std::vector<char>> &grid, const int &r,
                     const int &c) {
  size_t n_rows = grid.size();
  size_t n_cols = grid[0].size();
  return r >= 0 && r < n_rows && c >= 0 && c < n_cols;
}

std::unordered_set<std::tuple<int, int>> get_anti_nodes(
    const std::vector<std::vector<char>> &grid,
    const std::unordered_map<char, std::vector<std::tuple<size_t, size_t>>>
        &antena_sets) {
  std::unordered_set<std::tuple<int, int>> anti_nodes;

  for (const auto &key : antena_sets) {
    for (std::tuple<size_t, size_t> pos_1 : key.second) {
      size_t r_1 = std::get<0>(pos_1);
      size_t c_1 = std::get<1>(pos_1);
      for (std::tuple<size_t, size_t> pos_2 : key.second) {
        if (pos_1 == pos_2) {
          continue;
        }
        size_t r_2 = std::get<0>(pos_2);
        size_t c_2 = std::get<1>(pos_2);

        int d_r = r_2 - r_1;
        int d_c = c_2 - c_1;

        int n_r = r_1 + d_r;
        int n_c = c_1 + d_c;

        while (check_in_bounds(grid, n_r, n_c)) {
          anti_nodes.insert(std::tuple(n_r, n_c));
          n_r += d_r;
          n_c += d_c;
        }
      }
    }
  }

  return anti_nodes;
}

int main() {
  std::vector<std::vector<char>> grid = get_grid();

  std::unordered_map<char, std::vector<std::tuple<size_t, size_t>>>
      antena_sets = get_antena_sets(grid);

  std::unordered_set<std::tuple<int, int>> anti_nodes =
      get_anti_nodes(grid, antena_sets);

  std::cout << anti_nodes.size() << std::endl;
  return 0;
}
