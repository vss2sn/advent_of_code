#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void print(std::vector<std::vector<int>>& v) {
  for (const auto& row : v) {
    for(const auto ele : row) {
      std::cout << ele;
    }
    std::cout << '\n';
  }
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_09_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::vector<std::vector<int>> h_map;
  h_map.emplace_back();
  while(std::getline(file, line)) {
    h_map.emplace_back();
    h_map.back().emplace_back(9);
    for(const auto c : line) {
      h_map.back().emplace_back(c - '0');
    }
    h_map.back().emplace_back(9);
  }
  const std::vector<int> temp (h_map[1].size(), 9);
  h_map[0] = temp;
  h_map.emplace_back(temp);

  const auto lowest = [](const int row, const int col, const std::vector<std::vector<int>>& h_map) {
    return (
      h_map[row+1][col] > h_map[row][col] &&
      h_map[row-1][col] > h_map[row][col] &&
      h_map[row][col+1] > h_map[row][col] &&
      h_map[row][col-1] > h_map[row][col]
    );
  };
  long long risk = 0;
  for (int row = 1; row < h_map.size() - 1; row ++) {
    for (int col = 1; col < h_map[row].size() - 1; col++) {
      if (lowest(row, col, h_map)) {
        risk += 1 + h_map[row][col];
      }
    }
  }
  std::cout << risk << '\n';
  return 0;
}
