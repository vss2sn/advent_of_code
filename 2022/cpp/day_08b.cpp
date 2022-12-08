#include <fstream>
#include <iostream>
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

  std::vector<std::vector<int>> scores(
    heights.size(),
    std::vector<int>(heights[0].size(), 1)
  );

  const std::vector<std::vector<int>> motions {
    {0,1},
    {1,0},
    {0,-1},
    {-1,0}
  };

  const int max_rows = heights.size();
  const int max_cols = heights[0].size();

  const auto in_bounds = [max_rows, max_cols](const int row, const int col) {
    return (row < max_rows && row >= 0 && col < max_cols && col >=0);
  };

  for (int row = 0; row < heights.size(); row++) {
    for (int col = 0; col < heights[0].size(); col++) {
      for (const auto& motion : motions) {
        int score = 0;
        auto current = std::vector<int>{row + motion[0], col + motion[1]};
        while(in_bounds(current[0], current[1]) && heights[row][col] > heights[current[0]][current[1]]) {
          score++;
          current[0] += motion[0];
          current[1] += motion[1];
        }
        if (in_bounds(current[0], current[1])) score++;
        scores[row][col] *= score;
      }
    }
  }

  int best_score = 0;
  for (const auto row : scores) {
    for(const auto ele : row) {
      best_score = std::max(best_score, ele);
    }
  }
  std::cout << best_score << '\n';


  return 0;
}
