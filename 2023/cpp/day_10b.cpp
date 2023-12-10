#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <regex>
#include <queue>
#include <string>
#include <unordered_set>
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

struct hasher {
  std::size_t operator () (const Point& p) const {
    return p.row * 10000 + p.col;
  }
};

bool in_limits(const Point& p, const std::vector<std::string>& map) {
  return p.row >= 0 && p.row < map.size() && p.col >= 0 && p.col <  map[p.row].size();
}

int bfs(const Point& p, std::vector<std::string>& map, std::vector<std::vector<int>>& groups,  const std::vector<std::vector<bool>>& is_part_of_loop, const int value) {
  const std::vector<Point> motions {
    Point(-1, 0),
    Point(0, 1),
    Point(1, 0),
    Point(0, -1)
  };
  std::queue<Point> q;
  q.push(p);
  int count = 0;
  std::unordered_set<Point, hasher> seen;
  while(!q.empty()) {
    const auto current = q.front();
    q.pop();
    if (is_part_of_loop[current.row][current.col]) continue;
    if (seen.find(current) != seen.end()) continue;
    seen.insert(current);
    count++;
    groups[current.row][current.col] = value; 
    for (const auto& motion : motions) {
      Point new_point;
      new_point.row = current.row + motion.row;
      new_point.col = current.col + motion.col;
      if (in_limits(new_point, map) && groups[new_point.row][new_point.col] == -1) {
        q.push(new_point);
      }
    }
  }
  return count;
}

std::pair<Point, int> get_next_point(const Point& point, const Point& previous_point, const std::vector<std::string>& map, const std::vector<std::vector<std::array<int, 4>>>& direction_map) {
  // std::cout << point.row << ", " << point.col << '\n';
  std::vector<Point> next_points;
  const auto& point_directions = direction_map[point.row][point.col];
  if (point_directions[0]) {
    Point next_point;
    next_point.row = point.row - 1; 
    next_point.col = point.col; 
    if (in_limits(next_point, map) && direction_map[next_point.row][next_point.col][(0 + 2) % 4]  && next_point != previous_point) {
      return {next_point, 0};
    }
  }
  if (point_directions[1]) {
    Point next_point;
    next_point.row = point.row; 
    next_point.col = point.col + 1;
    if (in_limits(next_point, map) && direction_map[next_point.row][next_point.col][(1 + 2) % 4]  && next_point != previous_point) {
      return {next_point, 1};
    }
  }
  if (point_directions[2]) {
    Point next_point;
    next_point.row = point.row + 1; 
    next_point.col = point.col;
    if (in_limits(next_point, map) && direction_map[next_point.row][next_point.col][(2 + 2) % 4]  && next_point != previous_point) {
      return {next_point, 2};
    }
  }
  if (point_directions[3]) {
    Point next_point;
    next_point.row = point.row; 
    next_point.col = point.col - 1; 
    if (in_limits(next_point, map) && direction_map[next_point.row][next_point.col][(3 + 2) % 4]  && next_point != previous_point) {
      return {next_point, 3};
    }
  }
  Point p;
  p.row = -1;
  p.col = -1;
  std::cout << "This should not happen" << '\n';
  return {p, 0};
}

struct PS {
  Point point;
  int step;
  int direction;

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
    std::cout << line << '\n';
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
  current_ps.direction = 1; // Update direction of start;
  current_ps.step = 0;
  std::vector<PS> loop;
  std::vector<std::vector<bool>> is_part_of_loop(map.size(), std::vector<bool>(map[0].size(), false));
  while(current_ps.point != S || current_ps.step == 0) {
    is_part_of_loop[current_ps.point.row][current_ps.point.col] = true;
    const auto [new_current_point, direction] = get_next_point(current_ps.point, previous, map, direction_map);
    current_ps.direction = direction;
    loop.push_back(current_ps);
    previous = current_ps.point;
    current_ps.point = new_current_point;
    current_ps.step++;
  }

  const std::vector<Point> point_to_the_right_of_direction {
    Point(0, 1),
    Point(1, 0),
    Point(0, -1),
    Point(-1, 0)
  };

  std::vector<std::vector<int>> groups(map.size(), std::vector<int>(map[0].size(), -1));
  int value = 1;
  std::unordered_map<int, int> counts;
  for (int row = 0; row < groups.size(); row++) {
    for (int col = 0; col < groups[0].size(); col++) {
      if (groups[row][col] == -1) {
        const auto p = Point(row, col);
        counts[value] = bfs(p, map, groups, is_part_of_loop, value);
        // std::cout << value << ": " << counts[value] << '\n'; 
        value++;
      }
    } 
  }

