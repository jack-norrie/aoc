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
    return get<0>(t) ^ get<1>(t) << 16;
  }
};
template <> struct hash<tuple<int, int, int, int>> {
  size_t operator()(const tuple<int, int, int, int> t) const {
    return get<0>(t) ^ get<1>(t) << 8 ^ get<2>(t) << 16 ^ get<3>(t) << 24;
  }
};
} // namespace std

std::vector<std::vector<char>> get_grid() {
  std::ifstream input_file("year_2024/data/day_12/input");
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

size_t get_sides_contribution(
    const std::vector<std::vector<char>> &grid,
    std::unordered_set<std::tuple<int, int, int, int>> &seen_edges, int r,
    int c) {
  size_t res = 0;

  // Check for edges in each direction.
  for (std::tuple<int, int> direction : directions) {
    int d_r = std::get<0>(direction);
    int d_c = std::get<1>(direction);

    // Get interior and exterior point associated with edge.
    int i_r = r;
    int i_c = c;
    int e_r = r + d_r;
    int e_c = c + d_c;

    // Check if interior-exterior pair is associated with an edge.
    if (!seen_edges.contains(std::tuple(i_r, i_c, e_r, e_c)) &&
        (!in_bounds(grid, e_r, e_c) || grid[e_r][e_c] != grid[r][c]) &&
        (in_bounds(grid, i_r, i_c) && grid[i_r][i_c] == grid[r][c])) {
      ++res;
      seen_edges.insert(std::tuple(i_r, i_c, e_r, e_c));

      // Extend edge perpendicular to normal direction of edge so that edge
      // elements that are part of this edge are not double counted.
      i_r = r + d_c;
      i_c = c + d_r;
      e_r = r + d_r + d_c;
      e_c = c + d_c + d_r;
      while (!seen_edges.contains(std::tuple(i_r, i_c, e_r, e_c)) &&
             (!in_bounds(grid, e_r, e_c) || grid[e_r][e_c] != grid[r][c]) &&
             (in_bounds(grid, i_r, i_c) && grid[i_r][i_c] == grid[r][c])) {
        seen_edges.insert(std::tuple(i_r, i_c, e_r, e_c));
        i_r += d_c;
        i_c += d_r;
        e_r += d_c;
        e_c += d_r;
      }

      // Repeat for opposite perpendicular direction.
      i_r = r - d_c;
      i_c = c - d_r;
      e_r = r + d_r - d_c;
      e_c = c + d_c - d_r;
      while (!seen_edges.contains(std::tuple(i_r, i_c, e_r, e_c)) &&
             (!in_bounds(grid, e_r, e_c) || grid[e_r][e_c] != grid[r][c]) &&
             (in_bounds(grid, i_r, i_c) && grid[i_r][i_c] == grid[r][c])) {
        seen_edges.insert(std::tuple(i_r, i_c, e_r, e_c));
        i_r -= d_c;
        i_c -= d_r;
        e_r -= d_c;
        e_c -= d_r;
      }
    }
  }

  return res;
}

void get_area_and_sides_dfs(
    const std::vector<std::vector<char>> &grid,
    std::unordered_set<std::tuple<int, int>> &seen,
    std::unordered_set<std::tuple<int, int, int, int>> &seen_edges, int r,
    int c, size_t &area, size_t &sides) {
  seen.insert(std::tuple(r, c));

  ++area;
  sides += get_sides_contribution(grid, seen_edges, r, c);

  for (std::tuple<int, int> direction : directions) {
    int d_r = std::get<0>(direction);
    int d_c = std::get<1>(direction);

    int n_r = r + d_r;
    int n_c = c + d_c;

    if (in_bounds(grid, n_r, n_c) && !seen.contains(std::tuple(n_r, n_c)) &&
        grid[n_r][n_c] == grid[r][c]) {
      get_area_and_sides_dfs(grid, seen, seen_edges, n_r, n_c, area, sides);
    }
  }
}

int get_cost(const std::vector<std::vector<char>> &grid) {
  int res = 0;

  std::unordered_set<std::tuple<int, int>> seen = {};

  size_t n_row = grid.size();
  size_t n_col = grid[0].size();

  for (size_t r = 0; r < n_row; r++) {
    for (size_t c = 0; c < n_col; c++) {
      if (!seen.contains(std::tuple(r, c))) {
        size_t area = 0;
        size_t sides = 0;

        std::unordered_set<std::tuple<int, int, int, int>> seen_edges = {};
        get_area_and_sides_dfs(grid, seen, seen_edges, r, c, area, sides);

        std::cout << "Region: " << grid[r][c] << std::endl;
        std::cout << "Area: " << area << std::endl;
        std::cout << "Sides: " << sides << std::endl;
        std::cout << std::endl;

        res += area * sides;
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
