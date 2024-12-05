#include <algorithm>
#include <cstddef>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include <iostream>

template<typename E>
class Graph
{
public:

  using edges_t        = std::unordered_map<E,std::unordered_set<E>>;
  using const_iterator = edges_t::const_iterator;

  void
  insert(std::pair<E,E> const& edge)
  {
    auto const& [from, to]{edge};
    if (auto it{this->edges.find(from)}; it == this->edges.end()) {
      this->edges.insert({from, {to}});
    } else {
      it->second.insert(to);
    }
  }

  const_iterator
  begin() const
  {
    return this->edges.begin();
  }

  const_iterator
  end() const
  {
    return this->edges.end();
  }

  std::unordered_set<E> const&
  operator[](E const& key)
  {
    return this->edges[key];
  }

private:
  edges_t edges;
};

template<typename E, typename InputIt>
Graph<E>
make_graph(InputIt const start, InputIt const end)
{
  Graph<E> graph;
  std::for_each(start, end,
                [&graph](std::pair<E,E> const& edge){ graph.insert(edge); });
  return graph;
}

template <typename E, typename OutputIt>
void
topological_sort(Graph<E>& graph, std::unordered_set<E> const& vertex_subset,
                 OutputIt out)
{
  std::unordered_map<E, int> incident_edges;
  std::unordered_set<E>      destinations;
  std::unordered_set<E>      starting_points;

  for (auto const& from : vertex_subset) {
    for (auto const& to : graph[from]) {
      if (!vertex_subset.count(to)) {
        continue;
      }

      if (auto it{incident_edges.find(to)}; it == incident_edges.end()) {
          incident_edges.insert({to, 1});
        } else {
          ++it->second;
      }

      destinations.insert(to);
      if (auto it = starting_points.find(to); it != starting_points.end()) {
        starting_points.erase(it);
      }
    }
    if (0 == destinations.count(from)) {
      starting_points.insert(from);
    }
  }

  std::size_t total_vertices{starting_points.size() + destinations.size()};
  std::size_t written{0};
  std::queue<E> open_vertices;
  for (E const& open : starting_points) {
    open_vertices.push(open);
  }
  while (!open_vertices.empty()) {
    // We pop the element at the end of the iteration.
    E const& next = open_vertices.front();
    *(out++) = next;
    ++written;
    for (auto const& to : graph[next]) {
      if (!vertex_subset.count(to)) {
        continue;
      }

      auto it{incident_edges.find(to)};
      if (it->second == 1) {
        open_vertices.push(to);
      }
      --it->second;
    }
    open_vertices.pop();
  }

  if (total_vertices != written) {
    std::cerr << "read " << total_vertices
              << " but wrote " << written << '\n';
    throw std::runtime_error{"topological sort on cyclical graph!"};
  }
}
