#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue> 
#include <string>
#include <unordered_set>
#include <set>
#include <vector>
#include <cassert> 

std::array<int, 4> borders = {{0,0,0,0}}; // ymin, xmax, ymax, xmin == N,E,S,W

struct DigPlanStep {
  int direction;
  int length;
  std::string colour;
};

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


struct LineSegment {
  Point start;
  Point end;
  int length;

  LineSegment(const Point& start = Point(), const Point& end = Point(),  const int length = 0) : start(start), end(end), length(length) {}
  bool vertical_line_intersect (const int col) const {
    if (col <= end.col && col > start.col) return true;
    if (col >= end.col && col < start.col) return true;
    return false;
  }

  bool horizontal_line_intersect (const int row) const {
    if (row <= end.row && row > start.row) return true;
    if (row >= end.row && row < start.row) return true;
    return false;
  }

  bool operator == (const LineSegment& ls) const {
    return start == ls.start && end == ls.end;
  }
};

int length_without_perimeter(const LineSegment& vert_ls, const std::vector<LineSegment>& vertical_lines, std::vector<std::vector<char>>& map) {
  int length = vert_ls.length;
  int temp_c = 0;
  // std::cout << "--" << '\n';
  // std::cout << vert_ls.start.row << ',' << vert_ls.start.col << ',' << vert_ls.end.row << ',' << vert_ls.start.col << '\n';
  // std::cout << "Length: " << length << '\n';
  int col = vert_ls.start.col;
  for (const auto& vl : vertical_lines) {
    if (vl.start.col == col) {
      if (vl.start.row == vert_ls.start.row && vl.end.row == vert_ls.end.row) {
        // for (int i = vl.start.row; i <= vl.end.row; i++) {
          // std::cout << i << ' ' << borders[0] << ' ' << i-borders[0] << '\n';
          // assert(map[i-borders[0]][col-borders[3]] == '#');
        // }
        return 0;
        break;
      }
      if (vl.start.row >= vert_ls.start.row && vl.end.row <= vert_ls.end.row) {
        // std::cout << vl.start.row << ',' << vl.start.col << ',' << vl.end.row << ',' << vl.start.col << '\n';
        if (vl.end.row == vert_ls.end.row || vl.start.row == vert_ls.start.row) temp_c++;
        // for (int i = vl.start.row; i <= vl.end.row; i++) {
          // std::cout << i << ' ' << borders[0] << ' ' << i-borders[0] << '\n';
          // std::cout << col << ' ' << borders[3] << ' ' << col-borders[3] << '\n';
          // std::cout << map.size() << ' ' << map[0].size() << '\n';
          // assert(map[i-borders[0]][col-borders[3]] == '#');
        // }
        // std::cout << "Subtracting: " << vl.length + 1 << '\n';
        assert (vl.length + 1 > 0);
        length -= (vl.length + 1);
      }
    }
  }
  // int count = 0;
  // for (int i = std::min(vert_ls.start.row, vert_ls.end.row); i <= std::max(vert_ls.start.row, vert_ls.end.row); i++) {
  //   if(map[i-borders[0]][col-borders[3]] != '#') {
  //     map[i-borders[0]][col-borders[3]] = '$';
  //     // std::cout << i-borders[0] << ' ' << col - borders[3]<< '\n';
  //     count++;
  //   }
  // }
  length += temp_c;
  // std::cout << "New Length: " << length << '\n';
  // std::cout << count << '\n';
  // if (count != length) {
  //   for (const auto& row : map) {
  //     for (const auto ele : row) {
  //       // if (ele == '#') count++;
  //       std::cout << ele;
  //     }
  //     std::cout << '\n';
  //   }
  // }
  // if (length == vert_ls.length+2)
  // assert(count == length);  
  // assert (length >= 0);
  return length;
}

