#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

const size_t LOCK_HEIGHT = 5;

int main() {
  std::ifstream input_file("data/day_25/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not open file.");
  }

  std::vector<std::vector<std::string>> locks = {};
  std::vector<std::vector<std::string>> keys = {};

  std::string line = {};
  while (std::getline(input_file, line)) {
    bool is_lock = line[0] == '#';

    std::vector<std::string> block = {};
    // Lock
    for (size_t i = 0; i < LOCK_HEIGHT; i++) {
      std::getline(input_file, line);
      block.push_back(line);
    }

    if (is_lock) {
      locks.push_back(block);
    } else {
      keys.push_back(block);
    }

    std::getline(input_file, line); // bottom barrier
    std::getline(input_file, line); // gap
  }

  std::vector<std::vector<size_t>> lock_heights = {};
  for (const auto &lock : locks) {
    std::vector<size_t> lock_height = {};
    for (size_t c = 0; c < lock[0].size(); c++) {
      size_t h = 0;
      while ((h < lock.size()) && (lock[h][c] == '#')) {
        ++h;
      }
      lock_height.push_back(h);
    }
    lock_heights.push_back(lock_height);
  }

  std::vector<std::vector<size_t>> key_heights = {};
  for (const auto &key : keys) {
    std::vector<size_t> key_height = {};
    for (size_t c = 0; c < key[0].size(); c++) {
      size_t h = 0;
      while ((h < key.size()) && (key[h][c] == '.')) {
        ++h;
      }
      key_height.push_back(key.size() - h);
    }
    key_heights.push_back(key_height);
  }

  size_t res = 0;
  for (const auto &lock : lock_heights) {
    for (const auto &key : key_heights) {
      bool compatible = true;
      for (size_t c = 0; c < lock.size(); c++) {
        if (lock[c] + key[c] > LOCK_HEIGHT) {
          compatible = false;
          break;
        }
      }

      if (compatible) {
        ++res;
      }
    }
  }
  std::cout << res << std::endl;
}
