#include <array>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

std::tuple<std::vector<std::array<int, 2>>, std::vector<std::array<int, 2>>>
get_positions_and_velocicities() {
  std::ifstream input_file("year_2024/data/day_14/test_case_1");
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

int main() {
  std::tuple<std::vector<std::array<int, 2>>, std::vector<std::array<int, 2>>>
      particle_tuple = get_positions_and_velocicities();
  std::vector<std::array<int, 2>> positions = std::get<0>(particle_tuple);
  std::vector<std::array<int, 2>> velocities = std::get<1>(particle_tuple);

  std::array<size_t, 4> quadrants = {{0, 0, 0, 0}};

  size_t n = positions.size();
  size_t n_cols = 11;
  size_t n_rows = 7;
  for (size_t i = 0; i < n; i++) {
    int pos_x = positions[i][0];
    int pos_y = positions[i][1];

    int vel_x = velocities[i][0];
    int vel_y = velocities[i][1];

    std::cout << "Here" << std::endl;
    pos_x += vel_x * 100;
    pos_y += vel_y * 100;

    pos_x = remainder(pos_x, n_cols);
    pos_y = remainder(pos_y, n_rows);

    size_t quadrant = 0;

    bool xq1 = pos_x <= n_cols / 2 - 1;
    bool xq2 = pos_x >= (n_cols + 2 - 1) / 2;
    if (xq1 ^ xq2) {
      continue;
    } else if (xq1) {
      quadrant += 1;
    }

    bool yq1 = pos_y <= n_rows / 2 - 1;
    bool yq2 = pos_y >= (n_rows + 2 - 1) / 2;
    if (yq1 && yq2) {
      continue;
    } else if (yq1) {
      quadrant += 2;
    }

    quadrants[quadrant] += 1;

    std::cout << pos_x << std::endl;
    std::cout << pos_y << std::endl;
  }
}
