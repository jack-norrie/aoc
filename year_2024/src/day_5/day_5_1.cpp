#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

void update_dependencies(
    const std::unordered_map<size_t, std::vector<size_t>> &dependency_graph,
    std::set<size_t> &seen_dependencies, size_t dependee) {
  if (dependency_graph.contains(dependee)) {
    std::vector<size_t> direct_dependencies = dependency_graph.at(dependee);

    for (int i = 0; i < direct_dependencies.size(); i++) {
      size_t direct_dependency = direct_dependencies[i];
      if (!seen_dependencies.contains(direct_dependency)) {
        seen_dependencies.insert(direct_dependency);
      }
    }
  }
}

bool check_update_validity(
    const std::unordered_map<size_t, std::vector<size_t>> &dependency_graph,
    const std::vector<size_t> &topological_sort) {
  std::set<size_t> seen_dependencies;

  for (size_t dependee : topological_sort) {

    // None of the seen dependencies can occur after the dependees that
    // generated them.
    if (seen_dependencies.contains(dependee)) {
      return false;
    }

    update_dependencies(dependency_graph, seen_dependencies, dependee);
  }
  return true;
}

std::unordered_map<size_t, std::vector<size_t>>
build_dependency_graph(std::fstream &input_file) {
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
  return dependency_graph;
}

std::vector<size_t> get_topological_sort(std::string line) {
  std::vector<size_t> topological_sort;
  std::stringstream line_stream(line);
  std::string item;
  while (std::getline(line_stream, item, ',')) {
    topological_sort.push_back(std::stoi(item));
  }
  return topological_sort;
}

int main() {
  std::fstream input_file("year_2024/data/day_5/input");
  if (!input_file.is_open()) {
    std::cout << "Erorr: Could not open file." << std::endl;
    return 1;
  }

  std::unordered_map<size_t, std::vector<size_t>> dependency_graph =
      build_dependency_graph(input_file);

  int res = 0;
  std::string line;
  std::cout << "Invlaid Updates:" << std::endl;
  while (std::getline(input_file, line)) {
    std::vector<size_t> topological_sort = get_topological_sort(line);
    std::cout << line << std::endl;

    if (check_update_validity(dependency_graph, topological_sort)) {
      // int division is floor division, which for zero indexing returns mid
      // value.
      size_t middle_index = topological_sort.size() / 2;
      res += topological_sort[middle_index];
    }
  }
  std::cout << "Result:" << std::endl;
  std::cout << res << std::endl;

  return 0;
}
