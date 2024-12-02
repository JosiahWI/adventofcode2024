#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

std::vector<int> parse_levels(std::string const& report);
static bool is_safe(std::vector<int> const& levels);
static bool is_safe_asc(std::vector<int> const& levels);
static bool is_safe_pair_asc(int a, int b);
static bool is_safe_dsc(std::vector<int> const& levels);
static bool is_safe_pair_dsc(int a, int b);
static bool is_safe_dampened(std::vector<int>& levels);

template <typename ForwardIt, typename BinaryPred>
bool
all_adj(ForwardIt start, ForwardIt end, BinaryPred pred)
{
  return end == std::adjacent_find(start, end, pred);
}

int
main()
{
  int safe{0};
  int safe_dampened{0};
  while (!std::cin.eof()) {
    std::string report;
    std::getline(std::cin, report);
    if (!std::cin.eof()) {
      std::vector<int> levels{parse_levels(report)};
      if (is_safe(levels)) {
        ++safe;
      }
      if (is_safe_dampened(levels)) {
        ++safe_dampened;
      }
    }
  }

  std::cout << safe << ' ' << safe_dampened << '\n';

  return 0;
}

std::vector<int>
parse_levels(std::string const& report)
{
  std::istringstream levelstream{report};
  std::vector<int> levels{std::istream_iterator<int>{levelstream},
                          std::istream_iterator<int>{}};
  return levels;
}

bool
is_safe(std::vector<int> const& levels)
{
  return is_safe_asc(levels) || is_safe_dsc(levels);
}

bool
is_safe_asc(std::vector<int> const& levels)
{
  return all_adj(levels.begin(), levels.end(), std::not_fn(is_safe_pair_asc));
}

bool
is_safe_pair_asc(int a, int b)
{
  return (a < b) && ((b - a) >= 1) && ((b - a) <= 3);
}

bool
is_safe_dsc(std::vector<int> const& levels)
{
  return all_adj(levels.begin(), levels.end(), std::not_fn(is_safe_pair_dsc));
}

bool
is_safe_pair_dsc(int a, int b)
{
  return (b < a) && ((a - b) >= 1) && ((a - b) <= 3);
}

bool
is_safe_dampened(std::vector<int>& levels)
{
  // The case where levels is already safe is covered, because if levels is
  // safe, then the tail of levels must also be safe and we will find that
  // in the first iteration.
  for (std::size_t i{0}; i < levels.size(); ++i) {
    int temp       = levels[i];
    auto insert_at = levels.erase(levels.begin() + i);
    if (is_safe(levels)) {
      return true;
    }
    levels.insert(insert_at, temp);
  }
  return false;
}
