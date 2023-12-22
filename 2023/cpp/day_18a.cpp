#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue> 
#include <string>
#include <unordered_set>
#include <vector>
#include <cassert> 

struct DigPlanStep {
  int direction;
  int length;
  std::string colour;
};

struct Point {
  int row;
  int col;

  Point(const int row, const int col) : row(row), col(col) {}

  Point operator + (const Point& p) const {
    return Point (p.row + row,  p.col + col);
  }

  bool operator == (const Point& p) const {
    return p.row == row && p.col == col;
  }
};

struct hasher {
  std::size_t operator() (const Point& position) const {
    return position.row + position.col;
  }
};

const std::vector<Point> motions ={
  Point(-1,0),
  Point(0,1),
  Point(1,0),
  Point(0, -1),
};

bool in_limits(const std::vector<std::vector<int>>& map, const int row, const int col) {
  return row >= 0 && row < map.size() && col >= 0 && col < map[0].size();
}

DigPlanStep parse (const std::string& line) {
  DigPlanStep dps; 
  if (line[0] == 'U') {
    dps.direction = 0;
  } else if (line[0] == 'R') {
    dps.direction = 1;
  } else if (line[0] == 'D') {
    dps.direction = 2;
  } else if (line[0] == 'L') {
    dps.direction = 3;
  } else {
    std::cout << "This should not happen" << '\n';
  }
  auto space_idx = line.find(' ', 2);
  dps.length = std::stoi(line.substr(2, space_idx - 2));
  dps.colour = line.substr(space_idx + 2, line.size() - 1 - space_idx - 2);
  return dps;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_18_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<DigPlanStep> plan;
  std::array<int, 4> borders = {{0,0,0,0}}; // ymin, xmax, ymax, xmin == N,E,S,W
  auto current = Point(0,0);
  while(std::getline(file, line)) {
    plan.emplace_back(parse(line));
    current.row = current.row + motions[plan.back().direction].row * plan.back().length;
    current.col = current.col + motions[plan.back().direction].col * plan.back().length;
    borders[0] = std::min(borders[0], current.row);
    borders[1] = std::max(borders[1], current.col);
    borders[2] = std::max(borders[2], current.row);
    borders[3] = std::min(borders[3], current.col);
  }
  
  std::vector<std::vector<char>> map (borders[2] - borders[0] + 1, std::vector<char>(borders[1] - borders[3] + 1, '.'));
  current = Point(-borders[0], -borders[3]);
  for (const auto& step : plan) {
    for (int  i = 0; i < step.length; i++) {
      current = current + motions[step.direction];
      map[current.row][current.col] = '#';
    }
  }

  // find point touching the LHS border
  for (int i = 0; i < map.size(); i++) {
    if (map[i][0] == '#') {
      current.row = i+1;
      current.col = 1;
      // assert (map[current.row][current.col] == '.');
      break;
    }
  }
  
  std::queue<Point> q;
  std::unordered_set<Point, hasher> seen;
  q.push(current);
  while(!q.empty()) {
    current = q.front();
    q.pop();
    if (seen.find(current) != seen.end()) {
      continue;
    }
    seen.insert(current);
    map[current.row][current.col] = '#';
    for (const auto& motion : motions) {
      const auto new_point = current + motion;
      if (map[new_point.row][new_point.col] == '.') {
        q.push(new_point);
      }
    }
  }
  
  int count = 0;
  for (const auto& row : map) {
    for (const auto ele : row) {
      if (ele == '#') count++;
    }
  }
  std::cout << count << '\n';
  return 0;
}