#include <algorithm>
#include <charconv>
#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

static int              parse_digit(char c);
static long             checksum(std::vector<int> const& disk);
static std::vector<int> compact(std::vector<int> const& disk);

int
main()
{
  std::string disk_summary;
  std::getline(std::cin, disk_summary);

  std::vector<int> original_disk;
  for (int i{0}; i < static_cast<int>(disk_summary.size()); ++i) {
    if (i % 2 == 0) {
      int const file_id{i / 2};
      original_disk.insert(original_disk.end(), parse_digit(disk_summary[i]), file_id);
    } else {
      original_disk.insert(original_disk.end(), parse_digit(disk_summary[i]), -1);
    }
  }

  std::vector<int> fragmented_disk{original_disk};
  auto writer{fragmented_disk.begin()};
  auto reader{fragmented_disk.end() - 1};

  while ((*writer) != -1) {
    ++writer;
  }
  while (reader != fragmented_disk.begin() && ((*reader) == -1)) {
    --reader;
  }
  do {
    *writer = *reader;
    *reader = -1;
    while ((*writer) != -1) {
      ++writer;
    }
    while (reader != fragmented_disk.begin() && ((*reader) == -1)) {
      --reader;
    }
  } while (std::distance(writer, reader) > 0);

  std::vector<int> compacted_disk{compact(original_disk)};

  std::cout << checksum(fragmented_disk) << ' '
            << checksum(compacted_disk) << '\n';

  return 0;
}

int
parse_digit(char c)
{
  return c - '0';
}

long
checksum(std::vector<int> const& disk)
{
  long result{0};
  for (int i{0}; i < static_cast<int>(disk.size()); ++i) {
    if (disk[i] != -1) {
      result += i * disk[i];
    }
  }
  return result;
}

std::vector<int>
compact(std::vector<int> const& disk)
{
  std::vector<int> result{disk};
  auto reader{result.rbegin()};
  while (reader != result.rend()) {
    reader = std::find_if_not(reader, result.rend(), [](int v){ return v == -1; });
    int             file_id{*reader};
    auto            file_end{std::find_if_not(reader, result.rend(), [file_id](int v){ return v == file_id; })};
    std::ptrdiff_t  file_size{std::distance(reader, file_end)};
    if (auto writer{std::search_n(result.begin(), reader.base() - 1, file_size, -1)}; writer != (reader.base() - 1)) {
      std::copy(reader, file_end, writer);
      std::fill(reader, file_end, -1);
    }
    reader = file_end;
  }
  return result;
}
