#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <unordered_map>
#include <vector>

using Pos = std::array<int, 2>;

const size_t DEPTH = 26;

const std::array<std::array<char, 3>, 4> NUMPAD = {
    {{'7', '8', '9'}, {'4', '5', '6'}, {'1', '2', '3'}, {' ', '0', 'A'}}};
const std::array<std::array<char, 3>, 2> KEYPAD = {
    {{' ', '^', 'A'}, {'<', 'v', '>'}}};

template <size_t Rows, size_t Cols>
std::unordered_map<char, Pos>
get_symbol_to_pos_map(const std::array<std::array<char, Cols>, Rows> &grid) {
  std::unordered_map<char, Pos> pos_map = {};
  for (int r = 0; r < grid.size(); r++) {
    for (int c = 0; c < grid[0].size(); c++) {
      pos_map[grid[r][c]] = Pos{r, c};
    }
  }
  return pos_map;
}

const std::unordered_map<char, Pos> NUMPAD_POS_MAP =
    get_symbol_to_pos_map(NUMPAD);
const std::unordered_map<char, Pos> KEYPAD_POS_MAP =
    get_symbol_to_pos_map(KEYPAD);

std::vector<std::string> get_codes() {
  std::ifstream input_file("data/day_21/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("Could not open file.");
  }

  std::vector<std::string> codes = {};
  std::string line = {};
  while (std::getline(input_file, line)) {
    codes.push_back(line);
  }

  return codes;
}

using MemoKey = long;
inline MemoKey make_key(const int &r, const int &c, const int &d_r,
                        const int &d_c, const size_t &depth) {
  return (r) ^ (c << 8) ^ (d_r << 16) ^ (d_c << 24) ^ (depth << 32);
}

size_t request_movement_and_press(const int &r, const int &c, const int &d_r,
                                  const int &d_c, const size_t &depth,
                                  std::unordered_map<MemoKey, size_t> &memo);

size_t move_horizontally(Pos &pos, const int &d_c, const size_t &depth,
                         std::unordered_map<MemoKey, size_t> &memo) {
  size_t length = 0;
  if (d_c < 0) {
    Pos next_pos = KEYPAD_POS_MAP.at('<');
    length += request_movement_and_press(pos[0], pos[1], next_pos[0] - pos[0],
                                         next_pos[1] - pos[1], depth - 1, memo);
    pos = next_pos;
    length += (-d_c - 1) *
              request_movement_and_press(pos[0], pos[1], 0, 0, depth - 1, memo);
  } else if (d_c > 0) {
    Pos next_pos = KEYPAD_POS_MAP.at('>');
    length += request_movement_and_press(pos[0], pos[1], next_pos[0] - pos[0],
                                         next_pos[1] - pos[1], depth - 1, memo);
    pos = next_pos;
    length += (d_c - 1) *
              request_movement_and_press(pos[0], pos[1], 0, 0, depth - 1, memo);
  }
  return length;
}

size_t move_vertically(Pos &pos, const int &d_r, const size_t &depth,
                       std::unordered_map<MemoKey, size_t> &memo) {
  size_t length = 0;
  if (d_r > 0) {
    Pos next_pos = KEYPAD_POS_MAP.at('v');
    length += request_movement_and_press(pos[0], pos[1], next_pos[0] - pos[0],
                                         next_pos[1] - pos[1], depth - 1, memo);
    pos = next_pos;
    length += (d_r - 1) *
              request_movement_and_press(pos[0], pos[1], 0, 0, depth - 1, memo);
  } else if (d_r < 0) {
    Pos next_pos = KEYPAD_POS_MAP.at('^');
    length += request_movement_and_press(pos[0], pos[1], next_pos[0] - pos[0],
                                         next_pos[1] - pos[1], depth - 1, memo);
    pos = next_pos;
    length += (-d_r - 1) *
              request_movement_and_press(pos[0], pos[1], 0, 0, depth - 1, memo);
  }
  return length;
}

size_t press_button(Pos &pos, const size_t &depth,
                    std::unordered_map<MemoKey, size_t> &memo) {
  size_t length = 0;
  Pos next_pos = KEYPAD_POS_MAP.at('A');
  length += request_movement_and_press(pos[0], pos[1], next_pos[0] - pos[0],
                                       next_pos[1] - pos[1], depth - 1, memo);
  pos = next_pos;
  return length;
}

size_t request_movement_and_press(const int &r, const int &c, const int &d_r,
                                  const int &d_c, const size_t &depth,
                                  std::unordered_map<MemoKey, size_t> &memo) {

  MemoKey memo_key = make_key(r, c, d_r, d_c, depth);
  if (memo.contains(memo_key)) {
    return memo.at(memo_key);
  }

  if (depth == 0) {
    return 1;
  }

  Pos panic_pos = {};
  if (depth == DEPTH) {
    panic_pos = NUMPAD_POS_MAP.at(' ');
  } else {
    panic_pos = KEYPAD_POS_MAP.at(' ');
  }

  // At the lower level we want to chain same direction movements together, to
  // make this as effiicent as possible, we should try to move the largest
  // distance first, i.e. the following prioritisation: <, v, ^ = >.

  // Look into moving horizontal first.
  size_t length_horizontal_first = 0;
  if (Pos{r, c + d_c} != panic_pos) {
    Pos pos = KEYPAD_POS_MAP.at('A');
    length_horizontal_first += move_horizontally(pos, d_c, depth, memo);
    length_horizontal_first += move_vertically(pos, d_r, depth, memo);
    length_horizontal_first += press_button(pos, depth, memo);
  } else {
    length_horizontal_first = std::numeric_limits<size_t>::max();
  }
  size_t length_vertical_first = 0;
  if (Pos{r + d_r, c} != panic_pos) {
    Pos pos = KEYPAD_POS_MAP.at('A');
    length_vertical_first += move_vertically(pos, d_r, depth, memo);
    length_vertical_first += move_horizontally(pos, d_c, depth, memo);
    length_vertical_first += press_button(pos, depth, memo);
  } else {
    length_vertical_first = std::numeric_limits<size_t>::max();
  }

  size_t length = std::min(length_horizontal_first, length_vertical_first);

  memo[memo_key] = length;
  return length;
}

size_t get_code_complexity(const std::string &code,
                           std::unordered_map<MemoKey, size_t> &memo) {
  size_t code_value = std::stoi(code.substr(0, code.size() - 1));

  size_t code_length = 0;
  Pos pos = NUMPAD_POS_MAP.at('A');
  for (const auto &next_symbol : code) {
    Pos next_pos = NUMPAD_POS_MAP.at(next_symbol);
    code_length +=
        request_movement_and_press(pos[0], pos[1], next_pos[0] - pos[0],
                                   next_pos[1] - pos[1], DEPTH, memo);
    pos = next_pos;
  }
  std::cout << code << " : " << code_length << " : " << code_value << std::endl;

  return code_length * code_value;
}

int main() {
  size_t complexity = 0;

  std::vector<std::string> codes = get_codes();
  std::unordered_map<MemoKey, size_t> memo = {};
  for (const auto &code : codes) {
    complexity += get_code_complexity(code, memo);
  }

  std::cout << complexity << std::endl;
  return 0;
}