struct hasher {
  std::size_t operator() (const Point& position) const {
    return position.row + position.col;
  }
  std::size_t operator() (const LineSegment& ls) const {
    return ls.start.row + ls.start.col;
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
  std::cout << dps.direction << ' ' << dps.length << ' ' << dps.colour << '\n';
  return dps;
}

DigPlanStep parse2 (const std::string& line) {
  DigPlanStep dps; 
  // std::cout  << line[line.size() - 2] <<  '\n';
  dps.direction = (line[line.size() - 2] - '0' + 1) % 4;  
  const auto space_idx = line.find(' ', 2);
  dps.length = std::stoi(line.substr(space_idx + 2 + 1, line.size() - 1 - space_idx - 2 - 1 - 1), 0, 16);
  dps.colour = line.substr(0, space_idx);
  std::cout << dps.direction << " | " << dps.length << " | " << dps.colour << '\n';
  return dps;
}

std::size_t process (const int col, std::vector<LineSegment>& horizontal_lines, std::unordered_set<LineSegment, hasher>& perimeter_lines, std::vector<std::vector<char>>& map) {
  std::sort(std::begin(horizontal_lines), std::end(horizontal_lines), [](const auto& hl1, const auto& hl2) { return hl1.start.row < hl2.start.row; } );
  std::vector<int> intersecting_line_indices;
  for (int i = 0; i < horizontal_lines.size(); i++) {
    // std::cout << horizontal_lines[i].start.row << ',' << horizontal_lines[i].start.col << '\n';
    if (horizontal_lines[i].vertical_line_intersect(col)) {
      intersecting_line_indices.push_back(i);
      // std::cout << "intersecting" << '\n';
    } 
  }
  std::size_t area = 0;
  // std::cout << "col: " << col << '\n';
  for (int i = 0; i < intersecting_line_indices.size(); i++) {
    const auto& l1 = horizontal_lines[intersecting_line_indices[i]];
    // std::cout << "intersects: " << l1.start.row << ","<<  l1.start.col << " ---> " << l1.end.row << "," << l1.end.col << '\n';
  }
  for (int i = 0; i < intersecting_line_indices.size(); i = i + 2) {
    // std::cout << "col: " << col << '\n';
    auto& l1 = horizontal_lines[intersecting_line_indices[i]];
    auto& l2 = horizontal_lines[intersecting_line_indices[i+1]];
    // std::cout << "l1: " << l1.start.row << ","<<  l1.start.col << " ---> " << l1.end.row << "," << l1.end.col << '\n';
    // std::cout << "l2: " << l2.start.row << ","<<  l2.start.col << " ---> " << l2.end.row << "," << l2.end.col << '\n';

    assert(l1.start.col == l2.start.col);
    // std::abs should not be required as sorted
    // do not include perimeter of box
    auto temp =  std::size_t(std::abs(l1.start.row - l2.start.row) - 1) * std::size_t(col - std::min(l1.start.col, l2.start.col) - 1); // Only find enclosed area
    // std::cout << "Internal area: " << temp << '\n';
    area += temp;
    // for (int row_temp = l1.start.row+1; row_temp <= l2.start.row-1; row_temp++) {
    //   // std::cout << l1.start.col+1 << ' ' << col-1 << '\n';
    //   for (int col_temp = l1.start.col+1; col_temp <= col-1; col_temp++) {
    //     assert(map[row_temp-borders[0]][col_temp-borders[3]] == '.');
    //     map[row_temp-borders[0]][col_temp-borders[3]] = '@';
    //   }  
    // }
    l1.start.col = col;
    l2.start.col = col;
    // perimeter_lines.insert(LineSegment(Point(l1.start.row,     l1.start.col), Point(l1.end.row,       col),          col - std::min(l1.start.col, l2.start.col) + 1));
    // perimeter_lines.insert(LineSegment(Point(l2.start.row,     l2.start.col), Point(l2.end.row,       col),          col - std::min(l1.start.col, l2.start.col) + 1));
    auto line = LineSegment(Point(l1.start.row, l1.start.col), Point(l2.start.row , l2.start.col), std::abs(l2.start.row - l1.start.row) - 1);
    if (line.start.row > line.end.row) std::swap(line.start, line.end);
    perimeter_lines.insert(line);
    line = LineSegment(Point(l1.end.row,   col),          Point(l2.end.row,   col),          std::abs(l2.start.row - l1.start.row) - 1);
    if (line.start.row > line.end.row) std::swap(line.start, line.end);
    perimeter_lines.insert(line);
    
    // std::cout << (col - std::min(l1.start.col, l2.start.col) + 1) * 2 << ' ' <<  (std::abs(l2.start.row - l1.start.row) - 1) * 2 << '\n';
  }
  // Erase lines that are points
  for (int i = intersecting_line_indices.size() - 1 ; i >= 0; i--) {
    auto& l1 = horizontal_lines[intersecting_line_indices[i]];
    if (l1.start.col == l1.end.col) {
      horizontal_lines.erase(std::begin(horizontal_lines) + intersecting_line_indices[i]);
    } 
  }
  return area;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_18_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<DigPlanStep> plan;
  std::vector<LineSegment> horizontal_lines;
  std::vector<LineSegment> vertical_lines;

  std::size_t perimeter = 0;
  auto current = Point(0,0);
  while(std::getline(file, line)) {
    plan.emplace_back(parse2(line));
    perimeter += plan.back().length;
    LineSegment ls;
    ls.start = current;
    current.row = current.row + motions[plan.back().direction].row * plan.back().length;
    current.col = current.col + motions[plan.back().direction].col * plan.back().length;
    ls.end = current;
    if (ls.start.col > ls.end.col) std::swap(ls.start, ls.end);
    if (ls.start.row > ls.end.row) std::swap(ls.start, ls.end);
    ls.length = plan.back().length;

    if (ls.start.row == ls.end.row) {
      horizontal_lines.push_back(ls);
    } else {
      vertical_lines.push_back(ls);
    }
    // std::cout << current.row << ' ' << current.col << '\n';
    borders[0] = std::min(borders[0], current.row);
    borders[1] = std::max(borders[1], current.col);
    borders[2] = std::max(borders[2], current.row);
    borders[3] = std::min(borders[3], current.col);
  }
  // perimeter -= 1;
  // for (const auto& ele : borders) {
  //   std::cout << ele << ' ' ;
  // }
  // std::cout << '\n';

  std::vector<std::vector<char>> map;// (borders[2] - borders[0] + 1, std::vector<char>(borders[1] - borders[3] + 1, '.'));
  // current = Point(-borders[0], -borders[3]);
  // for (const auto& step : plan) {
  //   for (int  i = 0; i < step.length; i++) {
  //     current = current + motions[step.direction];
  //     // std::cout << current.row << ' ' << current.col << '\n';
  //     // map[current.row][current.col] = '#';
  //   }
  // }

  
  std::set<int> cols;
  for (const auto& horizontal_line : horizontal_lines) {
    cols.insert(horizontal_line.start.col);
    cols.insert(horizontal_line.end.col);
  }

  std::unordered_set<LineSegment, hasher> perimeter_lines;
  std::size_t total = 0;
  for (const auto col : cols) {
    // std::cout << "---------------------" << '\n';
    // std::cout << "Col: " << col << '\n';
    const auto area = process(col, horizontal_lines, perimeter_lines, map);
    std::cout << area << '\n';
    total += area;
    // std::cout << "---------------------" << '\n';
  }

  std::cout << "Internal area: " << total << '\n';
  for (auto& perimeter_line : perimeter_lines) {
    // std::cout << perimeter_line.length << '\n';
    total += length_without_perimeter(perimeter_line, vertical_lines, map);
  }
  std::cout << "perimeter: " << perimeter << '\n';
  total += perimeter;
  std::cout << total << '\n';


  
  // std::cout << "---------------------" << '\n';
  int count = 0;
  // for (const auto& row : map) {
  //   for (const auto ele : row) {
  //     // if (ele == '#') count++;
  //     std::cout << ele;
  //   }
  //   std::cout << '\n';
  // }
  // std::cout << count << '\n';

  return 0;
}