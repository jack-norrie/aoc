#include <algorithm>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>

std::array<double, 2> get_coordinates(std::ifstream &input_file,
                                      std::string &line) {
  std::getline(input_file, line);
  std::string button_a_string = line;

  std::regex button_pattern("X[+=](\\d+), Y[+=](\\d+)");
  std::smatch button_match = {};

  double x = {};
  double y = {};
  if (std::regex_search(button_a_string, button_match, button_pattern)) {
    x = std::stof(button_match[1]);
    y = std::stof(button_match[2]);
  } else {
    throw std::runtime_error("Could not match line to button A pattern");
  };

  return std::array<double, 2>{x, y};
}

std::array<double, 2>
matrix_mult(const std::array<std::array<double, 2>, 2> &mat,
            const std::array<double, 2> &vec) {
  return {mat[0][0] * vec[0] + mat[0][1] * vec[1],
          mat[1][0] * vec[0] + mat[1][1] * vec[1]};
}

bool int_close(const double &num) {
  return std::abs(num - std::round(num)) < 1e-4;
}

int main() {
  long res = 0;

  std::ifstream input_file("data/day_13/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not open hand.");
  }

  std::string line;
  while (input_file) {
    std::array<double, 2> a = get_coordinates(input_file, line);

    std::array<double, 2> b = get_coordinates(input_file, line);

    std::array<double, 2> p_o = get_coordinates(input_file, line);
    std::array<double, 2> p = {p_o[0] + 10000000000000,
                               p_o[1] + 10000000000000};
    // std::array<double, 2> p = p_o;

    std::getline(input_file, line);

    int det = a[0] * b[1] - a[1] * b[0];
    if (det == 0) {
      throw std::runtime_error("Cannot invert a singular matrix.");
    }
    std::array<std::array<double, 2>, 2> inv = {
        {{b[1] / det, -b[0] / det}, {-a[1] / det, a[0] / det}}};

    std::array<double, 2> presses = matrix_mult(inv, p);
    long a_press = std::round(presses[0]);
    long b_press = std::round(presses[1]);

    if ((a_press * a[0] + b_press * b[0] == p[0]) &&
        (a_press * a[1] + b_press * b[1] == p[1])) {
      res += 3 * a_press + b_press;
    }
  }

  std::cout << res << std::endl;

  return 0;
}
