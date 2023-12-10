#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <regex>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

// Idea (implemented):
// using the pipe as a boundary, fill each enclosed area giving each area and id and counting the number of points in the area
// traverse the pipe and cache the area_ids that are to the right of the directions of travel (plural as a turn might have 2 directions of travel)
// check if the points on the boundary are to the right or the left of the pipe
// If left, find the total count of the cached area ids, else the ids not in the cache.

// Another idea would be to expand area:
// Increase the size of the map to 2 * current_size - 1
// if x, y was the pipe, let 2 * row , 2 * col  be the pipe, and fill in (2*row+1, 2*col) and (2*row, 2*col+1) but not (2*row+1, 2*col+1) 
// this ensures that each point that is not enclosed by the loop will be connected to the outside edges
// flood fill from the outside edges and find the number of points that are neither filled nor parr of the pipe.
// Link to possible implementatio in python:
// https://www.reddit.com/r/adventofcode/comments/18evyu9/comment/kcs2oa9/?utm_source=share&utm_medium=web2x&context=3

// Another idea would be iterate over the map and check the number of the pipe is encountered in a row:
// If odd the the area is enclosed, if even then it is not
// Would need to handle points where the pipe is moving horizontally separately, maybe can rotate the map 90 degrees and repeat 

// Another idea is to use Pick's theorm and the Shoelace formula

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

const std::vector<Point> deltas {
  Point(-1, 0),
  Point(0, 1),
  Point(1, 0),
  Point(0, -1)
};

bool in_limits(const Point& p, const std::vector<std::string>& map) {
  return p.row >= 0 && p.row < map.size() && p.col >= 0 && p.col <  map[p.row].size();
}

int bfs(const Point& p, std::vector<std::string>& map, std::vector<std::vector<int>>& groups,  const std::vector<std::vector<bool>>& is_part_of_loop, const int group_id) {
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
    groups[current.row][current.col] = group_id; 
    for (const auto& delta : deltas) {
      Point new_point;
      new_point.row = current.row + delta.row;
      new_point.col = current.col + delta.col;
      if (in_limits(new_point, map) && groups[new_point.row][new_point.col] == -1) {
        q.push(new_point);
      }
    }
  }
  return count;
}

std::pair<Point, int> get_next_point(const Point& point, const Point& previous_point, const std::vector<std::string>& map, const std::vector<std::vector<std::array<int, 4>>>& direction_map) {
  std::vector<Point> next_points;
  const auto& point_directions = direction_map[point.row][point.col];
  for (int i = 0; i < 4; i++) {
    if (point_directions[i]) {
      const Point next_point(point.row + deltas[i].row, point.col + deltas[i].col);
      if (in_limits(next_point, map) && direction_map[next_point.row][next_point.col][(i + 2) % 4]  && next_point != previous_point) {
        return {next_point, i};
      }
    }
  }  
  Point p(-1, -1);
  std::cout << "This should not happen" << '\n';
  exit(0);
  return {p, 0};
}

struct PSD {
  Point point;
  int step; // No longer necessary
  int direction; // For debug only

