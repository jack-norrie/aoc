#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

const size_t DEPTH = 3;

using AdjList =
    std::unordered_map<std::string, std::unordered_set<std::string>>;

void get_cliques(const size_t i, const std::vector<std::string> &verticies,
                 const AdjList &adj_list, std::vector<std::string> &clique,
                 std::vector<std::vector<std::string>> &cliques) {
  if (clique.size() == DEPTH) {
    auto clique_copy = clique;
    cliques.push_back(clique_copy);
    return;
  }

  for (size_t j = i; j < verticies.size(); j++) {
    std::string v = verticies[j];
    auto neighbours = adj_list.at(v);

    // Check if v can be added to clique
    // - i.e. is connected to all of current clique.
    bool fully_connected = true;
    for (const auto &u : clique) {
      if (!neighbours.contains(u)) {
        fully_connected = false;
        break;
      }
    }

    if (fully_connected) {
      clique.push_back(v);
      get_cliques(j + 1, verticies, adj_list, clique, cliques);
      clique.pop_back();
    }
  }
}

int main() {
  std::ifstream input_file("data/day_23/input");
  if (!input_file.is_open()) {
    throw std::runtime_error("could not open file.");
  }

  std::unordered_map<std::string, std::unordered_set<std::string>> adj_list =
      {};
  std::string line = {};
  while (std::getline(input_file, line)) {
    // Split verticies associated with edge.
    size_t split_pos = line.find('-');
    std::string u = line.substr(0, split_pos);
    std::string v = line.substr(split_pos + 1, line.size() - split_pos - 1);

    // The edge is undirected, so add it in both directions.
    adj_list[u].insert(v);
    adj_list[v].insert(u);
  }

  std::vector<std::string> verticies = {};
  for (const auto &it : adj_list) {
    verticies.push_back(it.first);
  }

  std::vector<std::vector<std::string>> cliques = {};
  std::vector<std::string> clique = {};
  get_cliques(0, verticies, adj_list, clique, cliques);

  std::cout << "Cliques" << std::endl;
  for (const auto &clique : cliques) {
    for (size_t i = 0; i < clique.size(); i++) {
      std::cout << clique[i];
      if (i < clique.size() - 1) {
        std::cout << ',';
      }
    }
    std::cout << std::endl;
  }

  std::vector<std::vector<std::string>> t_cliques = {};
  for (const auto &clique : cliques) {
    bool t_starter = false;
    for (const auto &v : clique) {
      if (v[0] == 't') {
        t_starter = true;
        break;
      }
    }
    if (t_starter) {
      t_cliques.push_back(clique);
    }
  }

  std::cout << "T Starting Cliques" << std::endl;
  for (const auto &clique : t_cliques) {
    for (size_t i = 0; i < clique.size(); i++) {
      std::cout << clique[i];
      if (i < clique.size() - 1) {
        std::cout << ',';
      }
    }
    std::cout << std::endl;
  }
  std::cout << "Numer of T Starting Cliques: " << t_cliques.size() << std::endl;

  return 0;
}
