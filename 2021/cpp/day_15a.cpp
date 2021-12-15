#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

struct Point {
  int row, col;
  Point(const char row, const char col) : row(row), col(col) {}
  bool operator == (const Point& p) const {
    return row == p.row && col == p.col;
  }
  friend std::ostream& operator << (std::ostream& os, const Point& p);
};

struct compare {
  bool operator () (const std::pair<Point, int>& p1, const std::pair<Point, int>& p2) const {
    return p1.second > p2.second;
  }
};

std::ostream& operator << (std::ostream& os, const Point& p) {
  os << "(" << p.row << ", " << p.col << ")";
  return os;
}

struct hash_point {
  std::size_t operator () (const Point& p) const {
    return p.row * p.col;
  }
};

int main(int argc, char * argv[]) {
  std::string input = "../input/day_15_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::vector<std::vector<int>> map;
  while(std::getline(file, line)) {
    map.emplace_back();
    for (const auto ele : line) {
      map.back().emplace_back(ele  - '0');
    }
  }

  std::priority_queue<std::pair<Point, int>, std::vector<std::pair<Point, int>>, compare> q;
  std::unordered_set<Point, hash_point> seen;

  const auto get_neighbours = [](const Point& p) {
    return std::vector<Point> {
      Point(p.row + 1, p.col),
      Point(p.row - 1, p.col),
      Point(p.row, p.col + 1),
      Point(p.row, p.col - 1)
    };
  };

  const auto n_rows = map.size();
  const auto n_cols = map[0].size();

  const auto in_bounds = [&n_rows, &n_cols](const Point& p) {
    return p.row >= 0 && p.col >=0 && p.row < n_rows && p.col < n_cols;
  };

  const auto start = Point(0,0);
  const auto goal = Point(n_rows - 1, n_cols - 1);
  q.emplace(start, 0);

  while(!q.empty()) {
    const auto [cp, cost] = q.top();
    q.pop();
    if(cp == goal) {
      std::cout << cost << '\n';
      break;
    }
    for (const auto& n : get_neighbours(cp)) {
      if (in_bounds(n) && seen.find(n) == seen.end()) {
        q.emplace(n, cost + map[n.row][n.col]);
        seen.insert(n);
      }
    }
  }
  
  return 0;
}
