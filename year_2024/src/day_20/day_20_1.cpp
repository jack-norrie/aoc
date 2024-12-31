#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

using Grid = std::vector<std::vector<char>>;
using Pos = std::array<int, 2>;

Grid get_grid() {
  std::ifstream input_file("data/day_20/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not open file.");
  }

  Grid grid = {};
  std::string line = {};
  while (std::getline(input_file, line)) {
    std::vector<char> row = {};
    for (const auto &c : line) {
      row.push_back(c);
    }
    grid.push_back(row);
  }

  return grid;
}

Pos get_starting_pos(const Grid &grid) {
  for (int r = 0; r < grid.size(); r++) {
    for (int c = 0; c < grid[0].size(); c++) {
      if (grid[r][c] == 'S') {
        return Pos{r, c};
      }
    }
  }
  throw std::runtime_error("Could not find starting position.");
}

const std::vector<Pos> DIRECTIONS = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

std::vector<Pos> get_neighbours(const Grid &grid, const Pos &pos) {
  std::vector<Pos> neighbours = {};

  for (const auto &direction : DIRECTIONS) {
    int n_r = pos[0] + direction[0];
    int n_c = pos[1] + direction[1];

    if ((n_r >= 0 && n_r < grid.size()) && (n_c >= 0 && n_c < grid[0].size()) &&
        (grid[n_r][n_c] != '#')) {
      neighbours.push_back(Pos{n_r, n_c});
    }
  }
  return neighbours;
}

size_t get_shortest_path(Grid grid) {
  Pos pos = get_starting_pos(grid);

  std::queue<Pos> q = {};
  q.push(pos);

  size_t path_length = 0;
  while (q.size() > 0) {
    size_t q_length = q.size();
    for (size_t i = 0; i < q_length; i++) {
      pos = q.front();
      q.pop();

      if (grid[pos[0]][pos[1]] == 'E') {
        return path_length;
      }

      // Mark position as visited.
      grid[pos[0]][pos[1]] = '#';

      for (const auto &next_pos : get_neighbours(grid, pos)) {
        q.push(next_pos);
      }
    }
    ++path_length;
  }
  throw std::runtime_error("Could not find path.");
}

void print_grid(const Grid &grid) {
  for (size_t r = 0; r < grid.size(); r++) {
    for (size_t c = 0; c < grid[0].size(); c++) {
      std::cout << grid[r][c];
    }
    std::cout << std::endl;
  }
}

int main() {
  Grid grid = get_grid();
  print_grid(grid);

  size_t path_length_no_cheat = get_shortest_path(grid);
  std::cout << "Path lenght without cheating: " << path_length_no_cheat
            << std::endl;

  std::vector<size_t> saved_path_length_counts = {};
  for (size_t i = 0; i <= path_length_no_cheat; i++) {
    saved_path_length_counts.push_back(0);
  }

  for (size_t r = 0; r < grid.size(); r++) {
    for (size_t c = 0; c < grid[0].size(); c++) {
      Grid cheat_grid = grid; // Make a copy of the grid.
      if (cheat_grid[r][c] == '#') {
        cheat_grid[r][c] = '.';
        size_t path_length_cheat = get_shortest_path(cheat_grid);
        size_t saved_path_length = path_length_no_cheat - path_length_cheat;
        saved_path_length_counts[saved_path_length] += 1;
      }
    }
  }

  for (size_t i = 0; i <= path_length_no_cheat; i++) {
    if (saved_path_length_counts[i] > 0) {
      std::cout << "There are " << saved_path_length_counts[i]
                << " cheats that save " << i << " picoseconds." << std::endl;
    }
  }

  size_t res = 0;
  for (size_t i = 100; i <= path_length_no_cheat; i++) {
    res += saved_path_length_counts[i];
  }
  std::cout << "There are " << res
            << " cheats that save at least 100 picoseconds." << std::endl;
}