  // for (const auto& line : groups) {
  //   for (const auto& ele : line) {
  //     std::cout << ele << ' ';
  //   }
  //   std::cout << '\n';
  // }
  std::unordered_set<int> groups_to_the_right_of_loop;
  for (int idx = 1; idx < loop.size(); idx++) {
    const auto& p = loop[idx];
    const auto deltas = std::vector<Point>{
      Point(loop[idx + 1].point.row - loop[idx].point.row, loop[idx + 1].point.col - loop[idx].point.col),
      Point(loop[idx].point.row - loop[idx - 1].point.row, loop[idx].point.col - loop[idx - 1].point.col)
    };
    for (const auto& delta : deltas) {
      int direction = -1;
      if (delta == Point(-1,0)) direction = 0;
      else if (delta == Point(0,1)) direction = 1;
      else if (delta == Point(1,0)) direction = 2;
      else if (delta == Point(0,-1)) direction = 3;
      const auto delta_to_get_right_point = point_to_the_right_of_direction[direction];
      const auto point_to_right = Point(p.point.row + delta_to_get_right_point.row, p.point.col + delta_to_get_right_point.col);
      if(!in_limits(point_to_right, map)) continue;
      if (groups[point_to_right.row][point_to_right.col] != -1) {
        if(groups[point_to_right.row][point_to_right.col] == 5606) {
          std::cout << "direction: " << direction << '\n';
          std::cout << "point: " << p.point.row << ' ' << p.point.col << '\n';
          std::cout << "Value at point: " << map[p.point.row][p.point.col]   << '\n';
          std::cout << "Direction at point  : " << p.direction << '\n';
          std::cout << "Direction map at point: " << '\n';
          for (int j = 0; j < 4; j++) {
            std::cout << direction_map[p.point.row][p.point.col][j] << ' ';
          }
          std::cout << '\n';
          std::cout << "delta: " << delta.row << ' ' << delta.col << '\n';
          std::cout << "point_to_right: " << point_to_right.row << ' ' << point_to_right.col << '\n';
          // exit(0);
        }
        groups_to_the_right_of_loop.insert(groups[point_to_right.row][point_to_right.col]);
      }
    }
  }
  int total = 0;
  for (const auto group_to_the_right_of_loop : groups_to_the_right_of_loop) {
    // std::cout << "Groud id in loop: " << group_to_the_right_of_loop << ": " << counts[group_to_the_right_of_loop] << '\n';
    total += counts[group_to_the_right_of_loop];
  }
  int total_2 = 0;
  for(const auto& [group_id, count] : counts) {
    if (groups_to_the_right_of_loop.find(group_id) == groups_to_the_right_of_loop.end()) {
      total_2 += counts[group_id];
      // if (counts[group_id] > 0) std::cout << "Groud id out loop: " << group_id << ": " << counts[group_id] << '\n';
    }
  }

  for (int row = 0; row < groups.size(); row++) {
    for (int col = 0; col < groups[0].size(); col++) {
      if (groups[row][col] != -1) {
        if(groups_to_the_right_of_loop.find(groups[row][col]) != groups_to_the_right_of_loop.end()) {
          map[row][col] = 'O';
        } else {
           map[row][col] = 'I';
        }
      }
      if (is_part_of_loop[row][col]) {
        PS ps;
        ps.point = Point(row, col);
        ps = *std::find(loop.begin(), loop.end(), ps);
        if (ps.direction == 0) {
          map[row][col] = '^';
        } else if (ps.direction == 1) {
          map[row][col] = '>';
        } else if (ps.direction == 2) {
          map[row][col] = 'v';
        } else if (ps.direction == 3){
          map[row][col] = '<';
        }
      }
    }  
  }

  for (const auto& row : map) {
    for (const auto col : row) {
      std::cout << col;
    }
    std::cout << '\n';
  }

  // TODO: add check for which one is enclosed by checking which group contains the edge group ids
  bool groups_to_the_right_of_loop_are_enclosed = true;
  for (const auto& row : groups) {
    if (groups_to_the_right_of_loop.find(row[0]) != groups_to_the_right_of_loop.end()) {
  std::cout << total << '\n';
      std::cout << total_2 << '\n';
      return 0;
    }
    if (groups_to_the_right_of_loop.find(row[row.size() - 1]) != groups_to_the_right_of_loop.end()) {
  std::cout << total << '\n';

      std::cout << total_2 << '\n';
      return 0;
    }
  }
  std::cout << total << '\n';

  std::cout << total_2 << '\n';
  return 0;
}
