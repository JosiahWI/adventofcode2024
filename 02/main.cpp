#include <algorithm>
#include <functional>
#include <iostream>
#include <atomic>
#include <iterator>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace {
  constexpr std::size_t n_workers{2};
} // end anonymous namespace

struct stats_t
{
  std::atomic<int> safe{};
  std::atomic<int> safe_dampened{};
};

static void worker(std::vector<std::string>::const_iterator start,
                   std::vector<std::string>::const_iterator end, stats_t& stats);
static void solve(std::string const& report, stats_t& stats);
static std::vector<int> parse_levels(std::string const& report);
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
  stats_t stats{0, 0};
  std::vector<std::string> reports;
  while (!std::cin.eof()) {
    std::getline(std::cin, reports.emplace_back());
  }

  // There is an empty report at the end of the vector from when we read
  // EOF, and it would count as safe, so we need to remove it.
  if (!reports.empty()) {
    reports.erase(reports.end() - 1);
  }

  std::vector<std::thread> workers;
  std::size_t i{0};
  std::size_t max_workload_size{reports.size() / n_workers};
  for (; (i + max_workload_size) < reports.size(); i += max_workload_size) {
    workers.emplace_back(std::thread{worker, reports.begin() + i,
                         reports.begin() + i + max_workload_size,
                         std::ref(stats)});
  }
  if (i < reports.size()) {
    workers.emplace_back(std::thread{worker, reports.begin() + i, reports.end(),
                         std::ref(stats)});
  }

  for (std::thread& t : workers) {
    t.join();
  }

  // These can be relaxed loads because the previous joins synchronize
  // the threads.
  std::cout << stats.safe.load(std::memory_order_relaxed) << ' '
            << stats.safe_dampened.load(std::memory_order_relaxed) << '\n';

  return 0;
}

void
worker(std::vector<std::string>::const_iterator start,
       std::vector<std::string>::const_iterator end, stats_t& stats)
{
  using namespace std::placeholders;
  std::for_each(start, end, std::bind(solve, _1, std::ref(stats)));
}

void
solve(std::string const& report, stats_t& stats)
{
  int local_safe{0};
  int local_safe_dampened{0};
  std::vector<int> levels{parse_levels(report)};
  if (is_safe(levels)) {
    ++local_safe;
  }
  if (is_safe_dampened(levels)) {
    ++local_safe_dampened;
  }
    stats.safe.fetch_add(local_safe, std::memory_order_relaxed);
    stats.safe_dampened.fetch_add(local_safe_dampened, std::memory_order_relaxed);
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
