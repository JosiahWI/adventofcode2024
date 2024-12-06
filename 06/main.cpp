#include <cstddef>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using area_t = std::vector<std::vector<char>>;

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

using delta_t = pos_t;

class Guard
{
public:
  Guard(pos_t position) : position{position} {}

  void
  forward()
  {
    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;
  }

  void
  back()
  {
    this->position.x -= this->velocity.x;
    this->position.y -= this->velocity.y;
  }

  void
  turn()
  {
    std::swap(this->velocity.x, this->velocity.y);
    this->velocity.x *= -1;
  }

  pos_t
  pos() const
  {
    return this->position;
  }

  delta_t
  dir() const
  {
    return this->velocity;
  }

  int
  x() const
  {
    return this->position.x;
  }

  int
  y() const
  {
    return this->position.y;
  }

private:
  pos_t   position;
  delta_t velocity{0, -1};
};

bool
operator<(Guard const& a, Guard const& b)
{
  return a.pos() < b.pos() || (a.pos() == b.pos() && a.dir() < b.dir());
}


static Guard find_guard(area_t const& area);
static bool in_bounds(area_t const& area, pos_t pos);
static int count_useful_blockades(area_t& area, std::set<pos_t> const& visited,
    pos_t start);
static bool ends_in_loop(area_t const& area, Guard guard);

int
main()
{
  area_t area;
  std::string line;
  while (!std::getline(std::cin, line).eof()) {
    std::vector<char> row{line.begin(), line.end()};
    area.emplace_back(std::move(row));
  }

  Guard   guard{find_guard(area)};
  // We need this later to make sure we ignore it for part 2.
  pos_t   start{guard.pos()};
  delta_t off{0, -1};
  
  std::set<pos_t> visited;

  do {
    visited.insert(guard.pos());
    guard.forward();
    if (in_bounds(area, guard.pos()) && area[guard.y()][guard.x()] == '#') {
        guard.back();
        guard.turn();
    }
  } while (in_bounds(area, guard.pos()));

  std::cout << visited.size() << ' '
            << count_useful_blockades(area, visited, start) << '\n';

  return 0;
}

Guard
find_guard(area_t const& area)
{
  for (std::size_t i{0}; i < area.size(); ++i) {
    auto const& row{area[i]};
    for (std::size_t j{0}; j < row.size(); ++j) {
      if (row[j] == '^') {
        return {{static_cast<int>(j), static_cast<int>(i)}};
      }
    }
  }
  throw std::runtime_error{"no guard"};
}

bool
in_bounds(area_t const& area, pos_t pos)
{
  return 0 <= pos.y && pos.y < static_cast<int>(area.size()) &&
         0 <= pos.x && pos.x < static_cast<int>(area[pos.y].size());
}

int
count_useful_blockades(area_t& area, std::set<pos_t> const& visited,
    pos_t start)
{
  int result{0};
  for (pos_t idea : visited) {
    if (start == idea) {
      continue;
    }

    area[idea.y][idea.x] = '#';
    if (ends_in_loop(area, Guard{start})) {
      ++result;
    }
    area[idea.y][idea.x] ='.';
  }
  return result;
}

bool
ends_in_loop(area_t const& area, Guard guard)
{
  std::set<Guard> visited_detailed;

  do {
    if (visited_detailed.count(guard)) {
      return true;
    }
    visited_detailed.insert(guard);
    guard.forward();
    while (in_bounds(area, guard.pos()) && area[guard.y()][guard.x()] == '#') {
        guard.back();
        guard.turn();
        guard.forward();
    }
  } while (in_bounds(area, guard.pos()));
  return false;
}
