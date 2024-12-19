#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

using Row = std::vector<char>;
using Grid = std::vector<Row>;

using Coord = std::array<int, 2>;
using State = std::tuple<Coord, Coord>;
using WeightedState = std::tuple<int, State>;

namespace std {
template <> struct hash<State> {
  size_t operator()(const State &s) const {
    Coord p = get<0>(s);
    int p_x = p[0];
    int p_y = p[1];
    Coord d = get<1>(s);
    int d_x = d[0];
    int d_y = d[1];
    return p_x ^ p_y << 1 ^ d_x << 2 ^ d_y << 3;
  }
};
} // namespace std

Grid get_grid() {
  std::ifstream input_file("data/day_16/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not opne file.");
  }
  std::string line;

  Grid grid = {};
  while (std::getline(input_file, line)) {
    Row row = {};
    for (char c : line) {
      row.push_back(c);
    }
    grid.push_back(row);
  }
  return grid;
}

Coord find_target(const Grid &grid, const char target) {
  size_t n_rows = grid.size();
  size_t n_cols = grid[0].size();

  for (int r = 0; r < n_rows; r++) {
    for (int c = 0; c < n_cols; c++) {
      if (grid[r][c] == target) {
        return Coord{r, c};
      }
    }
  }
  throw std::runtime_error("Could not find character in grid.");
}

Coord add_coords(const Coord &x, const Coord &y) {
  int c1 = x[0] + y[0];
  int c2 = x[1] + y[1];
  return Coord{c1, c2};
}

std::vector<WeightedState> get_transitions(const State &state) {
  std::vector<WeightedState> transitions = {};

  Coord cur_position = std::get<0>(state);
  Coord cur_direction = std::get<1>(state);

  // Move forward
  Coord forward_position = add_coords(cur_position, cur_direction);
  transitions.push_back(std::tuple(1, State{forward_position, cur_direction}));

  // Rotate 90 degrees
  Coord direction_90_1 = Coord{cur_direction[1], cur_direction[0]};
  transitions.push_back(std::tuple(1000, State{cur_position, direction_90_1}));
  Coord direction_90_2 = Coord{-cur_direction[1], -cur_direction[0]};
  transitions.push_back(std::tuple(1000, State{cur_position, direction_90_2}));

  // Rotate 180 degrees
  Coord direction_180 = Coord{-cur_direction[0], -cur_direction[1]};
  transitions.push_back(std::tuple(1000, State{cur_position, direction_180}));

  return transitions;
}

bool check_state_valid(const Grid &grid, const State &state) {
  Coord pos = std::get<0>(state);
  bool in_bounds = pos[0] >= 0 && pos[0] < grid.size() && pos[1] >= 0 &&
                   pos[1] < grid[0].size();
  bool non_wall = grid[pos[0]][pos[1]] != '#';
  return in_bounds && non_wall;
}

size_t get_score(const Grid &grid) {
  size_t score = 0;
  std::unordered_set<State> seen = {};
  std::priority_queue<WeightedState, std::vector<WeightedState>,
                      std::greater<WeightedState>>
      heap = {};

  // Find starting position and add east direction to  starting state.
  Coord start_pos = find_target(grid, 'S');
  State start_state = State{start_pos, Coord{0, 1}};
  heap.push(WeightedState{0, start_state});

  while (heap.size() > 0) {
    WeightedState cur_weighted_state = heap.top();
    heap.pop();

    size_t weight = std::get<0>(cur_weighted_state);
    State state = std::get<1>(cur_weighted_state);
    Coord pos = std::get<0>(state);
    if (grid[pos[0]][pos[1]] == 'E') {
      return weight;
    }
    seen.insert(state);

    for (const WeightedState &state_transition : get_transitions(state)) {
      size_t next_weight = weight + std::get<0>(state_transition);
      State next_state = std::get<1>(state_transition);
      if (check_state_valid(grid, next_state) && !seen.contains(next_state)) {
        heap.push(WeightedState{next_weight, next_state});
      }
    }
  }
  throw std::runtime_error("Could not find end.");
}

int main() {
  Grid grid = get_grid();

  size_t score = get_score(grid);

  std::cout << score << std::endl;

  return 0;
}
