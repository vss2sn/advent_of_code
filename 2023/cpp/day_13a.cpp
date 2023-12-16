#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

bool check_row(const std::string& row, const int col) {
  for (int lhs = col, rhs = col+1; lhs >= 0 && rhs < row.size(); lhs--, rhs++) {
    if (row[lhs] != row[rhs]) return false;
  }
  return true;
}

// Make possible_values a set
std::pair<bool , int> find_reflection(const std::vector<std::string>& pattern) {
  bool is_center = false;
  std::vector<bool> possible_values(pattern[0].size()-1, true);
  for (int col = 0; col < pattern[0].size() - 1; col++) {
    possible_values[col] = check_row(pattern[0], col);
  }
  for (int row_idx = 1; row_idx < pattern.size(); row_idx++) {
    for (int values_idx = 0; values_idx < possible_values.size(); values_idx++) {
      if (!possible_values[values_idx]) {
        continue;
      }
      possible_values[values_idx] = check_row(pattern[row_idx], values_idx);
    }
  }
  for (int values_idx = 0; values_idx < possible_values.size(); values_idx++) {
    if (possible_values[values_idx]) {
      return {true, values_idx};
    } 
  }
  return {false, -1};
}

std::vector<std::string> rotate_pattern_anti_cw (const std::vector<std::string>& pattern) {
  std::vector<std::string> rotated(pattern[0].size(), std::string(pattern.size(), ' ')); // Sanity check on size
  for (int row = 0; row < pattern.size(); row++) {
    for (int col = 0; col < pattern[0].size(); col++) {
      rotated[pattern[0].size() - col - 1][row] = pattern[row][col];
    }
  }
  return rotated;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_13_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<std::vector<std::string>> patterns;
  while(std::getline(file, line)) {
    patterns.emplace_back();
    auto& pattern = patterns.back();
    while (!line.empty()) {
      pattern.push_back(line);
      std::getline(file, line);
    }
  }

  std::size_t ans =  0;
  for (const auto& pattern : patterns) {     
    const auto [found, center] = find_reflection(pattern);
    if (found) {
      ans += (center + 1);
      continue;
    }
    const auto [found_rotated, center_rotated] = find_reflection(rotate_pattern_anti_cw(pattern));
    if (found_rotated) {
      ans += (center_rotated + 1) * 100;
      continue;
    }
    // std::cout << "This should not happen" << '\n';  // Sanity check
  }
  std::cout << ans << '\n';
  return 0;
}