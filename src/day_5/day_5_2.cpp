#include <array>
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

void add_minimal_depedencies_dfs(
    const std::unordered_map<size_t, std::vector<size_t>> &dependency_graph,
    std::unordered_set<size_t> &seen_dependencies, size_t &dependee,
    std::vector<size_t> &topological_sort) {
  if (!seen_dependencies.contains(dependee)) {
    seen_dependencies.insert(dependee);

    // Before adding this dependee to the final topological_sort
    // See if the graph subset contains any dependencies that have to go before
    // it. Add these first by processing in a post-order dfs traversal.
    if (dependency_graph.contains(dependee)) {
      std::vector<size_t> direct_dependencies = dependency_graph.at(dependee);

      for (int i = 0; i < direct_dependencies.size(); i++) {
        size_t direct_dependency = direct_dependencies[i];
        if (!seen_dependencies.contains(direct_dependency) &&
            dependency_graph.contains(direct_dependency)) {
          add_minimal_depedencies_dfs(dependency_graph, seen_dependencies,
                                      direct_dependency, topological_sort);
        }
      }
    }
    topological_sort.push_back(dependee);
  }
}

std::vector<size_t> fix_topological_sort(
    const std::unordered_map<size_t, std::vector<size_t>> &dependency_graph,
    const std::vector<size_t> &topological_sort) {
  // The depedency that we are modelling is only over a subset of the graph
  std::unordered_map<size_t, std::vector<size_t>> dependency_graph_subset;
  for (size_t dependee : topological_sort) {
    if (dependency_graph.contains(dependee)) {
      dependency_graph_subset[dependee] = dependency_graph.at(dependee);
    }
  }

  std::vector<size_t> correct_topological_sort;
  std::unordered_set<size_t> seen_dependencies;
  for (size_t dependee : topological_sort) {
    add_minimal_depedencies_dfs(dependency_graph_subset, seen_dependencies,
                                dependee, correct_topological_sort);
  }
  return correct_topological_sort;
}
void update_dependencies(
    const std::unordered_map<size_t, std::vector<size_t>> &dependency_graph,
    std::unordered_set<size_t> &seen_dependencies, size_t &dependee) {
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
  std::unordered_set<size_t> seen_dependencies;

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
  // T(V, E, q)  = O(qE)
  // M(V, E, q)  = O(E)
  // - V: number of verticies in the depdency graph, i.e. number of rules.
  // - E: number of edgess in the depdency graph, i.e. number of pages.
  // - q: number of update queries, i.e. number of updates.
  std::fstream input_file("data/day_5/input");
  if (!input_file.is_open()) {
    std::cout << "Erorr: Could not open file." << std::endl;
    return 1;
  }

  // O(V+E) = O(E)
  std::unordered_map<size_t, std::vector<size_t>> dependency_graph =
      build_dependency_graph(input_file);

  int res = 0;
  std::string line;
  while (std::getline(input_file, line)) {
    std::vector<size_t> topological_sort = get_topological_sort(line);

    // O(V + E) = O(E)
    if (!check_update_validity(dependency_graph, topological_sort)) {
      std::cout << "Incorrect Update: ";
      for (size_t num : topological_sort) {
        std::cout << num << " ";
      }
      std::cout << std::endl;

      // O(V + E) = O(E)
      topological_sort =
          fix_topological_sort(dependency_graph, topological_sort);

      std::cout << "Corrected Update: ";
      for (size_t num : topological_sort) {
        std::cout << num << " ";
      }
      std::cout << std::endl;

      size_t middle_index = topological_sort.size() / 2;
      res += topological_sort[middle_index];
    }
  }
  std::cout << "Result:" << std::endl;
  std::cout << res << std::endl;

  return 0;
}
