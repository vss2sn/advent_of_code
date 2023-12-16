#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main(int argc, char * argv[]) {
  std::string input = "../input/day_14_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<std::string> platform;
  while(std::getline(file, line)) {
    platform.push_back(line);
  }

  std::size_t ans = 0;
  // Tilt north
  for (int row_idx = 1; row_idx < platform.size(); row_idx++) {
    for (int col_idx = 0; col_idx < platform[0].size(); col_idx++) {
      if (platform[row_idx][col_idx] == 'O') {
        int new_row_idx = row_idx - 1;
        while (new_row_idx >= 0) {
          if (platform[new_row_idx][col_idx] != '.') {
            // std::cout << "Obstruction at (" << new_row_idx << ',' << col_idx << ")" << '\n';
            new_row_idx++;
            break;
          }
          new_row_idx--;
        }
        if (new_row_idx < 0) new_row_idx  = 0;
        if (row_idx != new_row_idx) {
          platform[row_idx][col_idx] = '.';
          platform[new_row_idx][col_idx] = 'O';
        }
      }
    }
  }
  
  // Calculate load
  for (int row_idx = 0; row_idx < platform.size(); row_idx++) {
    for (int col_idx = 0; col_idx < platform[0].size(); col_idx++) {
      ans += (platform[row_idx][col_idx] == 'O' ? platform.size() - row_idx : 0);
    }
  }
  std::cout << ans << '\n';
  return 0;
}