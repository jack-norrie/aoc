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

bool safe_check_match(const std::vector<std::vector<char>> grid, int r, int c,
                      char t) {
  return check_in_bounds(grid, r, c) && grid[r][c] == t;
}

bool check_ms_cross(const std::vector<std::vector<char>> grid,
                    std::tuple<int, int> pos) {
  int r = std::get<0>(pos);
  int c = std::get<1>(pos);

  bool pos_diag_forward = safe_check_match(grid, r - 1, c + 1, 'S') &&
                          safe_check_match(grid, r + 1, c - 1, 'M');
  bool pos_diag_reverse = safe_check_match(grid, r - 1, c + 1, 'M') &&
                          safe_check_match(grid, r + 1, c - 1, 'S');
  bool pos_diag = pos_diag_forward || pos_diag_reverse;

  bool neg_diag_forward = safe_check_match(grid, r - 1, c - 1, 'M') &&
                          safe_check_match(grid, r + 1, c + 1, 'S');
  bool neg_diag_reverse = safe_check_match(grid, r - 1, c - 1, 'S') &&
                          safe_check_match(grid, r + 1, c + 1, 'M');
  bool neg_diag = neg_diag_forward || neg_diag_reverse;

  return pos_diag && neg_diag;
}

int main() {
  int res = 0;

  // Get file into 2d dynamic array.
  std::ifstream input_file("data/day_4/input");
  std::vector<std::vector<char>> grid;
  std::string line;
  while (std::getline(input_file, line)) {
    std::vector<char> row;
    for (int i = 0; i < line.length(); i++) {
      row.push_back(line[i]);
    }
    grid.push_back(row);
  }

  // Iteratre over each possible word starting posistion.
  int m = grid.size();
  int n = grid[0].size();
  for (int r = 0; r < m; r++) {
    for (int c = 0; c < n; c++) {

      // Check starting position.
      if (grid[r][c] == 'A') {
        std::tuple<int, int> pos = std::tuple(r, c);

        // Check each allowed direction.
        if (check_ms_cross(grid, pos)) {
          res += 1;
        }
      }
    }
  }
  std::cout << res << std::endl;
  return 0;
}
