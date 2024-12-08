#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <tuple>
#include <vector>

bool check_in_bounds(const std::vector<std::vector<char>> grid, int r, int c) {

  // check row is in bounds.
  if (r < 0 || r >= grid.size()) {
    return false;
  }

  // check column is in bounds.
  if (c < 0 || c >= grid[0].size()) {
    return false;
  }

  return true;
}

bool check_target_match_in_direction(const std::vector<std::vector<char>> grid,
                                     std::tuple<int, int> pos,
                                     std::tuple<int, int> direction,
                                     std::string target) {

  int delta_r = std::get<0>(direction);
  int delta_c = std::get<1>(direction);

  int r = std::get<0>(pos) + delta_r;
  int c = std::get<1>(pos) + delta_c;

  for (int i = 0; i < target.length(); i++) {
    if (!check_in_bounds(grid, r, c) or grid[r][c] != target[i]) {
      return false;
    }
    r += delta_r;
    c += delta_c;
  }
  return true;
}

int main() {
  int res = 0;

  // Get file into 2d dynamic array.
  std::ifstream input_file("year_2024/data/day_4/input");
  std::vector<std::vector<char>> grid;
  std::string line;
  while (std::getline(input_file, line)) {
    std::vector<char> row;
    for (int i = 0; i < line.length(); i++) {
      row.push_back(line[i]);
    }
    grid.push_back(row);
  }

  // Define allowed movement directions.
  std::vector<std::tuple<int, int>> allowed_directions = {
      {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

  // Iteratre over each possible word starting posistion.
  int m = grid.size();
  int n = grid[0].size();
  for (int r = 0; r < m; r++) {
    for (int c = 0; c < n; c++) {

      // Check starting position.
      if (grid[r][c] == 'X') {
        std::tuple<int, int> pos = std::tuple(r, c);

        // Check each allowed direction.
        for (int i = 0; i < allowed_directions.size(); i++) {
          std::tuple<int, int> direction = allowed_directions[i];
          if (check_target_match_in_direction(grid, pos, direction, "MAS")) {
            res += 1;
          }
        }
      }
    }
  }
  std::cout << res << std::endl;
  return 0;
}
