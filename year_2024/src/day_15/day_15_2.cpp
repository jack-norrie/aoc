#include <array>
#include <cwchar>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace std {
template <> struct hash<tuple<size_t, size_t>> {
  size_t operator()(const tuple<size_t, size_t> t) const {
    return get<0>(t) ^ get<1>(t) << 16;
  }
};
} // namespace std

std::unordered_map<char, std::array<int, 2>> direction_symbol_to_coords = {
    {'^', {-1, 0}}, {'>', {0, 1}}, {'v', {1, 0}}, {'<', {0, -1}}};

std::vector<std::vector<char>> get_grid(std::ifstream &input_file) {
  std::string line = {};
  std::getline(input_file, line);

  std::vector<std::vector<char>> grid;
  while (line != "") {
    std::vector<char> row = {};
    for (char c : line) {
      if (c == 'O') {
        row.push_back('[');
        row.push_back(']');
      } else if (c == '@') {
        row.push_back('@');
        row.push_back('.');
      } else {
        row.push_back(c);
        row.push_back(c);
      }
    }
    grid.push_back(row);
    std::getline(input_file, line);
  }

  return grid;
}

std::vector<char> get_direction_symbols(std::ifstream &input_file) {
  std::vector<char> direction_symbols = {};

  std::string line = {};
  while (std::getline(input_file, line)) {
    for (char c : line) {
      direction_symbols.push_back(c);
    }
  }

  return direction_symbols;
}

std::array<size_t, 2> get_position(const std::vector<std::vector<char>> &grid) {
  size_t n_rows = grid.size();
  size_t n_cols = grid[0].size();

  for (size_t r = 1; r < n_rows - 1; r++) {
    for (size_t c = 1; c < n_cols - 1; c++) {
      if (grid[r][c] == '@') {
        return std::array<size_t, 2>{r, c};
      }
    }
  }

  throw std::runtime_error("Could not find start position");
}

bool check_in_bounds(const std::vector<std::vector<char>> &grid,
                     const size_t &r, const size_t &c) {
  return r > 0 && r < grid.size() - 1 && c > 0 && c < grid[0].size() - 1;
}

void draw_grid(std::vector<std::vector<char>> &grid) {
  size_t n_rows = grid.size();
  size_t n_cols = grid[0].size();
  for (size_t r = 0; r < n_rows; r++) {
    for (size_t c = 0; c < n_cols; c++) {
      std::cout << grid[r][c];
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

bool check_no_blocking_walls(
    const std::vector<std::vector<char>> &grid,
    const std::array<size_t, 2> &position, const std::array<int, 2> &direction,
    std::unordered_set<std::tuple<size_t, size_t>> &seen) {
  size_t r = position[0];
  size_t c = position[1];
  seen.insert(std::tuple(r, c));

  if (grid[r][c] == '.') {
    return true;
  } else if (grid[r][c] == '#') {
    return false;
  }

  size_t n_r = position[0] + direction[0];
  size_t n_c = position[1] + direction[1];
  bool ahead_unblocked = true;
  if (!seen.contains(std::tuple(n_r, n_c))) {
    std::array<size_t, 2> ahead_pos = {n_r, n_c};
    ahead_unblocked = check_no_blocking_walls(grid, ahead_pos, direction, seen);
  }

  bool pair_unblocked = true;
  std::array<size_t, 2> box_pair_pos = {};
  if (grid[r][c] == '[') {
    box_pair_pos = {r, c + 1};
    if (!seen.contains(std::tuple(box_pair_pos[0], box_pair_pos[1]))) {
      pair_unblocked =
          check_no_blocking_walls(grid, box_pair_pos, direction, seen);
    }
  } else if (grid[r][c] == ']') {
    box_pair_pos = {r, c - 1};
    if (!seen.contains(std::tuple(box_pair_pos[0], box_pair_pos[1]))) {
      pair_unblocked =
          check_no_blocking_walls(grid, box_pair_pos, direction, seen);
    }
  }

  return ahead_unblocked && pair_unblocked;
}

void advance_movable(std::vector<std::vector<char>> &grid,
                     const std::array<size_t, 2> &position,
                     const std::array<int, 2> &direction,
                     std::unordered_set<std::tuple<size_t, size_t>> &seen) {

  size_t r = position[0];
  size_t c = position[1];
  seen.insert(std::tuple(r, c));

  if (grid[r][c] == '.' || grid[r][c] == '#') {
    return;
  }

  size_t n_r = position[0] + direction[0];
  size_t n_c = position[1] + direction[1];
  if (!seen.contains(std::tuple(n_r, n_c))) {
    std::array<size_t, 2> ahead_pos = {n_r, n_c};
    advance_movable(grid, ahead_pos, direction, seen);
  }

  std::array<size_t, 2> box_pair_pos = {};
  if (grid[r][c] == '[') {
    box_pair_pos = {r, c + 1};
  } else if (grid[r][c] == ']') {
    box_pair_pos = {r, c - 1};
  }
  if (!seen.contains(std::tuple(box_pair_pos[0], box_pair_pos[1]))) {
    advance_movable(grid, box_pair_pos, direction, seen);
  }

  grid[n_r][n_c] = grid[r][c];
  grid[r][c] = '.';
}

void advance_directions(std::vector<std::vector<char>> &grid,
                        const std::vector<char> &direction_symbols) {
  std::array<size_t, 2> position = get_position(grid);

  for (auto &direction_symbol : direction_symbols) {
    std::cout << direction_symbol << std::endl;
    std::array<int, 2> direction = direction_symbol_to_coords[direction_symbol];

    std::unordered_set<std::tuple<size_t, size_t>> seen = {};
    if (check_no_blocking_walls(grid, position, direction, seen)) {
      std::unordered_set<std::tuple<size_t, size_t>> seen = {};
      advance_movable(grid, position, direction, seen);
      position[0] += direction[0];
      position[1] += direction[1];
    }

    draw_grid(grid);
  }
}

size_t get_gps_score(std::vector<std::vector<char>> &grid) {
  size_t gps_score = 0;

  size_t n_rows = grid.size();
  size_t n_cols = grid[0].size();

  for (size_t r = 1; r < n_rows - 1; r++) {
    for (size_t c = 1; c < n_cols - 1; c++) {
      if (grid[r][c] == '[') {
        gps_score += r * 100 + c;
      }
    }
  }
  return gps_score;
}

int main() {
  std::ifstream input_file("data/day_15/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not open file.");
  }

  std::vector<std::vector<char>> grid = get_grid(input_file);
  draw_grid(grid);

  std::vector<char> direction_symbols = get_direction_symbols(input_file);

  advance_directions(grid, direction_symbols);

  size_t gps_score = get_gps_score(grid);

  std::cout << gps_score << std::endl;

  return 0;
}
