#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>

const std::string INPUT_FILE = "data/day_18/input";
const std::size_t N_ROWS = 71;
const std::size_t N_COLS = 71;
const std::size_t N_FALL = 1024;

using Coord = std::array<int, 2>;
using Grid = std::array<std::array<char, N_COLS>, N_ROWS>;

const std::vector<Coord> DIRECTIONS = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

std::stack<Coord> load_corrupted_addresses() {
  std::ifstream input_file(INPUT_FILE);
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not open input file.");
  }

  std::stack<Coord> locations = {};

  std::string line = {};
  while (std::getline(input_file, line)) {
    std::stringstream ss(line);
    std::string num_string = {};
    Coord location = {};
    for (size_t i = 0; i < 2; i++) {
      std::getline(ss, num_string, ',');
      std::size_t coord = std::stoi(num_string);
      location[1 - i] = coord;
    }
    locations.push(location);
  }

  std::stack<Coord> locations_reversed = {};
  while (locations.size() > 0) {
    locations_reversed.push(locations.top());
    locations.pop();
  }

  return locations_reversed;
}

Grid generate_grid() {
  Grid grid = {};
  for (int r = 0; r < N_ROWS; r++) {
    for (int c = 0; c < N_COLS; c++) {
      grid[r][c] = '.';
    }
  }
  return grid;
}

void simulate_falling(Grid &grid, std::stack<Coord> &corrupted_addresses) {
  for (size_t i = 0; i < N_FALL; i++) {
    Coord location = corrupted_addresses.top();
    corrupted_addresses.pop();
    grid[location[0]][location[1]] = '#';
  }
}

void print_grid(const Grid &grid) {
  for (int r = 0; r < N_ROWS; r++) {
    for (int c = 0; c < N_COLS; c++) {
      std::cout << grid[r][c];
    }
    std::cout << std::endl;
  }
}

std::vector<Coord> get_neighbours(const Grid &grid, const Coord &pos) {
  std::vector<Coord> neighbours = {};

  int r = pos[0];
  int c = pos[1];

  for (const auto &direction : DIRECTIONS) {
    int d_r = direction[0];
    int d_c = direction[1];

    int n_r = r + d_r;
    int n_c = c + d_c;

    if (n_r >= 0 && n_r < N_ROWS && n_c >= 0 && n_c < N_COLS &&
        grid[n_r][n_c] != '#') {
      neighbours.push_back({n_r, n_c});
    }
  }
  return neighbours;
}

size_t find_shortest_path_length(Grid &grid) {
  Coord pos = {0, 0};
  Coord end_pos = {N_ROWS - 1, N_COLS - 1};
  std::queue<Coord> q{{pos}};

  size_t path_length = 0;
  while (q.size() > 0) {
    size_t level_size = q.size();
    for (size_t i = 0; i < level_size; i++) {
      pos = q.front();
      q.pop();

      if (pos == end_pos) {
        return path_length;
      }

      // Make this location in accessible so that it is not re-traversed
      if (grid[pos[0]][pos[1]] == '#') {
        continue;
      }
      grid[pos[0]][pos[1]] = '#';

      for (const auto &n_pos : get_neighbours(grid, pos)) {
        q.push(n_pos);
      }
    }
    ++path_length;
  }
  throw std::runtime_error("No path to end found.");
}

int main() {
  std::stack<Coord> corrupted_addresses = load_corrupted_addresses();

  Grid grid = generate_grid();

  simulate_falling(grid, corrupted_addresses);

  print_grid(grid);

  size_t shortest_path_length = find_shortest_path_length(grid);
  std::cout << shortest_path_length << std::endl;

  return 0;
}
