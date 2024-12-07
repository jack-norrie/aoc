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
    return hash<int>()(get<0>(t)) ^ (hash<int>()(get<1>(t)) << 1);
  }
};
} // namespace std

bool check_in_bounds(const std::vector<std::vector<char>> &grid, const int &r,
                     const int &c) {
  size_t n_rows = grid.size();
  size_t n_cols = grid[0].size();

  if (r < 0 || r >= n_rows) {
    return false;
  }

  if (c < 0 || c >= n_cols) {
    return false;
  }

  return true;
}

std::tuple<int, int>
get_starting_position(const std::vector<std::vector<char>> &grid) {
  std::unordered_set<char> starting_pos_symbols = {'^', '>', 'v', '^'};

  for (size_t r = 0; r < grid.size(); r++) {
    for (size_t c = 0; c < grid.size(); c++) {
      if (starting_pos_symbols.contains(grid[r][c])) {
        return std::tuple(r, c);
      }
    }
  }

  throw std::runtime_error("No starting position found in supplied grid.");
}

std::vector<std::vector<char>> get_grid() {
  std::ifstream input_file("data/day_6/input");
  if (!input_file.is_open()) {
    std::cout << "Error: Could not open file";
  }

  std::vector<std::vector<char>> grid;
  std::string line;
  while (getline(input_file, line)) {
    std::vector<char> row;
    for (char c : line) {
      row.push_back(c);
      std::cout << c;
    }
    std::cout << std::endl;
    grid.push_back(row);
  }

  return grid;
}

int main() {

  std::vector<std::vector<char>> grid = get_grid();

  std::tuple<int, int> starting_pos = get_starting_position(grid);
  int r = std::get<0>(starting_pos);
  int c = std::get<1>(starting_pos);
  char state = grid[r][c];

  std::unordered_map<char, std::tuple<int, int>> state_to_direction{
      {'^', {-1, 0}}, {'>', {0, 1}}, {'v', {1, 0}}, {'<', {0, -1}}};

  std::unordered_map<char, char> state_transitions = {
      {'^', '>'}, {'>', 'v'}, {'v', '<'}, {'<', '^'}};

  std::unordered_set<std::tuple<int, int>> visited_positions;

  while (check_in_bounds(grid, r, c)) {
    visited_positions.insert(std::tuple(r, c));
    std::cout << r << "-" << c << std::endl;

    std::tuple<int, int> direction = state_to_direction[state];
    int d_r = std::get<0>(direction);
    int d_c = std::get<1>(direction);

    int n_r = r + d_r;
    int n_c = c + d_c;

    // If not in bounds then update and allow while loop to end.
    if (!check_in_bounds(grid, n_r, n_c) || grid[n_r][n_c] != '#') {
      r = n_r;
      c = n_c;
    } else {
      state = state_transitions[state];
    }
  }

  std::cout << "Number of positions visited: " << visited_positions.size()
            << std::endl;

  return 0;
}
