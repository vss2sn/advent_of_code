#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_set>
#include <set>
#include <vector>

// IDEA:
// Store all the start and end points (row, col) of each line segment that is part of the border.
// Store all the lines these points create as a set of horizontal and aother set of vertical lines.
// Moving from left to right, for each value of col in these point (not including the first).
// find the points at which the vertical line with the given col value intersects a horizontal line.
// Since the lines are sorted, the intersection points will be on consecutive lines.
// Given that the lines here are only vertical and horizontal, the vertical line will create rectangles to its left.
// Calculate the interal area of those rectangles, not including edges and store the vertical edges (variable used for this: `perimeter_lines`).
// Update the horizontal lines to start at the point at which the vertical line intersected them (to remove the rectangle whose area was calculated from the lagoon).
// Continue moving the line to the right and repeating the process.
// Add the perimeter (* 1 unit width) to the area.
// Note that until now the are of the verical lines of each rectangle have not been added.
// This is to prevent counting them twice in the rectangle to the left and the right of the line/
// Iterate over each of the verical lines what were part of the rectangles, get their length, substract from this length the number of points that lie on the perimeter.
// Multiply the calculated lenght by 1 unit and add it to the area..

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

  bool operator == (const LineSegment& ls) const {
    return start == ls.start && end == ls.end;
  }
};

// Given a veritcal line, remove all the points that exist on the perimiter of the lagoon and calculate the remaining length
// This is the length that will be added to the are (length units * 1 unit)
int length_without_perimeter(const LineSegment& vert_ls, const std::vector<LineSegment>& vertical_lines) {
  int length = vert_ls.length;
  int corner_count = 0; 
  int col = vert_ls.start.col;
  for (const auto& vl : vertical_lines) {
    if (vl.start.col == col) {
      if (vl.start.row == vert_ls.start.row && vl.end.row == vert_ls.end.row) {        
        return 0; // If the entire line is on the perimeter, return 
      }
      if (vl.start.row >= vert_ls.start.row && vl.end.row <= vert_ls.end.row) {
        if (vl.end.row == vert_ls.end.row || vl.start.row == vert_ls.start.row) {
          // The length passed in is already without the edge points, re-add them if the edge poinst should be part of the line
          corner_count++;
        }
        length -= (vl.length + 1);
      }
    }
  }
  length += corner_count;
  return length;
}

struct hasher {
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

DigPlanStep parse (const std::string& line) {
  DigPlanStep dps; 
  dps.direction = (line[line.size() - 2] - '0' + 1) % 4;  
  const auto space_idx = line.find(' ', 2);
  dps.length = std::stoi(line.substr(space_idx + 2 + 1, line.size() - 1 - space_idx - 2 - 1 - 1), 0, 16);
  dps.colour = line.substr(0, space_idx);
  return dps;
}

std::size_t process (const int col, std::vector<LineSegment>& horizontal_lines, std::unordered_set<LineSegment, hasher>& perimeter_lines) {
  std::sort(std::begin(horizontal_lines), std::end(horizontal_lines), [](const auto& hl1, const auto& hl2) { return hl1.start.row < hl2.start.row; } );
  std::vector<int> intersecting_line_indices;
  for (int i = 0; i < horizontal_lines.size(); i++) {
    if (horizontal_lines[i].vertical_line_intersect(col)) {
      intersecting_line_indices.push_back(i);
    } 
  }
  std::size_t area = 0;
  for (int i = 0; i < intersecting_line_indices.size(); i = i + 2) {
    auto& l1 = horizontal_lines[intersecting_line_indices[i]];
    auto& l2 = horizontal_lines[intersecting_line_indices[i+1]];
    // std::abs should not be required as sorted
    // do not include perimeter of box
    area +=  std::size_t(std::abs(l1.start.row - l2.start.row) - 1) * std::size_t(col - std::min(l1.start.col, l2.start.col) - 1); // Only find enclosed area
    l1.start.col = col;
    l2.start.col = col;
    auto line = LineSegment(Point(l1.start.row, l1.start.col), Point(l2.start.row , l2.start.col), std::abs(l2.start.row - l1.start.row) - 1); // Note that the edges are not being included in the length here
    if (line.start.row > line.end.row) std::swap(line.start, line.end);
    perimeter_lines.insert(line);
    line = LineSegment(Point(l1.end.row, col), Point(l2.end.row, col), std::abs(l2.start.row - l1.start.row) - 1); // Note that the edges are not being included in the length here
    if (line.start.row > line.end.row) std::swap(line.start, line.end);
    perimeter_lines.insert(line);
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
  std::vector<LineSegment> horizontal_lines;
  std::vector<LineSegment> vertical_lines;
  std::size_t perimeter = 0;
  auto current = Point(0,0);
  while(std::getline(file, line)) {
    const DigPlanStep dsp = parse(line);
    perimeter += dsp.length;
    LineSegment ls;
    ls.start = current;
    current.row = current.row + motions[dsp.direction].row * dsp.length;
    current.col = current.col + motions[dsp.direction].col * dsp.length;
    ls.end = current;
    // Always arrage in increasing order 
    if (ls.start.col > ls.end.col) std::swap(ls.start, ls.end);
    if (ls.start.row > ls.end.row) std::swap(ls.start, ls.end);
    ls.length = dsp.length;
    if (ls.start.row == ls.end.row) {
      horizontal_lines.push_back(ls);
    } else {
      vertical_lines.push_back(ls);
    }
  }

  std::set<int> cols; // sorted
  for (const auto& horizontal_line : horizontal_lines) {
    cols.insert(horizontal_line.start.col);
    cols.insert(horizontal_line.end.col);
  }

  std::unordered_set<LineSegment, hasher> perimeter_lines; // contain only the verital lines that were part of the perimeter of each rectangle
  std::size_t total = 0;
  for (const auto col : cols) {
    total += process(col, horizontal_lines, perimeter_lines); // Internal area of rectangles formed only. Does not include perimeter of rectangle
  }
  // std::cout << "Internal area: " << total << '\n'; 
  for (auto& perimeter_line : perimeter_lines) {
    total += length_without_perimeter(perimeter_line, vertical_lines); // Add the area occupied by the vertical lines
  }
  // std::cout << "Perimeter: " << perimeter << '\n';
  total += perimeter; // Addd the perimeter
  std::cout << total << '\n';
  return 0;
}