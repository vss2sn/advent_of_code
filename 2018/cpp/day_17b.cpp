#include <cassert>
#include <queue>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cassert>
#include <unordered_map>
#include <unordered_set>

#include <chrono>
#include <thread>

using namespace std::literals::chrono_literals;

struct Point;
struct point_hash;
using Scan = std::unordered_map<Point, char, point_hash>;

struct Point {
  int row, col;
  Point (const int row, const int col) : row(row), col(col) {}
  bool operator == (const Point& p) const {
    return p.row == row && p.col ==col;
  }

  friend std::ostream& operator << (std::ostream& os, const Point& p);
};

std::ostream& operator << (std::ostream& os, const Point& p) {
  os << "(" << p.row << ", " << p.col << ")";
  return os;
}

struct point_hash {
  std::size_t operator () (const Point& p) const {
    return (p.row << 4) + p.col;
  }
};

inline bool inScan(const Point& p, const Scan& scan) {
  return scan.find(p) != scan.end();
}

inline bool inLimits(const Point& p, const Point& min, const Point& max) {
  return p.row <= max.row && p.row >= min.row && p.col <= max.col && p.col >= min.col;
}

bool check_horizontal(const Point& p, const Point& delta, Scan& scan, const Point& min, const Point& max) {
  const auto new_point = Point(p.row, p.col + delta.col);
  if (!inLimits(p, min, max)) return false;
  else if (!inScan(new_point, scan)) return false;
  else if (scan[new_point] == '#') return true;
  else if (scan[new_point] == '~') return check_horizontal(new_point, delta, scan, min, max);
  return false;
}

std::tuple<bool, std::vector<Point>> check_horizontal_with_vec(const Point& p, const Point& delta, Scan& scan, const Point& min, const Point& max, std::vector<Point>& v) {
  const auto new_point = Point(p.row, p.col + delta.col);
  if (!inLimits(p, min, max)) return {false, v};
  v.push_back(p);
  if (!inScan(new_point, scan)) return {false, v};
  else if (scan[new_point] == '#') return {true, v};
  else if (scan[new_point] == '~') return check_horizontal_with_vec(new_point, delta, scan, min, max, v);
  return {false, v};
}

bool expandPoint(const Point& p, Scan& scan, const Point& min, const Point& max) {
  scan[p] = '~';
  auto below = Point(p.row+1, p.col);
  auto left = Point(p.row, p.col-1);
  auto right = Point(p.row, p.col+1);

  if (!inLimits(below, min, max)) {
    return false;
  }

  bool supported_below = false;
  bool supported_left = false;
  bool supported_right = false;
  if (inScan(below, scan) && scan[below] == '#') {
    supported_below = true;
    if (!inScan(left, scan) ) {
      supported_left = expandPoint(left, scan, min, max);
      scan[left] = '~';
    } else {
      supported_left = true;
    }
    if (!inScan(right, scan)) {
      supported_right = expandPoint(right, scan, min, max);
      scan[right] = '~';
    } else {
      supported_right = true;
    }
    return supported_below && supported_left && supported_right;
  }

  if (!inScan(below, scan)) {
    supported_below = expandPoint(below, scan ,min, max);
  }
  if (inScan(below, scan) && scan[below] == '~') {
    if (check_horizontal(below, Point(0, 1), scan, min, max) && check_horizontal(below, Point(0, -1), scan, min, max)) {
      if (!inScan(left, scan) ) {
        supported_left = expandPoint(left, scan, min, max);
      }
      if (!inScan(right, scan)) {
        supported_right = expandPoint(right, scan, min, max);
      }
      return true;
    }
  }
  return false;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_17_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::ifstream file(input);
  std::unordered_map<Point, char, point_hash> scan;
  std::regex pattern(R"(([xy])=([0-9]+), ([xy])=([0-9]+)..([0-9]+))");
  int min_col = std::numeric_limits<int>::max();
  int min_row = std::numeric_limits<int>::max();
  int max_col = 0;
  int max_row = 0;
  int min_row_without_src = std::numeric_limits<int>::max();
  while(std::getline(file, line)) {
    std::smatch match;
    std::regex_match(line, match, pattern);
    if (match[1] == 'x') {
      const int x = std::stoi(std::string(match[2]));
      const int min_y = std::stoi(std::string(match[4]));
      const int max_y = std::stoi(std::string(match[5]));
      min_row = std::min(min_row, min_y);
      max_row = std::max(max_row, max_y);
      min_col = std::min(min_col, x);
      max_col = std::max(max_col, x);
      for (int y = min_y; y <= max_y; y++) {
        scan[Point(y, x)] = '#';
      }
    } else {
      const int y = std::stoi(std::string(match[2]));
      const int min_x = std::stoi(std::string(match[4]));
      const int max_x = std::stoi(std::string(match[5]));
      min_row = std::min(min_row, y);
      max_row = std::max(max_row, y);
      min_col = std::min(min_col, min_x);
      max_col = std::max(max_col, max_x);
      for (int x = min_x; x <= max_x; x++) {
        scan[Point(y, x)] = '#';
      }
    }
  }
  min_row_without_src = min_row;

  scan[Point(0, 500)] = '+';
  min_row = std::min(min_row, 0);
  min_col = std::min(min_col, 500);
  max_row = std::max(max_row, 0);
  max_col = std::max(max_col, 500);

  auto in_limits = [min_row, min_col, max_row, max_col](const Point& p) {
    return min_row <= p.row && p.row <= max_row && min_col <= p.col && p.col <= max_col;
  };

  min_col -= 1;
  max_col += 1;
  Point min(min_row, min_col);
  Point max(max_row, max_col);
  expandPoint(Point(0, 500), scan, min, max);

  std::unordered_set<Point, point_hash> dried;
  std::unordered_set<Point, point_hash> water;

  // This can be significantly optimized
  for(const auto& [p, c] : scan) {
    if (dried.find(p) == dried.end() && water.find(p) == water.end() && c != '#') {
      std::vector<Point> v;
      const auto [left, lv] = check_horizontal_with_vec(p, Point(0, -1), scan, min, max, v);
      const auto [right, rv] = check_horizontal_with_vec(p, Point(0, 1), scan, min, max, v);
      if (left && right) {
        for (const auto& l : lv) {
          water.insert(l);
        }
        for (const auto& r : rv) {
          water.insert(r);
        }
        water.insert(p);
      } else {
        for (const auto& l : lv) {
          dried.insert(l);
        }
        for (const auto& r : rv) {
          dried.insert(r);
        }
        dried.insert(p);
      }
    }
  }

  for (const auto& n : dried) {
    scan.erase(n);
  }

  long long ans = 0;
  for (const auto& p : scan) {
    if (p.second == '~' && p.first.row >= min_row_without_src) {
      ans++;
    }
  }
  std::cout << ans << '\n';
  return 0;
}
