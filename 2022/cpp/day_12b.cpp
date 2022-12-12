#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <stack>
#include <string>
#include <vector>

struct Point {
  Point(const int row, const int col, const int parent, const int steps) : row(row), col(col), parent(parent), steps(steps) {}
  int row, col, parent, steps;
};

struct PointHash {
  std::size_t operator() (const Point& p) {
    return p.row * p.col;
  }
};

int main(int argc, char * argv[]) {
  std::string input = "../input/day_12_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::vector<Point> motions = {
    Point(0, -1, 0, 0),
    Point(-1, 0, 0, 0),
    Point(0, 1, 0, 0),
    Point(1, 0, 0, 0)
  };

  std::vector<std::string> grid;
  Point start(0,0,0, 0);
  Point end(0,0,0, 0);
  while(std::getline(file, line)) {
    grid.push_back(line);
    auto it = line.find('S');
    if (it != std::string::npos) {
      grid[grid.size() - 1][it] = 'a';
    }
    it = line.find('E');
    if (it != std::string::npos) {
      end.row = grid.size() - 1;
      end.col = it;
      end.parent = 0;
      end.steps = 0;
      grid[end.row][end.col] = 'z';
    }
  }

  // TODO repalce with unordered set
  std::vector<std::vector<bool>> visited(
    grid.size(),
    std::vector<bool>(grid[0].size(), false)
  );

  // Point current;
  bool found = false;
  std::queue<Point> q;
  q.push(end);

  // Make lambda as_const
  const auto in_bounds = [&grid](const auto& p) {
    return p.row >= 0 && p.col >= 0 && p.row < grid.size() && p.col < grid[0].size();
  };

  // TODO Add operators for point
  while (!q.empty()) {
    auto current = q.front();
    q.pop();
    if (visited[current.row][current.col]) continue;
    visited[current.row][current.col] = true;
    if (grid[current.row][current.col] == 'a') {
      found = true;
      std::cout << current.steps << '\n';
      break;
    }
    for (const auto& motion : motions) {
      const auto new_point = Point(
        current.row + motion.row,
        current.col + motion.col,
        current.row * grid[0].size() + current.col,
        current.steps + 1
      );
      if (!in_bounds(new_point)) continue;
      if (visited[new_point.row][new_point.col]) continue;
      if (grid[current.row][current.col] > grid[new_point.row][new_point.col]+1) continue;
      q.push(new_point);
    }
  }

  return 0;
}
