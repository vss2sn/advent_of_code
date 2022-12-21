#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Point {
  Point(const int row, const int col) : row (row), col(col) {}
  int row;
  int col;
};

using Rock = std::vector<Point>;

void move_to_starting_height(Rock& rock, int highest) {
  for (auto& ele : rock) {
    ele.row += highest + 3;
  }
}

void move(Rock& rock) {
  for (auto& ele : rock) {
    ele.row--;
  }
}

bool intersection(const std::vector<std::array<char, 7>>& chamber, const Rock& rock) {
  for (const auto& ele : rock) {
    if (ele.row < chamber.size()) {
      if (chamber[ele.row][ele.col] == '#') {
        return true;
      }
    }
  }
  return false;
}

void add_rock_to_chamber(std::vector<std::array<char, 7>>& chamber, const Rock& rock) {
  for (const auto& ele : rock) {
    while(chamber.size() <= ele.row) {
      std::array<char, 7> temp;
      std::fill(std::begin(temp), std::end(temp), '.');
      chamber.push_back(temp);
    }
  }
  for (const auto& ele : rock) {
    chamber[ele.row][ele.col] = '#';
  }
}

void apply_jet(Rock& rock, const std::string& jets, const int jet_index, const std::vector<std::array<char, 7>>& chamber) {
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

void print_chamber(const std::vector<std::array<char, 7>>& chamber) {
  std::cout << chamber.size() << '\n';
  for (int i = chamber.size()-1; i > 0; i--) {
    std::cout << "|";
    for (const auto & ele : chamber[i]) {
      std::cout << ele;
    }
    std::cout << "|" << '\n';
  }
  std::cout << "+";
  for (int i = 0; i < chamber[0].size()+2; i++) {
    std::cout << '-';
  }
  std::cout << "+" << '\n';
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

  int highest = 1;
  int rock_count = 0;
  int jet_index = 0;
  std::vector<std::array<char, 7>> chamber;
  std::array<char, 7> temp;
  std::fill(std::begin(temp), std::end(temp), '#');
  chamber.push_back(temp);
  while (rock_count < 2022) {
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
  }
  std::cout << highest - 1 << '\n';
}
