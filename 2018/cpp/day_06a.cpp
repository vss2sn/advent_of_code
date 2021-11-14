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
  std::vector<int> nearest(size_t(n), std::numeric_limits<int>::max());
  for (int i = 0; i < n; i++) {
    const Point p0 = { i % w, i / w };
    int best_j = -1;
    int best_dist = std::numeric_limits<int>::max();
    for (int j = 0; j < num_points; j++) {
      int dist = distance(p0, points[j]);
      if (dist < best_dist) {
        best_j = j;
        best_dist = dist;
      }
      else if (dist == best_dist) {
        best_j = -1;
      }
    }
    nearest[i] = best_j;
  }

  std::vector<bool> infinite_area(points.size(), false);
  for (int y = 0; y < h; y++) {
    int i0 = y * w;
    int i1 = i0 + (w - 1);
    if (nearest[i0] >= 0)
      infinite_area[nearest[i0]] = true;
    if (nearest[i1] >= 0)
      infinite_area[nearest[i1]] = true;
  }
  for (int x = 0; x < w; x++) {
    int i0 = x;
    int i1 = i0 + h * (w - 1);
    if (nearest[i0] >= 0)
      infinite_area[nearest[i0]] = true;
    if (nearest[i1] >= 0)
      infinite_area[nearest[i1]] = true;
  }

  std::vector<int> area(points.size(), 0);
  for (int i = 0; i < n; i++) {
    if (nearest[i] >= 0) {
      area[nearest[i]]++;
    }
  }

  int largest_area = 0;
  for (int i = 0; i < num_points; i++) {
    if (!infinite_area[i] && area[i] > largest_area) {
      largest_area = area[i];
    }
  }

  std::cout << "Largest area: " << largest_area << '\n';
  return largest_area;
}
