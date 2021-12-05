#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Point {
  int row;
  int col;
  Point(const int row, const int col) : row(row), col(col) {}
  Point operator + (const Point& p) const {
    return Point(row + p.row, col + p.col);
  }
  bool operator != (const Point& p) const {
    return row != p.row ||  col != p.col;
  }
  bool operator == (const Point& p) const {
    return row == p.row &&  col == p.col;
  }
  friend std::ostream& operator << (std::ostream& os, const Point& p);
};

std::ostream& operator << (std::ostream& os, const Point& p) {
  os << "(" << p.row << ", " << p.col << ")";
  return os;
}

struct Point_hash {
  std::size_t operator() (const Point& p) const {
    return p.row ^ p.col;
  }
};

std::tuple<Point, Point> convert_to_nums (const std::string& line) {
  std::size_t start = 0;
  const std::string delimiters = " ,";
  std::size_t end = line.find_first_of(delimiters);
  std::vector<int> vals;
  int count = 0;
  while (end != std::string::npos) {
    if (count !=2 ) { // Account for ->
      vals.push_back(std::stoi(line.substr(start, end - start)));
    }
    count++;
    start = end + 1;
    end  = line.find_first_of(delimiters, start);
  }
  vals.push_back(std::stoi(line.substr(start, end - start)));
  return {Point(vals[0], vals[1]), Point(vals[2], vals[3])};
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_05_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::unordered_map<Point, int, Point_hash> vents;
  while(std::getline(file, line)) {
    const auto [start, end] = convert_to_nums(line);
    if (!(start.row == end.row || start.col == end.col)) {
      continue;
    }
    auto delta = Point(0, 0);
    if (start.row == end.row) {
      delta.col = std::abs(start.col - end.col) / (start.col - end.col);
    } else if  (start.col == end.col) {
      delta.row = std::abs(start.row - end.row) / (start.row - end.row);
    } else {
      // Not reached due to continue
    }
    auto temp = end;
    while (temp != start) {
      if (vents.find(temp) == vents.end()) {
        vents[temp] = 0;
      }
      vents[temp]++;
      temp = temp + delta;
    }
    if (vents.find(temp) == vents.end()) {
      vents[temp] = 0;
    }
    vents[temp]++;
  }
  int n = 0;
  for (const auto& [p, count] : vents) {
    if (count > 1) {
      n++;
    }
  }
  std::cout << n << '\n';
  return 0;
}
