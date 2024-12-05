#include "topological_sort.h"

#include <cstddef>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

static std::pair<int,int> read_edge(std::string const& s);


#include <algorithm>
int
main()
{
  std::vector<std::pair<int,int>> edges;

  std::string line;
  while (!std::getline(std::cin, line).eof() && !line.empty()) {
    edges.emplace_back(read_edge(line));
  }

  Graph graph{make_graph<int,decltype(edges)::const_iterator>(
      edges.begin(), edges.end())};
  std::unordered_set<int> update{12, 45};

  int result_valid{0};
  int result_invalid_fixed{0};
  while (!std::getline(std::cin, line).eof()) {
    std::istringstream is{line};
    std::vector<int> update;
    for (int i{}; is >> i;) {
      update.emplace_back(i);
      is.ignore(1, ',');
    }


    // This represents one valid ordering of all the pages.
    std::vector<int>        valid_order;
    std::unordered_set<int> page_subset{update.begin(), update.end()};
    topological_sort(graph, page_subset, std::back_inserter(valid_order));

    if (update == valid_order) {
      result_valid += update[update.size() / 2];
    } else {
      result_invalid_fixed += valid_order[update.size() / 2];
    }
  }

  std::cout << result_valid << ' ' << result_invalid_fixed << '\n';

  return 0;
}

std::pair<int,int> read_edge(std::string const& s)
{
  std::size_t sep{s.find_first_of('|')};
  int from{std::stoi(s.substr(0, sep))};
  int to{std::stoi(s.substr(sep + 1, s.size()))};
  return {from, to};
}
