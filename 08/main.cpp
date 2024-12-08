#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

struct pos_t
{
  int x;
  int y;
};

bool
operator<(pos_t const a, pos_t const b)
{
  return a.x < b.x || (a.x == b.x && a.y < b.y);
}

bool
operator==(pos_t const a, pos_t const b)
{
  return a.x == b.x && a.y == b.y;
}

namespace std {
  template<>
  struct hash<pos_t> {
    std::size_t operator()(pos_t const& p) const {
      std::size_t hash1{std::hash<int>{}(p.x)};
      std::size_t hash2{std::hash<int>{}(p.y)};
      return hash1 ^ (hash2 << 1);
    }
  };
} // end namespace std

static bool in_bounds(int width, int height, pos_t const& pos);

int
main()
{
  int width{0};
  int height{0};
  std::unordered_map<char, std::vector<pos_t>> antennas;
  std::string line;
  for (int i{0}; !std::getline(std::cin, line).eof(); ++i) {
    ++height;
    width = static_cast<int>(line.size());
    for (int j{0}; j < static_cast<int>(line.size()); ++j) {
      if (line[j] != '.') {
        if (auto it = antennas.find(line[j]); it == antennas.end()) {
          std::vector<pos_t> temp;
          temp.emplace_back(pos_t{j, i});
          antennas.emplace_hint(it, line[j], std::move(temp));
        } else {
          it->second.emplace_back(pos_t{j, i});
        }
      }
    }
  }

  std::unordered_set<pos_t> unique;
  std::unordered_set<pos_t> unique_two;
  for (auto const& [_, same_type_antennas] : antennas) {
    for (int i{0}; i < static_cast<int>(same_type_antennas.size()); ++i) {
      for (int j{i + 1}; j < static_cast<int>(same_type_antennas.size()); ++j) {
        auto const& pos_a{same_type_antennas[i]};
        auto const& pos_b{same_type_antennas[j]};
        unique_two.insert(pos_a);
        unique_two.insert(pos_b);
        int max_x{std::max(pos_a.x, pos_b.x)};
        int min_x{std::min(pos_a.x, pos_b.x)};
        int max_y{std::max(pos_a.y, pos_b.y)};
        int min_y{std::min(pos_a.y, pos_b.y)};
        int rise{pos_b.y - pos_a.y};
        int run{pos_b.x - pos_a.x};
        pos_t first_antinode{pos_a.x - run, pos_a.y - rise};
        if (in_bounds(width, height, first_antinode)) {
          unique.insert(first_antinode);
        }
        pos_t second_antinode{pos_b.x + run, pos_b.y + rise};
        if (in_bounds(width, height, second_antinode)) {
          unique.insert(second_antinode);
        }
        while (in_bounds(width, height, first_antinode)) {
          unique_two.insert(first_antinode);
          first_antinode.x -= run;
          first_antinode.y -= rise;
        }
        while (in_bounds(width, height, second_antinode)) {
          unique_two.insert(second_antinode);
          second_antinode.x += run;
          second_antinode.y += rise;
        }
      }
    }
  }

  std::cout << unique.size() << ' ' << unique_two.size() << '\n';

  return 0;
}

bool
in_bounds(int width, int height, pos_t const& pos)
{
  return 0 <= pos.x && 0 <= pos.y && pos.x < width && pos.y < height;
}
