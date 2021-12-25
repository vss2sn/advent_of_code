#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

template<typename T>
void print(std::vector<std::vector<T>>& v) {
  for (const auto& row : v) {
    for(const auto ele : row) {
      std::cout << ele;
    }
    std::cout << '\n';
  }
}

std::tuple<int, int> get_next(
  const std::vector<std::vector<char>>&cucumbers,
  const int row,
  const int col,
  const char dir
) {
  int next_row = 0;
  int next_col = 0;
  if (dir == 'v') {
    next_col = col;
    if ((row + 1) == cucumbers.size()) {
      next_row = 0;
    } else {
      next_row = row + 1;
    }
  }
  else if (dir == '>') {
    next_row = row;
    if ((col + 1) == cucumbers[0].size()) {
      next_col = 0;
    } else {
      next_col = col + 1;
    }
  }

  return {next_row, next_col};
}

bool move(std::vector<std::vector<char>>& cucumbers) {
  bool changed = false;
  auto next_step_cucumbers = cucumbers;
  int c1 = 0, c2 = 0, c3 = 0;

  for (int row = 0; row < cucumbers.size(); row++) {
    for (int col = 0; col < cucumbers[row].size(); col++) {
      if (cucumbers[row][col] == '>') {
        const auto [next_row, next_col] = get_next(cucumbers, row, col, cucumbers[row][col]);
        if (cucumbers[next_row][next_col] == '.') {
          next_step_cucumbers[next_row][next_col] = cucumbers[row][col];
          next_step_cucumbers[row][col] = '.';
          changed = true;
        }
      }
    }
  }
  cucumbers = next_step_cucumbers;

  for (int row = 0; row < cucumbers.size(); row++) {
    for (int col = 0; col < cucumbers[row].size(); col++) {
      if (cucumbers[row][col] == 'v') {
        const auto [next_row, next_col] = get_next(cucumbers, row, col, cucumbers[row][col]);
        if (cucumbers[next_row][next_col] == '.') {
          next_step_cucumbers[next_row][next_col] = cucumbers[row][col];
          next_step_cucumbers[row][col] = '.';
          changed = true;
        }
      }
    }
  }
  cucumbers = next_step_cucumbers;

  return changed;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_25_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::vector<std::vector<char>> cucumbers;
  while(std::getline(file, line)) {
    cucumbers.emplace_back();
    for (const auto c : line) {
      // Some error in file formatting on local machine;
      // adding additional check. TBDebugged later
      if (c == '.' || c == '>' || c == 'v') {
        cucumbers.back().emplace_back(c);
      }
    }
  }

  bool moved = true;
  int step = 0;
  while(moved) {
    moved = move(cucumbers);
    step++;
  }
  std::cout << step << '\n';
  return 0;
}
