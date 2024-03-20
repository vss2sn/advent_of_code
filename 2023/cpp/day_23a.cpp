#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue> 
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

struct Point {
  int row;
  int col;

  Point(const int row = 0, const int col = 0) : row(row), col(col) {}

  Point operator + (const Point& p) const {
    return Point (p.row + row,  p.col + col);
  }

  bool operator == (const Point& p) const {
    return p.row == row && p.col == col;
  }
};

struct hasher {
  std::size_t operator() (const Point& p) const {
    return p.row + p.col;
  }
};

const std::vector<Point> motions ={
  Point(-1,0),
  Point(0,1),
  Point(1,0),
  Point(0, -1),
};

bool in_limits(const std::vector<std::string>& map, const int row, const int col) {
  return row >= 0 && row < map.size() && col >= 0 && col < map[0].size();
}

void dfs(std::stack<Point>& stack, const std::vector<std::string>& map, int steps, int& max_steps, const Point& dest, std::vector<std::vector<bool>>& seen) {
  if (!stack.empty()) {
    const Point current = stack.top();
    seen[current.row][current.col] = true;
    if (current == dest) {
      max_steps = std::max(steps, max_steps);
      seen[current.row][current.col] = false;
      return;
    }
    if (map[current.row][current.col] != '.') {
      Point next;
      if (map[current.row][current.col] == '^') {
        next.row = current.row - 1;
        next.col = current.col;
      }
      else if (map[current.row][current.col] == '>') {
        next.row = current.row;
        next.col = current.col + 1;
      }
      else if (map[current.row][current.col] == 'v') {
        next.row = current.row + 1;
        next.col = current.col;
      }
      else if (map[current.row][current.col] == '<') {
        next.row = current.row;
        next.col = current.col - 1;
      } 
      else {
        std::cout << "this should not happen" << '\n';
      }
      if(in_limits(map, next.row, next.col) && map[next.row][next.col] != '#' && !seen[next.row][next.col]) {
        stack.push(next);
        dfs(stack, map, steps + 1, max_steps, dest, seen);
        stack.pop();
      }
    } else {
      for (const auto& motion : motions) {
        const auto next = current + motion;
        if(in_limits(map, next.row, next.col) && map[next.row][next.col] != '#' && !seen[next.row][next.col]) {
          stack.push(next);
          dfs(stack, map, steps + 1, max_steps, dest, seen);
          stack.pop();
        } 
      }
    }
    seen[current.row][current.col] = false;
  }
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_24_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<std::string> map;
  while(std::getline(file, line)) {
    map.emplace_back(line);
  }

  // std::priority_queue<Point, std::vector<Point>, Comparator> pq;
  std::stack<Point> stack;
  std::vector<std::vector<bool>> seen(map.size(), std::vector<bool>(map[0].size(), false));
  
  Point current;
  current.row = 0;
  for (int col = 0; col < map[0].size(); col++) {
    if (map[0][col] == '.') {
      current.col = col;
      break;
    }
  }

  Point dest;
  dest.row = map.size() - 1;
  for (int col = 0; col < map[dest.row].size(); col++) {
    if (map[dest.row][col] == '.') {
      dest.col = col;
      break;
    }
  }

  stack.push(current);
  int max_steps = 0;
  dfs(stack, map, 0, max_steps, dest, seen);
  std::cout << max_steps << '\n';
  return 0;
}