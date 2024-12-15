#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <regex>
#include <stdexcept>
#include <string>
#include <tuple>

std::tuple<size_t, size_t> get_coordinates(std::ifstream &input_file,
                                           std::string &line) {
  std::getline(input_file, line);
  std::string button_a_string = line;

  std::regex button_pattern("X[+=](\\d+), Y[+=](\\d+)");
  std::smatch button_match = {};

  size_t x = {};
  size_t y = {};
  if (std::regex_search(button_a_string, button_match, button_pattern)) {
    x = std::stoi(button_match[1]);
    y = std::stoi(button_match[2]);
  } else {
    throw std::runtime_error("Could not match line to button A pattern");
  };

  return std::tuple(x, y);
}

int main() {
  size_t res = 0;

  std::ifstream input_file("year_2024/data/day_13/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not open hand.");
  }

  std::string line;
  while (input_file) {
    std::tuple<size_t, size_t> button_a_direction =
        get_coordinates(input_file, line);
    size_t a_x = std::get<0>(button_a_direction);
    size_t a_y = std::get<1>(button_a_direction);

    std::tuple<size_t, size_t> button_b_direction =
        get_coordinates(input_file, line);
    size_t b_x = std::get<0>(button_b_direction);
    size_t b_y = std::get<1>(button_b_direction);

    std::tuple<size_t, size_t> prize_pos = get_coordinates(input_file, line);
    size_t p_x = std::get<0>(prize_pos);
    size_t p_y = std::get<1>(prize_pos);

    std::getline(input_file, line);

    size_t cost = std::numeric_limits<size_t>::max();
    for (size_t i = 0; i <= 100; i++) {
      for (size_t j = 0; j <= 100; j++) {
        if ((i * a_x + j * b_x == p_x) && (i * a_y + j * b_y == p_y)) {
          cost = std::min(cost, 3 * i + j);
        }
      }
    }

    if (cost < std::numeric_limits<size_t>::max()) {
      res += cost;
    }
  }

  std::cout << res << std::endl;

  return 0;
}
