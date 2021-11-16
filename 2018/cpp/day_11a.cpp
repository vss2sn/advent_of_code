#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <thread>
#include <memory>
#include <numeric>
#include <cassert>

int main(int argc, char* argv[]) {
  std::string input = "../input/day_11_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string line;
  std::getline(file, line);
  const int gsn = std::stoi(line);

  const int grid_size = 300;
  const int x_max = grid_size;
  const int y_max = grid_size;

  std::vector<std::vector<long long>> grid(y_max, std::vector<long long>(x_max));
  std::vector<std::vector<long long>> power(y_max, std::vector<long long>(x_max));
  for (int row = 0; row < y_max; row++) {
    for (int col = 0; col < x_max; col ++) {
      const int rack_id = col + 10;
      grid[row][col] = (((((rack_id * row) + gsn) * rack_id) / 100) % 10) - 5;
    }
  }

  int max_row = 0;
  int max_col = 0;
  int dg_size = 3;
  long long max_val = std::numeric_limits<long long>::min();
  long long sum = 0;
  for (int dx = 0; dx < dg_size; dx++) {
    for (int dy = 0; dy < dg_size; dy++) {
      sum +=grid[dy][dx];
    }
  }
  power[0][0] = sum;
  for (int row = 0; row < x_max - dg_size; row++) {
    if (row > 0) {
      sum = power[row - 1][0];
      for (int delta = 0; delta < dg_size; delta++) {
        sum -= grid[row - 1][delta];
        sum += grid[row + dg_size - 1][delta];
      }
    }
    for (int col = 0; col < y_max - dg_size; col++){
      if (col > 0) {
        for (int delta = 0; delta < dg_size; delta++) {
          sum -= grid[row + delta][col - 1];
          sum += grid[row + delta][col + dg_size - 1];
        }
      }
      power[row][col] = sum;
      if (sum > max_val) {
        max_val = sum;
        max_col = col;
        max_row = row;
      }
    }
  }
  std::cout << max_val << ' ' << ' ' << max_col <<  ' ' << max_row << '\n';
  return 0;
}
