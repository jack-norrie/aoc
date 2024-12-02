#include <cstdint>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

using MemoKey = uint64_t;
inline MemoKey make_key(int start, int prev, bool increasing, bool skipable) {
  return (static_cast<uint64_t>(start) << 32) |
         (static_cast<uint64_t>(prev) << 16) |
         (static_cast<uint64_t>(increasing) << 1) |
         static_cast<uint64_t>(skipable);
}

bool check_safe(int num1, int num2, bool increasing) {
  if (increasing && num2 < num1) {
    return false;
  } else if (!increasing && num2 > num1) {
    return false;
  }

  int delta = num2 - num1;
  uint delta_abs = std::abs(delta);
  return (delta_abs > 0 and delta_abs <= 3);
}

bool check_if_line_stream_is_safe(std::vector<int> numbers, int start, int prev,
                                  bool increasing, bool skipable,
                                  std::unordered_map<MemoKey, bool> memo) {
  if (start == numbers.size()) {
    return true;
  }

  MemoKey key = make_key(start, prev, increasing, skipable);
  auto it = memo.find(key);
  if (it != memo.end()) {
    return it->second;
  }

  // Option to skip current input (if available).
  bool skip_option = false;
  if (skipable) {
    skip_option = check_if_line_stream_is_safe(numbers, start + 1, prev,
                                               increasing, false, memo);
  }

  // Option to keep current input (if available).
  bool non_skip_option = false;
  if (prev == -1 || check_safe(numbers[prev], numbers[start], increasing)) {
    non_skip_option = check_if_line_stream_is_safe(numbers, start + 1, start,
                                                   increasing, skipable, memo);
  }

  bool result = skip_option || non_skip_option;
  memo[key] = result;
  return result;
}

int main() {
  // T(m, n) = O(nm) and M(m, n) = O(n).
  // - m: number of lines.
  // - n: number of numbers in a line.
  int res = 0;

  std::ifstream input_file("data/day_2/input");

  if (!input_file.is_open()) {
    std::cerr << "Error: Could not open file" << std::endl;
    return 1;
  }

  std::string line;
  while (std::getline(input_file, line)) {
    std::istringstream line_stream(line);
    std::vector<int> numbers;
    int tmp;
    while (line_stream >> tmp) {
      numbers.push_back(tmp);
    }

    // The memoisation is over:
    // - starting indexes (n).
    // - previous element index, which can either be i-1 or i-2 (2).
    // - Wether we are on the increasing path (2).
    // - Wether we have skips available (2).
    // This gives an upper bound for memoisation space complexity O(8n) = O(n),
    // and each memoised result is computed in constant time, this giving a
    // upper bound for time complexity O(n).
    std::unordered_map<MemoKey, bool> memo;
    bool increasing_option =
        check_if_line_stream_is_safe(numbers, 0, -1, true, true, memo);
    bool decreasing_option =
        check_if_line_stream_is_safe(numbers, 0, -1, false, true, memo);

    if (increasing_option || decreasing_option) {
      res += 1;
    }
  }
  std::cout << res << std::endl;
  return 0;
}
