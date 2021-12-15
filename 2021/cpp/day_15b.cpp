#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

void print(const std::vector<std::vector<int>>& v) {
  for (const auto& row : v) {
    for (const auto& col : row) {
      std::cout << col ;
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

struct Point {
  int row, col;
  Point(const int row, const int col) : row(row), col(col) {}
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

  std::vector<std::vector<int>> input_map;
  while(std::getline(file, line)) {
    input_map.emplace_back();
    for (const auto ele : line) {
      input_map.back().emplace_back(ele  - '0');
    }
  }

  std::vector<std::vector<int>> map(
    input_map.size() * 5,
    std::vector<int>(input_map[0].size() * 5, 0)
  );

  int offset = 0;
  for (int m = 0; m < (5 * 5); m++) {
    const auto row_offset = (offset / 5);
    const auto col_offset = (offset % 5);
    const auto row_offset_m = row_offset * input_map.size();
    const auto col_offset_m = col_offset * input_map[0].size();
    for (int i = 0; i < input_map.size(); i++) {
      for (int j = 0; j < input_map[0].size(); j++) {
        auto& v = map[i + row_offset_m][j + col_offset_m];
        v = (input_map[i][j] + (row_offset + col_offset));
        if (v > 9) {
          v = v % 9;
          if (v == 0) {
            v = 1;
          }
        }
      }
    }
    offset += 1;
  }

  std::priority_queue<std::pair<Point, int>, std::vector<std::pair<Point, int>>, compare> q;
  std::unordered_set<Point, hash_point> already_inserted;

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

  const auto start = Point(0,0);
  const auto goal = Point(n_rows - 1, n_cols - 1);

  const auto in_bounds = [&n_rows, &n_cols](const Point& p) {
    return p.row >= 0 && p.col >=0 && p.row < n_rows && p.col < n_cols;
  };

  q.emplace(start, 0);
  while(!q.empty()) {
    const auto [cp, cost] = q.top();
    q.pop();
    if(cp == goal) {
      std::cout << cost << '\n';
      break;
    }
    for (const auto& n : get_neighbours(cp)) {
      if (in_bounds(n) && already_inserted.find(n) == already_inserted.end()) {
        q.emplace(n, cost + map[n.row][n.col]);
        already_inserted.insert(n);
      }
    }
  }

  return 0;
}
