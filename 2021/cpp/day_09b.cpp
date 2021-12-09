#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

struct Point {
  int row, col;
  Point(const int row, const int col) : row(row), col (col) {}
  bool operator == (const Point& p) const {
    return row == p.row && col == p.col;
  }
};

struct pair_hash {
  std::size_t operator () (const Point& p) const {
    return p.row * p.col;
  }
};

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

  std::vector<Point> low_pts;
  const auto lowest = [](const int row, const int col, const std::vector<std::vector<int>>& h_map) {
    return (
      h_map[row+1][col] > h_map[row][col] &&
      h_map[row-1][col] > h_map[row][col] &&
      h_map[row][col+1] > h_map[row][col] &&
      h_map[row][col-1] > h_map[row][col]
    );
  };

  for (int row = 1; row < h_map.size() - 1; row ++) {
    for (int col = 1; col < h_map[row].size() - 1; col++) {
      if (lowest(row, col, h_map)) {
        low_pts.emplace_back(row, col);
      }
    }
  }

  const auto get_neighbours = [](const int row, const int col) {
    return std::vector<Point> {
      Point(row+1, col),
      Point(row-1, col),
      Point(row, col+1),
      Point(row, col-1)
    };
  };

  std::vector<long long> basin_size;
  long long total_count = 1;

  for (const auto& p : low_pts) {
    std::queue<Point> q;
    std::unordered_set<Point, pair_hash> seen;
    q.push(p);
    seen.insert(p);
    long long count = 0;
    while (!q.empty()) {
      const auto neightbours = get_neighbours(q.front().row, q.front().col);
      q.pop();
      for (const auto& n : neightbours) {
        if (h_map[n.row][n.col] != 9 && seen.find(n) == seen.end()) {
          count++;
          q.push(n);
          seen.insert(n);
        }
      }
    }
    basin_size.emplace_back(count + 1);
  }
  std::sort(std::begin(basin_size), std::end(basin_size));
  std::cout << basin_size[basin_size.size() - 3]*basin_size[basin_size.size() - 2]*basin_size[basin_size.size() - 1] << '\n';
  return 0;
}
