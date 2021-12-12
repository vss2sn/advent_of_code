#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <cassert>
#include <thread>
#include <chrono>
#include <stack>
#include <queue>

struct Point {
  int row, col;
  Point (const int row, const int col) : row(row), col(col) {}
  bool operator == (const Point& p) const {
    return p.row == row && p.col == col;
  }
};

struct hash_point {
  std::size_t operator()(const Point& p) const {
    return p.row << 5 + p.col;
  }
};

void convert_to_map(const std::unordered_map<Point, char, hash_point>& map) {
  int min_row = std::numeric_limits<int>::max();
  int min_col = std::numeric_limits<int>::max();
  int max_row = std::numeric_limits<int>::min();
  int max_col = std::numeric_limits<int>::min();
  for (const auto& p : map) {
    if (p.first.row < min_row) min_row = p.first.row;
    if (p.first.row > max_row) max_row = p.first.row;
    if (p.first.col < min_col) min_col = p.first.col;
    if (p.first.col > max_col) max_col = p.first.col;
  }

  std::vector<std::vector<char>> map_v((max_row - min_row) + 4, std::vector<char>((max_col - min_col) + 3, '#'));
  for (const auto& p : map) {
    std::cout << p.second;
    map_v[p.first.row - min_row+2][p.first.col - min_col+1] = p.second;
  }
  for (const auto& row : map_v) {
    for (const auto c : row) {
      std::cout << c;
    }
    std::cout << '\n';
  }
}

void move_point(const char d, Point& p, std::unordered_map<Point, char, hash_point>& map) {
  auto delta = Point(-1, -1);
  if (d == 'N') {
    delta = Point(- 1, 0);
  } else if (d == 'E') {
    delta = Point(0, 1);
  } else if (d == 'S') {
    delta = Point(1, 0);
  } else if (d == 'W') {
    delta = Point(0, -1);
  } else {
    std::cout << "Shouldn't happen" << '\n';
    exit(0);
  }
  p.row += delta.row;
  p.col += delta.col;
  if (d == 'N' || d == 'S') {
    map[p] = '-';
  } else {
    map[p] = '|';
  }
  p.row += delta.row;
  p.col += delta.col;
  map[p] = '.';
}

int main(int argc, char * argv []) {
  std::string input = "../input/day_20_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;

  const auto start = Point(0, 0);
  std::getline(file, line);
  const auto regex = line.substr(1, line.size() - 2);
  std::unordered_map<Point, char, hash_point> map;
  map[Point(0, 0)] = 'X';
  std::stack<std::pair<char, Point>> s;

  auto cp = start;
  int index = 0;
  while (index < regex.size()) {
    const auto c = regex[index];
    if (c == '(') {
      s.emplace(c, cp);
    } else if (c == 'N' || c == 'E' || c == 'S' || c == 'W') {
      s.emplace(c, cp);
      move_point(c, cp, map);
    } else if (c == '|') {
      auto new_c = c;
      while (!s.empty() && s.top().first != '(') {
        s.pop();
      }
      cp = s.top().second;
    } else if (c == ')') {
      while (!s.empty() && s.top().first != '(') {
        s.pop();
      }
      cp = s.top().second;
      s.pop();
    }
    index++;
  }
  const auto c = regex[regex.size() - 1];

  const auto get_neighbours = [](const Point& p) {
    return std::vector<Point> {
      Point(p.row+1, p.col),
      Point(p.row-1, p.col),
      Point(p.row, p.col+1),
      Point(p.row, p.col-1)
    };
  };

  long long n = 0;
  std::queue<std::tuple<Point, int>> q;
  std::unordered_set<Point, hash_point> seen;
  q.push({start, 0});
  auto max_point = start;
  auto max_dist = 0;
  while (!q.empty()) {
    const auto [p, count] = q.front();
    if (count >= 2000 && map[p] == '.') n++;
    q.pop();
    seen.insert(p);
    for (const auto& n : get_neighbours(p)) {
      if (auto it = seen.find(n) == seen.end() && map.find(n) != map.end()) {
        q.push({n, count+1});
      }
    }
  }

  std::cout << n << '\n';
  return 0;
}
