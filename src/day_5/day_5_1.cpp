#include <cstddef>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main() {
  std::fstream input_file("data/day_5/test_case_1");

  std::unordered_map<size_t, std::vector<size_t>> dependency_graph;

  std::string line;
  std::getline(input_file, line);
  while (line != "") {
    size_t split_point = line.find('|');
    int dependency = std::stoi(line.substr(0, split_point));
    int dependable = std::stoi(line.substr(split_point + 1, line.size()));

    // Have the dependency point from dependee to dependency such that DFS can
    // be performed to get all dependencies of a given dependee.
    dependency_graph[dependable].push_back(dependency);

    std::getline(input_file, line);
  }

  while (std::getline(input_file, line)) {
    std::stringstream line_stream(line);
    std::string item;
    std::set<size_t> required_dependencies;

    while (std::getline(line_stream, item, ',')) {
      size_t dependee = std::stoi(item);

      // if (in required_dependencies) {
      //   return false
      // }
    }
  }
}
