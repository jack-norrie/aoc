#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

using Pos = std::array<int, 2>;

const size_t DEPTH = 3;

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
  std::ifstream input_file("data/day_21/test_case_1");
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

size_t get_traversal_complexity(const int &d_r, const int &d_c,
                                const size_t &depth) {
  // This function recursively finds the number of button presses required at
  // lower levels to achieve some desired movement at a higher level.input_file

  // All non-trivial button expansions end with an 'A', this terminal 'A'
  // requires no further expansions since all lower level button pressing cycles
  // start at an 'A', so this terminal 'A' at any level is an identity operand
  // under this expansion, i.e. stays the same.
  size_t complexity = 0;

  // In the case of the trivial expansion, i.e. we are at the human level, we
  // can simply press the desired button and the complexity score for this
  // operation is yet again 1.
  if (depth == 0) {
    return 1;
  }

  // If we are not at the human level we now ask what needs to be done at the
  // next level down to cause the movement (d_r, d_c) at the current level.
  // * Move d_r up(-) or down(+) depending on sign
  // * Move d_c right(+) or left(-) depending on sign.
  // * Now the current level robot is pointing at the correct button, we now
  // need to hit the accept key on the next level to cause a button press.
  //
  // NOTE: The "gap" constraint is a red herring, all it does is determine
  // wether we need to move horizontally or vertically first, however the
  // resulting path length for the round trip back to 'A'  will be the same.
  Pos pos = KEYPAD_POS_MAP.at('A');
  if (d_r < 0) {
    Pos next_pos = KEYPAD_POS_MAP.at('^');
    complexity += get_traversal_complexity(next_pos[0] - pos[0],
                                           next_pos[1] - pos[1], depth - 1);
    complexity += -d_r - 1;
    pos = next_pos;
  } else if (d_r > 0) {
    Pos next_pos = KEYPAD_POS_MAP.at('v');
    complexity += get_traversal_complexity(next_pos[0] - pos[0],
                                           next_pos[1] - pos[1], depth - 1);
    complexity += d_r - 1;
    pos = next_pos;
  }

  if (d_c < 0) {
    Pos next_pos = KEYPAD_POS_MAP.at('<');
    complexity += get_traversal_complexity(next_pos[0] - pos[0],
                                           next_pos[1] - pos[1], depth - 1);
    complexity += -d_c - 1;
    pos = next_pos;
  } else if (d_c > 0) {
    Pos next_pos = KEYPAD_POS_MAP.at('>');
    complexity += get_traversal_complexity(next_pos[0] - pos[0],
                                           next_pos[1] - pos[1], depth - 1);
    complexity += d_c - 1;
    pos = next_pos;
  }

  Pos next_pos = KEYPAD_POS_MAP.at('A');
  complexity += get_traversal_complexity(next_pos[0] - pos[0],
                                         next_pos[1] - pos[1], depth - 1);

  return complexity;
}

size_t get_code_complexity(const std::string &code) {
  size_t complexity = 0;

  Pos pos = NUMPAD_POS_MAP.at('A');

  for (const auto &next_symbol : code) {
    Pos next_pos = NUMPAD_POS_MAP.at(next_symbol);
    complexity += get_traversal_complexity(next_pos[0] - pos[0],
                                           next_pos[1] - pos[1], DEPTH);
    pos = next_pos;
  }

  std::cout << code << " : " << complexity << std::endl;
  return complexity;
}

int main() {
  size_t complexity = 0;

  std::vector<std::string> codes = get_codes();
  for (const auto &code : codes) {
    complexity += get_code_complexity(code);
  }

  std::cout << complexity << std::endl;
  return 0;
}
