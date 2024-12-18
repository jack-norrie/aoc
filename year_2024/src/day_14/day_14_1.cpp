#include <array>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

const size_t N_COLS = 101;
const size_t N_ROWS = 103;

std::tuple<std::vector<std::array<int, 2>>, std::vector<std::array<int, 2>>>
get_positions_and_velocicities() {
  std::ifstream input_file("data/day_14/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not open file.");
  }

  std::vector<std::array<int, 2>> positions = {};
  std::vector<std::array<int, 2>> velocities = {};

  std::regex pos_pattern("p=(.*),(.*) .+");
  std::regex vel_pattern(".+v=(.*),(.*)");

  std::string line = {};
  while (std::getline(input_file, line)) {

    std::smatch pos_match = {};
    if (std::regex_search(line, pos_match, pos_pattern)) {
      int x_pos = std::stoi(pos_match[1]);
      int y_pos = std::stoi(pos_match[2]);
      positions.push_back(std::array<int, 2>{{x_pos, y_pos}});
    }

    std::smatch vel_match = {};
    if (std::regex_search(line, vel_match, vel_pattern)) {
      int x_vel = std::stoi(vel_match[1]);
      int y_vel = std::stoi(vel_match[2]);
      velocities.push_back(std::array<int, 2>{{x_vel, y_vel}});
    }
  }
  return std::tuple(positions, velocities);
}

int remainder(int a, int b) {
  int trunc_div = a % b;
  int remainder = (trunc_div + b) % b;
  return remainder;
}

void progress_time(std::vector<std::array<int, 2>> &positions,
                   const std::vector<std::array<int, 2>> &velocities,
                   const size_t t) {
  size_t n = positions.size();
  for (size_t i = 0; i < n; i++) {
    int pos_x = positions[i][0];
    int pos_y = positions[i][1];

    int vel_x = velocities[i][0];
    int vel_y = velocities[i][1];

    pos_x += vel_x * t;
    pos_y += vel_y * t;

    pos_x = remainder(pos_x, N_COLS);
    pos_y = remainder(pos_y, N_ROWS);

    positions[i][0] = pos_x;
    positions[i][1] = pos_y;
  }
}

size_t entropy(const std::vector<std::array<int, 2>> &positions) {
  std::array<size_t, 4> quadrants = {{0, 0, 0, 0}};

  size_t n = positions.size();
  for (size_t i = 0; i < n; i++) {
    int pos_x = positions[i][0];
    int pos_y = positions[i][1];

    size_t quadrant = 0;

    bool xq1 = pos_x < N_COLS / 2;
    bool xq2 = pos_x > N_COLS / 2;
    if (!(xq1 ^ xq2)) {
      continue;
    } else if (xq2) {
      quadrant += 1;
    }

    bool yq1 = pos_y < N_ROWS / 2;
    bool yq2 = pos_y > N_ROWS / 2;
    if (!(yq1 ^ yq2)) {
      continue;
    } else if (yq2) {
      quadrant += 2;
    }

    quadrants[quadrant] += 1;
  }

  size_t entropy = 1;
  for (size_t q_count : quadrants) {
    entropy *= q_count;
    std::cout << q_count << std::endl;
  }

  return entropy;
}
int main() {
  std::tuple<std::vector<std::array<int, 2>>, std::vector<std::array<int, 2>>>
      particle_tuple = get_positions_and_velocicities();
  std::vector<std::array<int, 2>> positions = std::get<0>(particle_tuple);
  std::vector<std::array<int, 2>> velocities = std::get<1>(particle_tuple);

  progress_time(positions, velocities, 100);

  size_t entropy_eval = entropy(positions);

  std::cout << entropy_eval << std::endl;

  return 0;
}
