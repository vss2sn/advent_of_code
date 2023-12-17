#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue> 
#include <string>
#include <unordered_set>
#include <vector>

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

struct Beam {
  Point head;
  int direction;

  Beam(const Point& head, const int direction) : head(head), direction(direction) {}

  bool operator == (const Beam& b) const {
    return head == b.head && direction == b.direction;
  }
};

struct hasher {
  std::size_t operator() (const Point& p) const {
    return std::max(p.row + p.col, 0);
  }
    std::size_t operator() (const Beam& b) const {
    return std::max(b.head.row + b.head.col, 0);
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

std::size_t fire_beam(const std::vector<std::string>& map, const int start_row, const int start_col, const int direction) {
  std::vector<std::vector<int>> counter (map.size(),  std::vector<int>(map[0].size(), 0));
  std::queue<Beam> beams;
  beams.emplace(Point(start_row, start_col), direction);
  std::unordered_set<Beam, hasher> seen;
  while(!beams.empty()) {
    const auto beam = beams.front();
    auto current = beam.head;
    beams.pop();
    if (seen.find(beam) != seen.end()) {
      continue;
    }
    seen.insert(beam);
    if (in_limits(map, current.row, current.col))counter[current.row][current.col]++;
    current = current + motions[beam.direction];
    while (in_limits(map, current.row, current.col) &&
          (map[current.row][current.col] == '.' || \
           ((map[current.row][current.col] == '-') && (beam.direction == 1 || beam.direction == 3)) ||
           ((map[current.row][current.col] == '|') && (beam.direction == 0 || beam.direction == 2)))
       ) {
      counter[current.row][current.col]++;
      current = current + motions[beam.direction];
    }
    if (!in_limits(map, current.row, current.col)) {
      continue;
    }
    counter[current.row][current.col]++;
    if (map[current.row][current.col] == '|' && (beam.direction == 1 || beam.direction == 3)) {
      beams.push(Beam(current, 0));
      beams.push(Beam(current, 2));
    } else if (map[current.row][current.col] == '-' && (beam.direction == 0 || beam.direction == 2)) {
      beams.push(Beam(current, 1));
      beams.push(Beam(current, 3));
    } else if (map[current.row][current.col] == '\\') {
      if (beam.direction == 1) beams.push(Beam(current, 2));
      else if (beam.direction == 2) beams.push(Beam(current, 1));
      else if (beam.direction == 0) beams.push(Beam(current, 3));
      else if (beam.direction == 3) beams.push(Beam(current, 0));
      else {
        std::cout << __LINE__ << " This should not happen" << '\n';
        exit(0);
      }
    } else if (map[current.row][current.col] == '/') {
      if (beam.direction == 1) beams.push(Beam(current, 0));
      else if (beam.direction == 0) beams.push(Beam(current, 1));
      else if (beam.direction == 2) beams.push(Beam(current, 3));
      else if (beam.direction == 3) beams.push(Beam(current, 2));
      else {
        std::cout << __LINE__ << " This should not happen" << '\n';
        exit(0);
      }
    } 
  }
  std::size_t total = 0;
  for (const auto& row : counter) {
    for (const auto ele : row) {
      total += (ele != 0);
    }
  }
  return total;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_16_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<std::string> map;
  while(std::getline(file, line)) {
    map.push_back(line);
  }


  // Start ourtside the map
  // Can modify code to ensure that beam state in queue is state immediately after turning/splitting,  
  // in which case can start with 0,0 and remove some of the checks for in_limits 
  // as they are only useful  for the 1st point
  std::size_t total = 0;
  for (int start_row = 0; start_row < map.size(); start_row++) {
    total = std::max(total, fire_beam(map, start_row, -1, 1));
    total = std::max(total, fire_beam(map, start_row, map[0].size(), 3));
  }
  for (int start_col = 0; start_col < map[0].size(); start_col++) {
    total = std::max(total, fire_beam(map, -1, start_col, 2));
    total = std::max(total, fire_beam(map, map.size(), start_col, 0));
  }
  std::cout << total <<  '\n';
  return 0;
}