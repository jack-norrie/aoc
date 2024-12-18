#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

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

void advance_directions(std::vector<std::vector<char>> &grid,
                        const std::vector<char> &direction_symbols) {
  std::array<size_t, 2> position = get_position(grid);

  for (auto &direction_symbol : direction_symbols) {
    std::cout << direction_symbol << std::endl;
    std::array<int, 2> direction = direction_symbol_to_coords[direction_symbol];

    if (check_no_blocking_walls(grid, position, direction)) {
      advance_movable(grid, position, direction);
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
      if (grid[r][c] == 'O') {
        gps_score += r * 100 + c;
      }
    }
  }
  return gps_score;
}

int main() {
  std::ifstream input_file("data/day_15/test_case_3");
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
