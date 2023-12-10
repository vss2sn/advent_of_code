#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

struct Point {
  int row;
  int col;

  bool operator ==  (const Point& p) const {
    return p.row == row && p.col == col;
  }
  bool operator !=  (const Point& p) const {
    return !(p == *this);
  }

  Point(const int row = 0, const int col = 0) : row(row), col(col) {}
};

bool in_limits(const Point& p, const std::vector<std::string>& map) {
  return p.row >= 0 && p.row < map.size() && p.col >= 0 && p.col <  map[p.row].size();
}

const std::vector<Point> deltas {
  Point(-1, 0),
  Point(0, 1),
  Point(1, 0),
  Point(0, -1)
};

Point get_next_point(const Point& point, const Point& previous_point, const std::vector<std::string>& map, const std::vector<std::vector<std::array<int, 4>>>& direction_map) {
  std::vector<Point> next_points;
  const auto& point_directions = direction_map[point.row][point.col];
  for (int i = 0; i < 4; i++) {
    if (point_directions[i]) {
      const Point next_point(point.row + deltas[i].row, point.col + deltas[i].col);
      if (in_limits(next_point, map) && direction_map[next_point.row][next_point.col][(i + 2) % 4]  && next_point != previous_point) {
        return next_point;
      }
    }
  }  
  Point p(-1, -1);
  std::cout << "This should not happen" << '\n';
  exit(0);
  return p;
}


struct PS {
  Point point;
  int step;

  bool operator == (const PS ps) const {
    return ps.point == point;
  }
};

int main(int argc, char * argv[]) {
  std::string input = "../input/day_10_input";
  if (argc > 1) {
    input = argv[1];
  }

  // N, E, S ,W
  std::string line;
  std::fstream file(input);
  std::vector<std::string> map;
  std::vector<std::vector<std::array<int, 4>>> direction_map;
  Point S;
  Point previous;
  previous.row = -1;
  previous.col = -1;
  while(std::getline(file, line)) {
    map.push_back(line);
    direction_map.push_back({});
    direction_map.back().reserve(line.size()  * 4);
    for (const char c : line) {
      if (c == '|') {
        direction_map.back().push_back({{true, false, true, false}});
      } else  if (c == '-') {
        direction_map.back().push_back({{false, true, false, true}});
      } else  if (c == 'L') {
        direction_map.back().push_back({{true, true, false, false}});
      } else  if (c == 'J') {
        direction_map.back().push_back({{true, false, false, true}});
      } else  if (c == '7') {
        direction_map.back().push_back({{false, false, true, true}});
      } else  if (c == 'F') {
        direction_map.back().push_back({{false, true, true, false}});
      } else  if (c == '.') {
        direction_map.back().push_back({{false, false, false, false}});
      } else  if (c == 'S') {
        // std::cout << "S set " << '\n';
        direction_map.back().push_back({{true, true, true, true}});
        S.row = direction_map.size() - 1;
        for (int i = 0 ; i < line.size(); i++) {
          if (line[i] == 'S') {
            S.col = i;
            break;
          }
        }
      }
    }
  }

  PS current_ps;
  current_ps.point = S;
  current_ps.step = 0;
  // queue.push(current_ps);
  std::vector<PS> visited;
  while(current_ps.point != S || current_ps.step == 0) {
    visited.push_back(current_ps);
    const auto new_current_point = get_next_point(current_ps.point, previous, map, direction_map);
    previous = current_ps.point;
    current_ps.point = new_current_point;
    current_ps.step++;
  }
  std::cout << visited[visited.size()/2].step << '\n';

  return 0;
}
