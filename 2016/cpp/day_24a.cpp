#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>  

struct Point {
  Point(const int row = 0, const int col = 0) : row(row), col(col) {}
  int row;
  int col;
  bool operator == (const Point& p) const {
    return row == p.row && col == p.col;
  }
};

struct Cost {
  int already_incurred = 0;
  int heuristic = 0;
  int total = 0;
};

struct PointWithCost {
  Point point;
  Cost cost;
  bool operator == (const PointWithCost& pc) const {
    return point == pc.point;
  }
};

struct Comparator {
  bool operator () (const PointWithCost& p1, const PointWithCost& p2) {
    return p1.cost.total > p2.cost.total;
  }
};

struct hasher {
  std::size_t operator () (const PointWithCost& pc) const  {
    return pc.point.row << 10 + pc.point.col;
  }
};

int calculate_distance(const Point& p1, const Point& p2, const std::vector<std::string>& map) {
  std::unordered_set<PointWithCost, hasher> seen;
  const std::vector<Point> moves {
    Point(0,1),
    Point(1,0),
    Point(0,-1),
    Point(-1,0)
  };
  PointWithCost start;
  start.point = p1;
  std::priority_queue<PointWithCost, std::vector<PointWithCost>, Comparator> pq;
  pq.push(start);
  while(!pq.empty()) {
    const auto current = pq.top();
    pq.pop();
    if (seen.find(current) != seen.end()) continue;
    seen.insert(current);
    if (current.point.row == p2.row && current.point.col == p2.col) {
      return current.cost.already_incurred;
    }
    for (const auto& move : moves) {
      PointWithCost next;
      next.point.row = current.point.row + move.row;
      next.point.col = current.point.col + move.col;
      if (next.point.row >= map.size() || next.point.row < 0 || next.point.col >= map[0].size() || next.point.col < 0 || map[next.point.row][next.point.col] == '#') {
        continue;
      }
      next.cost.already_incurred = current.cost.already_incurred + 1;
      next.cost.heuristic = std::abs(p2.row - next.point.row) + std::abs(p2.col - next.point.col);
      next.cost.total = next.cost.heuristic + next.cost.already_incurred;
      pq.push(next);
    }
  }
  std::cout << "path not found" << '\n';
  return -1;
}

void dfs_util(int current, const std::vector<std::vector<int>>& am, std::vector<bool>& seen, int cost, int& min_cost) {
  if (all_of(std::begin(seen), std::end(seen), [](const auto ele) {return ele;})) {
    min_cost = std::min(cost, min_cost);
    return;
  }
  for (int i = 0; i < seen.size(); i++) {
    if (seen[i]) continue;
    seen[i] = true;
    cost += am[current][i];
    dfs_util(i,am,seen,cost, min_cost);
    cost -= am[current][i];
    seen[i] = false;
  }
}

int dfs(const std::vector<std::vector<int>>& am) {
  std::vector<bool> seen(am.size(), false);
  seen[0] = true;
  int min_cost = std::numeric_limits<int>::max();
  dfs_util(0, am, seen, 0, min_cost);
  return min_cost;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_24_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  std::vector<std::string> map;
  std::vector<Point> points;
  while(std::getline(file, line)) {
    map.push_back(line);
    for (int idx = 0; idx < line.size(); idx++) {
      const char c = line[idx];
      if (c >= '0' && c <= '9') {
        Point p;
        p.row = map.size() - 1;
        p.col = idx;
        if (points.size() <= (c - '0')) {
          points.resize(c - '0' + 1);
        }
        points[c-'0'] = p;
      } 
    }
  }
  std::vector<std::vector<int>> adjacency_matrix(points.size(), std::vector<int>(points.size()));
  for (int idx_1 = 0; idx_1 < points.size(); idx_1++) {
    for (int idx_2 = idx_1+1; idx_2 < points.size(); idx_2++) {
      const auto dist = calculate_distance(points[idx_1], points[idx_2], map);
      adjacency_matrix[idx_1][idx_2] = dist;
      adjacency_matrix[idx_2][idx_1] = dist;
    }
  }
  std::cout << dfs (adjacency_matrix) << '\n';
  return 0;
}