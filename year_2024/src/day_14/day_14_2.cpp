#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <thread>
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
  }

  return entropy;
}

void draw_board(const std::vector<std::array<int, 2>> positions) {
  std::array<std::array<char, N_COLS>, N_ROWS> board = {};
  for (auto &row : board) {
    row.fill('.');
  }

  size_t n = positions.size();
  for (size_t i = 0; i < n; i++) {
    int pos_x = positions[i][0];
    int pos_y = positions[i][1];
    board[pos_y][pos_x] = '*';
  }

  for (size_t i = 0; i < N_COLS; i++) {

    std::cout << '#';
  }
  std::cout << std::endl;

  for (auto &row : board) {
    for (char &c : row) {
      std::cout << c;
    }
    std::cout << std::endl;
  }

  for (size_t i = 0; i < N_COLS; i++) {

    std::cout << '#';
  }
  std::cout << std::endl;
}

int main() {
  std::tuple<std::vector<std::array<int, 2>>, std::vector<std::array<int, 2>>>
      particle_tuple = get_positions_and_velocicities();
  std::vector<std::array<int, 2>> positions = std::get<0>(particle_tuple);
  std::vector<std::array<int, 2>> velocities = std::get<1>(particle_tuple);

  // Sample the entropies of the first num_samples evolutions.
  // Then get the 0.1% quantile.
  const std::size_t num_samples = 10000;
  std::array<size_t, num_samples> entropy_samples = {};
  std::vector<std::array<int, 2>> positions_copy = positions;
  for (size_t i = 0; i < num_samples; i++) {
    progress_time(positions_copy, velocities, 1);
    entropy_samples[i] = entropy(positions_copy);
  }
  std::sort(entropy_samples.begin(), entropy_samples.end());
  size_t low_entropy = entropy_samples[num_samples / 1000];

  size_t t = 0;
  while (true) {
    progress_time(positions, velocities, 1);
    ++t;

    // The christmas tree is an ordered structure, it will have a low entropy.
    // To speed up our search only draw low entropy boards.
    size_t entropy_eval = entropy(positions);
    if (entropy_eval < low_entropy) {
      draw_board(positions);
      std::this_thread::sleep_for(std::chrono::seconds(1));
      std::cout << "The timestamp for the above board state is: " << t
                << std::endl;
    }
  }

  return 0;
}
