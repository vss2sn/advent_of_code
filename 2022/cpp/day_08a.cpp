#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main(int argc, char * argv[]) {
  std::string input = "../input/day_08_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::vector<std::vector<int>> heights;
  while(std::getline(file, line)) {
    heights.emplace_back();
    for (const char c : line) {
      heights.back().emplace_back(c-'0');
    }
  }

  std::vector<std::vector<bool>> visible(
    heights.size(),
    std::vector<bool>(heights[0].size(), false)
  );

  const int max_rows = heights.size();
  const int max_cols = heights[0].size();

  for (int row = 0; row < heights.size(); row++) {
    visible[row][0] = true;
    visible[row][max_cols-1] = true;
  }
  for (int col = 0; col < heights[0].size() - 1; col++) {
    visible[0][col] = true;
    visible[max_rows-1][col] = true;
  }

  for (int row = 0; row < heights.size(); row++) {
    int max_yet_right = heights[row][0];
    int max_yet_left = heights[row][max_cols - 1];
    for (int col = 0; col < heights[0].size() - 1; col++) {
      if (max_yet_right < heights[row][col+1]) {
        visible[row][col + 1] = true;
        max_yet_right = heights[row][col + 1];
      }
      if (max_yet_left < heights[row][max_cols - col - 1 - 1]) {
        visible[row][max_cols - col - 1 - 1] = true;
        max_yet_left = heights[row][max_cols - col - 1 - 1];
      }
    }
  }

  for (int col = 0; col < heights[0].size(); col++) {
    int max_yet_down = heights[0][col];
    int max_yet_up = heights[max_cols - 1][col];
    for (int row = 0; row < heights.size() - 1; row++) {
      if (max_yet_down < heights[row+1][col]) {
        visible[row+1][col] = true;
        max_yet_down = heights[row+1][col];
      }
      if (max_yet_up < heights[max_rows - row - 1 - 1][col]) {
        visible[max_rows - row - 1 - 1][col] = true;
        max_yet_up = heights[max_rows - row - 1 - 1][col];
      }
    }
  }

  const int count = std::accumulate(
    std::begin(visible),
    std::end(visible),
    0,
    [](const int sum, const std::vector<bool>& row) { return sum + std::count(std::begin(row), std::end(row), true); });
  std::cout << count << '\n';


  return 0;
}
