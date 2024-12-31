#include <cstddef>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Trie {
public:
  std::unordered_map<char, Trie> neighbours = {};
  std::optional<std::string> word;

  Trie() {}

  void add_word(const std::string &word) {
    Trie *cur = this;
    for (const auto &c : word) {
      if (!cur->neighbours.contains(c)) {
        cur->neighbours[c] = Trie();
      }
      cur = &(cur->neighbours[c]);
    }
    cur->word = word;
  }

  const Trie *get_subtrie(const std::string &prefix) const {
    const Trie *cur = this;
    for (const auto &c : prefix) {
      if (!cur->neighbours.contains(c)) {
        return nullptr;
      }
      cur = &(cur->neighbours.at(c));
    }
    return cur;
  }
};

size_t count_words(const Trie &t) {
  size_t res = 0;
  if (t.word) {
    ++res;
  }

  for (const auto &it : t.neighbours) {
    Trie nt = it.second;
    res += count_words(nt);
  }

  return res;
}

size_t count_reachable_words_in_trie(const Trie &designs,
                                     const std::vector<std::string> &towels,
                                     std::unordered_map<size_t, size_t> &memo) {

  size_t id = reinterpret_cast<size_t>(&designs);
  if (memo.contains(id)) {
    return memo.at(id);
  }

  size_t res = 0;
  if (designs.word.has_value()) {
    ++res;
  }

  for (const auto &towel : towels) {
    const Trie *sub_designs = designs.get_subtrie(towel);
    if (sub_designs != nullptr) {
      res += count_reachable_words_in_trie(*sub_designs, towels, memo);
    }
  }

  memo[id] = res;
  return res;
};

int main() {
  // Naive approach:
  // treat the towels as decisions in a decision tree which is trying to
  // traverse the supplied designs. If there are n design, m towels and the max
  // design length is w, then this would lead to a O(nm^w) solution. This bound
  // is actualy verly loose. If we look more precisly each decision tree
  // actually one has w verticies and at most m edges per vertex. This brings
  // the time complexity down to O(nmw).
  //
  // Better approach:
  // We can collect all of the designs into a trie and traverse this trie using
  // the allowed towels. The trie will take O(nw) time to be constructed and
  // will have at most nw verticies. Each internal node will loop over m
  // possible towels, i.e. traversal edges. This will lead to a O(nw + nmw) =
  // O(nmw) solution overall. Although the time complexity is the same, the
  // benefit of this approach is that all designs are being searches
  // simultaneiously, so we end up reducing a lot of repeated computation when
  // it comes to words of the same prefix.

  std::ifstream input_file("data/day_19/input");
  std::string line = {};

  std::vector<std::string> towels = {};
  std::getline(input_file, line);
  std::stringstream towel_ss(line);
  std::string towel = {};
  while (std::getline(towel_ss, towel, ',')) {
    towel.erase(0, towel.find_first_not_of(' '));
    towels.push_back(towel);
  }

  // blank line
  std::getline(input_file, line);

  Trie designs = {};
  while (std::getline(input_file, line)) {
    designs.add_word(line);
  }
  std::cout << "Unique words added to trie: " << count_words(designs)
            << std::endl;

  std::unordered_map<size_t, size_t> memo = {};
  size_t reachable_word_count =
      count_reachable_words_in_trie(designs, towels, memo);
  std::cout << "Number of ways to make supplied desings: "
            << reachable_word_count << std::endl;

  return 0;
}
