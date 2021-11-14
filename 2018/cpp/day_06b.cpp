#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

struct Point {
  int x, y;

  Point& operator -= (const Point& p) {
    x -= p.x;
    y -= p.y;
    return *this;
  }
};

int distance(const Point& a, const Point& b) {
  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

Point min(const Point& a, const Point& b) {
  return Point{ std::min(a.x, b.x), std::min(a.y, b.y) };
}

Point max(const Point& a, const Point& b) {
  return Point{ std::max(a.x, b.x), std::max(a.y, b.y) };
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_06_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  Point p;
  std::vector<Point> points;
  constexpr int max_dist = 10000;

  const std::string delimiter = ", ";
  while(std::getline(file, line)) {
    const auto split = line.find(delimiter);
    const Point p{stoi(line.substr(0, split)), stoi(line.substr(split + delimiter.size(), line.size() - split))};
    points.emplace_back(p);
  }

  Point lo{std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
  Point hi{std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};

  for (const Point& p : points) {
    lo = min(lo, p);
    hi = max(hi, p);
  }

  for (Point& p : points) {
    p -= lo;
  }
  hi -= lo;

  const int w = hi.x + 1;
  const int h = hi.y + 1;
  const int n = w * h;

  const int num_points = int(points.size());
  std::vector<bool> pts_in_region(size_t(n), false);
  // Assumes distance is less than either of map's dimensions
  for (int i = 0; i < n; i++) {
    const Point p0 = { i % w, i / w };
    bool pt_in_region = true;
    int dist = 0;
    for (int j = 0; j < num_points && pt_in_region; j++) {
      dist += distance(p0, points[j]);
      if(dist >= max_dist) {
        pt_in_region = false;
      }
    }
    pts_in_region[i] = pt_in_region;
  }

  const int count_pts_in_region = std::count_if(std::begin(pts_in_region), std::end(pts_in_region), [](const bool b) {return b;});
  std::cout << "Points in region: " << count_pts_in_region << '\n';
  return count_pts_in_region;
}
