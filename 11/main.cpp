#include <array>
#include <cmath>
#include <deque>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <utility>

namespace {
  constexpr long step_size{5};
} // end anonymous namespace

static std::deque<long> simulate(long stone, long steps);
static long count_digits(long stone);
static bool is_even(long value);
static long left_side(long stone, long digits);
static long right_side(long stone, long digits);

class Solver {
public:
  void
  add(long stone, long count=1) {
    if (auto it{this->stones.find(stone)}; it == this->stones.end()) {
      this->stones.emplace_hint(it, stone, count);
    } else {
      ++it->second;
    }
  }

  void
  step(long times)
  {
    std::unordered_map<long, long> new_stones;
    for (auto& it : this->stones) {
      auto [stone,count]{it};
      if (count > 0) {
        for (long new_stone : simulate(it.first, times)) {
          if (auto new_it{new_stones.find(new_stone)}; new_it == new_stones.end()) {
            new_stones.emplace_hint(new_it, new_stone, count);
          } else {
            new_it->second += count;
          }
        }
      }
    }
    this->stones = std::move(new_stones);
  }

  long
  get_count() const
  {
    long result{0};
    for (auto const [_,count] : this->stones) {
      result += count;
    }
    return result;
  }

  void
  show() const
  {
    for (auto const [stone,count] : this->stones) {
      if (count > 0) {
        std::cout << stone << ":" << count << ' ';
      }
    }
    std::cout << '\n';
  }

private:
  std::unordered_map<long, long> stones;
};

std::deque<long>
simulate(long stone, long steps)
{
  std::deque<long> result;
  result.push_back(stone);
  for (long i{0}; i < steps; ++i) {
    long const stones{static_cast<long>(result.size())};
    for (long j{0}; j < stones; ++j) {
      long const next_stone = result.front();
      if (next_stone == 0) {
        result.push_back(1);
      } else if (long digits{count_digits(next_stone)}; is_even(digits)) {
        result.push_back(left_side(next_stone, digits));
        result.push_back(right_side(next_stone, digits));
      } else {
        result.push_back(next_stone * 2024);
      }
      result.pop_front();
    }
  }
  return result;
}

long
count_digits(long stone)
{
  long digits{0};
  do {
    ++digits;
    stone /= 10;
  } while (stone != 0);
  return digits;
}

bool
is_even(long value)
{
  return value % 2 == 0;
}

long
left_side(long stone, long digits)
{
  long result = stone;
  for (long i{0}; i < digits / 2; ++i) {
    result /= 10;
  }
  return result;
}

long
right_side(long stone, long digits)
{
  long result = 0;
  for (long i{0}; i < digits / 2; ++i) {
    result += std::pow(10, i) * (stone % 10);
    stone /= 10;
  }
  return result;
}

int
main()
{
  Solver s;
  while (true) {
    long stone;
    std::cin >> stone;
    if (std::cin.fail()) {
      break;
    } else {
      s.add(stone);
    }
  }

  long steps{75};
  while (steps > step_size) {
    s.step(step_size);
    steps -= step_size;
  }
  s.step(steps);

  std::cout << s.get_count() << '\n';
  return 0;
}