  bool operator == (const PSD& psd) const {
    return psd.point == point;
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
  std::vector<std::vector<std::array<int, 4>>> direction_map; // Travel allowed in these directions (Order is NESW)
  Point S;
  Point previous(-1,-1);
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

  PSD current_psd;
  current_psd.point = S;
  current_psd.direction = 1; // Update direction of start, though not used
  current_psd.step = 0;
  std::vector<PSD> loop;
  std::vector<std::vector<bool>> is_part_of_loop(map.size(), std::vector<bool>(map[0].size(), false));
  while(current_psd.point != S || current_psd.step == 0) {
    is_part_of_loop[current_psd.point.row][current_psd.point.col] = true;
    const auto [new_current_point, direction] = get_next_point(current_psd.point, previous, map, direction_map);
    current_psd.direction = direction;
    loop.push_back(current_psd);
    previous = current_psd.point;
    current_psd.point = new_current_point;
    current_psd.step++;
  }

  const std::vector<Point> point_to_the_right_of_direction {
    Point(0, 1), // N->E
    Point(1, 0), // E->S
    Point(0, -1), // S->W
    Point(-1, 0) // W->N
  };

  std::vector<std::vector<int>> groups(map.size(), std::vector<int>(map[0].size(), -1)); // Fill enclosed area
  int group_id = 1;
  std::unordered_map<int, int> counts;
  for (int row = 0; row < groups.size(); row++) {
    for (int col = 0; col < groups[0].size(); col++) {
      if (groups[row][col] == -1) {
        const auto p = Point(row, col);
        counts[group_id] = bfs(p, map, groups, is_part_of_loop, group_id);
        // std::cout << group_id << ": " << counts[group_id] << '\n'; 
        group_id++;
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
    const auto deltas = std::vector<Point>{ // Change in location from previous to current and current to next; used to get direction. To refactor/optimise.
      Point(loop[idx + 1].point.row - loop[idx].point.row, loop[idx + 1].point.col - loop[idx].point.col),
      Point(loop[idx].point.row - loop[idx - 1].point.row, loop[idx].point.col - loop[idx - 1].point.col)
    };
    for (const auto& delta : deltas) {
      int direction = -1;
      // Change in location to direction (0123 -> NESW)
      if (delta == Point(-1,0)) direction = 0; 
      else if (delta == Point(0,1)) direction = 1;
      else if (delta == Point(1,0)) direction = 2;
      else if (delta == Point(0,-1)) direction = 3;
      // else {std::cout << "This should not happen"; exit(0);}
      const auto delta_to_get_right_point = point_to_the_right_of_direction[direction];
      const auto point_to_right = Point(p.point.row + delta_to_get_right_point.row, p.point.col + delta_to_get_right_point.col);
      if(!in_limits(point_to_right, map)) continue;
      if (groups[point_to_right.row][point_to_right.col] != -1) {
        groups_to_the_right_of_loop.insert(groups[point_to_right.row][point_to_right.col]);
      }
    }
  }
  int total_right = 0;
  for (const auto group_to_the_right_of_loop : groups_to_the_right_of_loop) {
    // std::cout << "Groud id in loop: " << group_to_the_right_of_loop << ": " << counts[group_to_the_right_of_loop] << '\n';
    total_right += counts[group_to_the_right_of_loop];
  }
  int total_left = 0;
  for(const auto& [group_id, count] : counts) {
    if (groups_to_the_right_of_loop.find(group_id) == groups_to_the_right_of_loop.end()) {
      total_left += counts[group_id];
      // if (counts[group_id] > 0) std::cout << "Groud id out loop: " << group_id << ": " << counts[group_id] << '\n';
    }
  }

  // DEBUG: 
  // Update the map based on direction of points and whether inside or outside the loop using 'T' & 'O'
  // for (int row = 0; row < groups.size(); row++) {
  //   for (int col = 0; col < groups[0].size(); col++) {
  //     if (groups[row][col] != -1) {
  //       if(groups_to_the_right_of_loop.find(groups[row][col]) != groups_to_the_right_of_loop.end()) {
  //         map[row][col] = 'O';
  //       } else {
  //          map[row][col] = 'I';
  //       }
  //     }
  //     if (is_part_of_loop[row][col]) {
  //       PSD psd;
  //       psd.point = Point(row, col);
  //       psd = *std::find(loop.begin(), loop.end(), psd);
  //       if (ps.direction == 0) {
  //         map[row][col] = '^';
  //       } else if (ps.direction == 1) {
  //         map[row][col] = '>';
  //       } else if (ps.direction == 2) {
  //         map[row][col] = 'v';
  //       } else if (ps.direction == 3){
  //         map[row][col] = '<';
  //       }
  //     }
  //   }  
  // }
  // for (const auto& row : map) {
  //   for (const auto col : row) {
  //     std::cout << col;
  //   }
  //   std::cout << '\n';
  // }

  // Check whether groups on right are on boundary
  for (const auto& row : groups) {
    if (groups_to_the_right_of_loop.find(row[0]) != groups_to_the_right_of_loop.end()) {
      std::cout << total_left << '\n';
      return 0;
    }
    if (groups_to_the_right_of_loop.find(row[row.size() - 1]) != groups_to_the_right_of_loop.end()) {
      std::cout << total_left << '\n';
      return 0;
    }
  }
  std::cout << total_right << '\n';
  return 0;
}
