#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<long long>
dense_to_sparse_disk_map(const std::string &dense_disk_map) {
  std::vector<long long> sparse_disk_map;

  long long id = 0;
  long long i = 0;
  while (i < dense_disk_map.size()) {
    // Add next file size.
    char file_size = dense_disk_map[i];
    for (size_t j = 0; j < file_size - '0'; j++) {
      sparse_disk_map.push_back(id);
    }
    i++;
    id++;

    // Add next empty size.
    char empty_size = dense_disk_map[i];
    if (i < dense_disk_map.size()) {
      for (size_t j = 0; j < empty_size - '0'; j++) {
        sparse_disk_map.push_back(-1);
      }
    }
    i++;
  }

  return sparse_disk_map;
}
std::vector<long long>
sparse_to_compact_disk_map(const std::vector<long long> &sparse_disk_map) {
  std::vector<long long> compact_disk_map = sparse_disk_map;

  long long l = 0;
  long long r = compact_disk_map.size() - 1;

  while (l < r) {
    // Find next space on left side.
    while (l < r && compact_disk_map[l] != -1) {
      l++;
    }

    // Find next non-space on right side.
    while (l < r && compact_disk_map[r] == -1) {
      r--;
    }

    long long tmp = compact_disk_map[l];
    compact_disk_map[l] = compact_disk_map[r];
    compact_disk_map[r] = tmp;
  }

  return compact_disk_map;
}

long long get_check_sum(const std::vector<long long> &compact_disk_map) {
  long long res = 0;

  for (long long i = 0; i < compact_disk_map.size(); i++) {
    if (compact_disk_map[i] == -1) {
      break;
    }
    res += i * compact_disk_map[i];
  }

  return res;
}

int main() {
  std::ifstream input_file("data/day_9/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Error: Could not open file.");
  }

  std::string dense_disk_map;
  std::getline(input_file, dense_disk_map);

  std::vector<long long> sparse_disk_map =
      dense_to_sparse_disk_map(dense_disk_map);

  std::vector<long long> compact_disk_map =
      sparse_to_compact_disk_map(sparse_disk_map);

  for (long long token : compact_disk_map) {
    std::cout << std::to_string(token) << " ";
  }
  std::cout << std::endl;

  long long check_sum = get_check_sum(compact_disk_map);
  std::cout << check_sum << std::endl;

  return 0;
}
