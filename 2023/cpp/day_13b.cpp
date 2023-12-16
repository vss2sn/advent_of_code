#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

bool check_row(const std::string& row, int col) {
  for (int lhs = col, rhs = col+1; lhs >= 0 && rhs < row.size(); lhs--, rhs++) {
    if (row[lhs] != row[rhs]) return false;
  }
  return true;
}

// Can make possible_values_map a set
std::pair<bool , std::vector<int>> find_reflection(const std::vector<std::string>& pattern) {
  bool is_center = false;
  std::vector<bool> possible_values_map(pattern[0].size()-1, true);
  for (int col = 0; col < pattern[0].size() - 1; col++) {
    possible_values_map[col] = check_row(pattern[0], col);
  }
  for (int row_idx = 1; row_idx < pattern.size(); row_idx++) {
    for (int values_idx = 0; values_idx < possible_values_map.size(); values_idx++) {
      if (!possible_values_map[values_idx]) {
        continue;
      }
      possible_values_map[values_idx] = check_row(pattern[row_idx], values_idx);
    }
  }
  
  std::vector<int> possible_values;
  for (int values_idx = 0; values_idx < possible_values_map.size(); values_idx++) {
    if (possible_values_map[values_idx]) {
      possible_values.push_back(values_idx);
    } 
  }
  return {!possible_values.empty(), possible_values};
}

std::vector<std::string> rotate_pattern_anti_cw (const std::vector<std::string>& pattern) {
  std::vector<std::string> rotated(pattern[0].size(), std::string(pattern.size(), ' '));
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

  std::vector<std::pair<bool, int>>  previous_lines;
  for (auto& pattern : patterns) { 
    for (const auto& row : pattern) {              
      const auto [found, centers] = find_reflection(pattern);
      if (found) {
        previous_lines.push_back({true, centers[0]});
        break;
      }
      const auto [found_rotated, centers_rotated] = find_reflection(rotate_pattern_anti_cw(pattern));
      if (found_rotated) {
        previous_lines.push_back({false, centers_rotated[0]});
        break;
      }
    }
  }

  std::size_t ans =  0;
  for (int p = 0; p < patterns.size(); p++) { 
    auto& pattern = patterns[p];
    auto rotated = rotate_pattern_anti_cw(pattern);
    bool found_main = false;
    for (int i = 0; i < pattern.size() && !found_main; i++) {
      for (int j = 0; j < pattern[0].size() && !found_main; j++) {
        pattern[i][j] = (pattern[i][j] == '.' ? '#' : '.');
        rotated[pattern[0].size() - j - 1][i] =  pattern[i][j];
        for (const auto& row : pattern) {
          if (found_main) break;              
          const auto [found, centers] = find_reflection(pattern);
          for (const auto center : centers) {
            if (found && !(previous_lines[p].first && previous_lines[p].second == center)) {
              ans += (center + 1);
              found_main = true;
              break;
            }
          }
          if (found_main) break;
          const auto [found_rotated, centers_rotated] = find_reflection(rotated);
          for (const auto center_rotated : centers_rotated) {
            if (found_rotated && !(!previous_lines[p].first && previous_lines[p].second == center_rotated)) {
              ans += (center_rotated + 1) * 100;
              found_main = true;
              break;
            }
          }
          if (found_main) break;
        }
        pattern[i][j] = (pattern[i][j] == '.' ? '#' : '.');
        rotated[pattern[0].size() - j - 1][i] =  pattern[i][j];
      }
    }
    // if(!found_main) {
    //   std::cout << "This should not happen" << '\n';
    //   exit(0);
    // }
  }
  std::cout << ans << '\n';
  return 0;
}