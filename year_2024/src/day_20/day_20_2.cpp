#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using Grid = std::vector<std::vector<char>>;
using Pos = std::array<int, 2>;

const size_t CHEAT_LENGTH = 20;

namespace std {
template <> struct hash<Pos> {
  size_t operator()(const Pos p) const { return p[0] ^ p[1] << 16; }
};
} // namespace std

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

void print_grid(const Grid &grid) {
  for (size_t r = 0; r < grid.size(); r++) {
    for (size_t c = 0; c < grid[0].size(); c++) {
      std::cout << grid[r][c];
    }
    std::cout << std::endl;
  }
}

Pos get_start_pos(const Grid &grid) {
  for (int r = 0; r < grid.size(); r++) {
    for (int c = 0; c < grid[0].size(); c++) {
      if (grid[r][c] == 'S') {
        return Pos{r, c};
      }
    }
  }
  throw std::runtime_error("Could not find starting position.");
}

Pos get_end_pos(const Grid &grid) {
  for (int r = 0; r < grid.size(); r++) {
    for (int c = 0; c < grid[0].size(); c++) {
      if (grid[r][c] == 'E') {
        return Pos{r, c};
      }
    }
  }
  throw std::runtime_error("Could not find starting position.");
}

const std::vector<Pos> DIRECTIONS = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

std::vector<Pos>
get_neighbours(const Grid &grid, const Pos &pos,
               const std::unordered_map<Pos, size_t> &distances_from_start) {
  std::vector<Pos> neighbours = {};

  for (const auto &direction : DIRECTIONS) {
    int n_r = pos[0] + direction[0];
    int n_c = pos[1] + direction[1];
    Pos next_pos = Pos({n_r, n_c});

    if ((n_r >= 0 && n_r < grid.size()) && (n_c >= 0 && n_c < grid[0].size()) &&
        (grid[n_r][n_c] != '#') && (!distances_from_start.contains(next_pos))) {
      neighbours.push_back(next_pos);
    }
  }
  return neighbours;
}

std::unordered_map<Pos, size_t> get_distances_from_start(const Grid &grid) {

  std::unordered_map<Pos, size_t> distances_from_start = {};

  Pos pos = get_start_pos(grid);

  std::queue<Pos> q = {};
  q.push(pos);

  size_t path_length = 0;
  while (q.size() > 0) {
    size_t q_length = q.size();
    for (size_t i = 0; i < q_length; i++) {
      pos = q.front();
      q.pop();

      // Mark position as visited.
      distances_from_start[pos] = path_length;

      for (const auto &next_pos :
           get_neighbours(grid, pos, distances_from_start)) {
        q.push(next_pos);
      }
    }
    ++path_length;
  }
  return distances_from_start;
}

int main() {
  // We are told that there is only a single route from start to end.
  // This means our BFS only ever has one member, and in fact our original
  // solution is more general than the problem demands.
  //
  // However, this one path constraint also means that once we have the full
  // path length we can determine the distance of each square from the end by
  // simply calculating distance_to_end = total_length - distnace_from_start.
  //
  // Importantly this means we don't have to recalculate the BFS for the
  // cheat_grid, instead look at the start and end point of the cheat grid and
  // the distance to teh end that this cuts out.
  //
  // This allows us to cut out a BFS compute factor of O(V+E)=O(V+4V) to make
  // room for the cheat jump search of O(V). Overall the time complexity will be
  // the same as our previous O(V^2) approach
  Grid grid = get_grid();
  std::cout << "Original Grid:" << std::endl;
  print_grid(grid);

  std::unordered_map<Pos, size_t> distances_from_start =
      get_distances_from_start(grid);

  Pos end_pos = get_end_pos(grid);
  size_t path_length_no_cheat = distances_from_start[end_pos];
  std::cout << "Path lenght without cheating: " << path_length_no_cheat
            << std::endl;

  std::vector<size_t> saved_path_length_counts = {};
  for (size_t i = 0; i <= path_length_no_cheat; i++) {
    saved_path_length_counts.push_back(0);
  }

  for (int r = 0; r < grid.size(); r++) {
    for (int c = 0; c < grid[0].size(); c++) {
      if (grid[r][c] != '#') {
        for (int n_r = 0; n_r < grid.size(); n_r++) {
          for (int n_c = 0; n_c < grid[0].size(); n_c++) {
            size_t jump_distance = std::abs(r - n_r) + std::abs(c - n_c);
            if ((grid[n_r][n_c] != '#') &&
                (jump_distance > 0 && jump_distance <= CHEAT_LENGTH)) {
              int saved_path_length = distances_from_start.at(Pos{n_r, n_c}) -
                                      distances_from_start.at(Pos{r, c}) -
                                      jump_distance;
              if (saved_path_length >= 0) {
                saved_path_length_counts[saved_path_length] += 1;
              }
            }
          }
        }
      }
    }
  }

  for (size_t i = 1; i <= path_length_no_cheat; i++) {
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
