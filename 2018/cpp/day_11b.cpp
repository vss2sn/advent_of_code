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
  for (int row = 0; row < y_max; row++) {
    for (int col = 0; col < x_max; col ++) {
      const int rack_id = col + 10;
      grid[row][col] = (((((rack_id * row) + gsn) * rack_id) / 100) % 10) - 5;
    }
  }

  std::vector<std::vector<std::vector<long long>>> power(grid_size+1, std::vector<std::vector<long long>>(y_max, std::vector<long long>(x_max, std::numeric_limits<long long>::min())));

  int max_row = 0;
  int max_col = 0;
  int max_dg = 1; // dg = delta grid
  long long max_val = std::numeric_limits<long long>::min();
  power[1] = grid;
  for (int dg_size = 2; dg_size <= grid_size; dg_size++) {
    for (int row = 0; row < x_max - dg_size; row++) {
      for (int col = 0; col < y_max - dg_size; col++){
        long long sum = power[dg_size - 1][row][col];
        for (int delta = 0; delta < dg_size; delta++) {
          sum += grid[row + dg_size - 1][col + delta];
          sum += grid[row + delta][col + dg_size - 1];
        }
        sum -= grid[row + dg_size - 1][col + dg_size - 1];
        power[dg_size][row][col] = sum;
        if (sum > max_val) {
          max_val = sum;
          max_col = col;
          max_row = row;
          max_dg = dg_size;
        }
      }
    }
  }
  std::cout << max_val << ' ' << max_dg << ' ' << max_col <<  ' ' << max_row << '\n';
  return 0;
}
