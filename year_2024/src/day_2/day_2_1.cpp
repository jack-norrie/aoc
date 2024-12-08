#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

int main() {
  int res = 0;

  std::ifstream input_file("year_2024/data/day_2/input");

  if (!input_file.is_open()) {
    std::cerr << "Error: Could not open file" << std::endl;
  }

  std::string line;
  while (std::getline(input_file, line)) {
    bool safe = true;

    std::istringstream line_stream(line);

    int first_number;
    int second_number;
    if (line_stream >> first_number and line_stream >> second_number) {

      // Determine initial direction and wether initial delta is valid.
      int delta = second_number - first_number;
      int sign;
      if (delta >= 0) {
        sign = 1;
      } else {
        sign = -1;
      }
      if (delta * sign <= 0 or delta * sign > 3) {
        safe = false;
      }

      // Check remaining deltas have correct diretion and magnitude.
      int prev_number = second_number;
      int number;
      while (line_stream >> number and safe) {
        delta = number - prev_number;
        if (delta * sign <= 0 or delta * sign > 3) {
          safe = false;
        }
        prev_number = number;
      }
    }

    // Increment safe count if line is safe.
    if (safe) {
      res += 1;
    }
  }
  std::cout << res << std::endl;

  return 0;
}
