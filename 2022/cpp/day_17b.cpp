#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Using just the relative height of each column to the floor is not enough to remove false positives
// even when adding in jet index and rock type to the == comparator.
// 5 rows of the chamber is arbitrary, might be able to make do with less.

// TODO: use bitset instead of array<char>
// though array<bool> might be faster than bitset

struct Point {
  Point(const std::size_t row, const int col) : row (row), col(col) {}
  std::size_t row;
  int col;
};

constexpr size_t n_rows_in_hist = 5;
constexpr size_t n_cols_in_chamber = 7;
using Chamber = std::array<std::array<char, n_cols_in_chamber>, n_rows_in_hist>;
using Rock = std::vector<Point>;

struct Memory {
  Memory(const Chamber& rel_top_n, const int jet_index, const int rock_type, const std::size_t highest)
    : rel_top_n(rel_top_n), jet_index(jet_index), rock_type(rock_type), highest(highest)
  {}
  Chamber rel_top_n;
  int jet_index;
  int rock_type;
  std::size_t highest;
  bool operator == (const Memory& m) const {
    return m.jet_index == jet_index && m.rock_type == rock_type &&
    m.rel_top_n == rel_top_n;
  }
};

struct MemoryHash {
  std::size_t operator () (const Memory& m) const {
    std::size_t ans = 1;
    for (std::size_t row = 0; row < m.rel_top_n.size(); row++ ) {
      for (std::size_t i = 0; i < m.rel_top_n[row].size(); i++) {
        ans = ans + (i+1) * m.rel_top_n[row][i];
      }
    }
    ans = ans + m.jet_index + m.rock_type;
    return ans;
  }
};

void move_to_starting_height(Rock& rock, const std::size_t& highest) {
  for (auto& ele : rock) {
    ele.row += highest + 3;
  }
}

void move(Rock& rock) {
  for (auto& ele : rock) {
    ele.row--;
  }
}

bool intersection(const std::vector<std::array<char, n_cols_in_chamber>>& chamber, const Rock& rock) {
  for (const auto& ele : rock) {
    if (ele.row < chamber.size()) {
      if (chamber[ele.row][ele.col] == '#') {
        return true;
      }
    }
  }
  return false;
}

void add_rock_to_chamber(
  std::vector<std::array<char, n_cols_in_chamber>>& chamber,
  const Rock& rock
) {
  for (const auto& ele : rock) {
    std::array<char, n_cols_in_chamber> temp;
    std::fill(std::begin(temp), std::end(temp), '.');
    while(chamber.size() <= ele.row) {
      chamber.push_back(temp);
    }
  }
  for (const auto& ele : rock) {
    chamber[ele.row][ele.col] = '#';
  }
}

void apply_jet(Rock& rock, const std::string& jets, const int jet_index, const std::vector<std::array<char, n_cols_in_chamber>>& chamber) {
  // Debug
  // for (int i = 0; i < jet_index; i++) {
  //   std::cout << ' ';
  // }
  // std::cout << '^' << '\n';
  const auto prev = rock;
  if (const char j = jets[jet_index]; j == '>') {
    for (auto& ele : rock) {
      ele.col++;
      if (ele.col > 6) {
        rock = prev;
        return;
      }
    }
  } else if (j == '<') {
    for (auto& ele : rock) {
      ele.col--;
      if (ele.col < 0) {
        rock = prev;
        return;
      }
    }
  }
  if (intersection(chamber, rock)) {
    rock = prev;
  }
}

void print(const Rock& rock) {
  for (const auto ele : rock) {
    std::cout << '(' << ele.row << ", " << ele.col << ')' << '\n';
  }
  std::cout << '\n';
}

void print_chamber(const Chamber& chamber) {
  std::cout << "Chamber size: " << chamber.size() << '\n';
  for (std::size_t i = chamber.size()-1; i > 0; i--) {
    std::cout << "|";
    for (const auto & ele : chamber[i]) {
      std::cout << ele;
    }
    std::cout << "|" << '\n';
  }
  std::cout << "+";
  for (std::size_t i = 0; i < chamber[0].size()+2; i++) {
    std::cout << '-';
  }
  std::cout << "+" << '\n';
}

std::tuple<bool, std::size_t, std::size_t> check_if_seen_else_add(
  std::unordered_map<Memory, std::size_t, MemoryHash>& history,
  const Memory& memory,
  const std::size_t& iteration
) {
  if (const auto it = history.find(memory); it != history.end()) {
    return {true, it->second, it->first.highest};
  }
  history[memory] = iteration;
  return {false, 0, 0};
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_17_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::string jets;
  std::fstream file(input);
  std::getline(file, jets);

  std::vector<Rock> rocks {
    Rock{Point(0,0), Point(0,1), Point(0,2), Point(0,3)},
    Rock{Point(0,1), Point(1,0), Point(1,1), Point(1,2), Point(2,1)},
    Rock{Point(2,2), Point(1,2), Point(0,0), Point(0,1), Point(0,2)},
    Rock{Point(0,0), Point(1,0), Point(2,0), Point(3,0)},
    Rock{Point(0,0), Point(0,1), Point(1,0), Point(1,1)}
  };

  for (auto& rock : rocks) {
    for (auto& ele : rock) {
      ele.col += 2;
    }
  }

  std::size_t highest = 1;
  int rock_count = 0;
  int jet_index = 0;
  std::vector<std::array<char, n_cols_in_chamber>> chamber;
  std::unordered_map<Memory, std::size_t, MemoryHash> history;
  std::array<char, n_cols_in_chamber> temp;
  std::fill(std::begin(temp), std::end(temp), '#');
  chamber.push_back(temp);

  Chamber top_n; // Mini chamber, top_n rows of chamber
  for (auto& row : top_n) {
    std::fill(std::begin(row), std::end(row), 0);
  }

  while (rock_count < 1000000000000) {
    auto rock = rocks[rock_count % 5];
    rock_count++;
    move_to_starting_height(rock, highest);
    auto prev = rock;
    while (!intersection(chamber, rock)) {
      apply_jet(rock, jets, jet_index, chamber);
      jet_index++;
      if (jet_index == jets.size()) jet_index = 0;
      prev = rock;
      move(rock);
    }

    add_rock_to_chamber(chamber, prev);
    highest = chamber.size();
    top_n = Chamber();
    for (int i = std::max(chamber.size()-5, std::size_t(0)), j = 0; i < chamber.size(); i++, j++) {
      top_n[j] = chamber[i];
    }

    const auto memory = Memory(top_n, jet_index, rock_count%5, highest);
    const auto [seen, prev_level, prev_highest] = check_if_seen_else_add(history, memory, rock_count);
    if (seen) {
      // std::cout << "Seen first at level: " << prev_level << '\n';
      // std::cout << "Seen again at level " << rock_count << '\n';
      const std::size_t delta_iter = rock_count - prev_level;
      const std::size_t delta_height = highest - prev_highest;
      const std::size_t n_rep = (std::size_t(1000000000000) - std::size_t(prev_level)) / delta_iter;
      const std::size_t n_rem = (std::size_t(1000000000000) - std::size_t(prev_level)) % delta_iter;
      // Use STl
      std::size_t extra_height = 0;
      for (const auto& [m, iter] : history) {
        if (std::size_t(iter) - prev_level == n_rem) {
          extra_height = m.highest - prev_highest;
	        break;
        }
      }
      std::size_t total_height = prev_highest + delta_height * (n_rep) + extra_height;
      std::cout << total_height - 1 << '\n';
      return 0;
    }
  }
  std::cout << highest - 1 << '\n';
  return 0;
}
