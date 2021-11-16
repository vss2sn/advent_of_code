#include <cassert>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <thread>
#include <memory>
#include <numeric>
#include <regex>

struct Point {
  int x;
  int y;

  Point(int x, int y) : x(x), y(y){}

  Point operator + (const Point& p) const {
    return {this->x + p.x, this->y + p.y};
  }

  Point operator - (const Point& p) const {
    return {this->x - p.x, this->y - p.y};
  }
};

inline long long calcArea(const std::vector<Point>& positions) {
  const auto min_x = std::min_element(std::begin(positions), std::end(positions), [](const Point& p1, const Point& p2) { return p1.x < p2.x; })->x;
  const auto max_x = std::max_element(std::begin(positions), std::end(positions), [](const Point& p1, const Point& p2) { return p1.x < p2.x; })->x;
  const auto min_y = std::min_element(std::begin(positions), std::end(positions), [](const Point& p1, const Point& p2) { return p1.y < p2.y; })->y;
  const auto max_y = std::max_element(std::begin(positions), std::end(positions), [](const Point& p1, const Point& p2) { return p1.y < p2.y; })->y;
  const long long area = (max_y - min_y) * (max_y - min_y);
  return area > 0 ? area : std::numeric_limits<long long>::max();
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_10_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string line;
  const std::regex pattern(R"(position=<\s*(-?[0-9]+), \s*(-?[0-9]+)> velocity=<\s*(-?[0-9]+), \s*(-?[0-9]+)>)");
  std::vector<Point> positions;
  std::vector<Point> velocities;

  while(std::getline(file, line)) {
    std::smatch match;
    std::regex_search(line, match, pattern);
    positions.emplace_back(stoi(match[1]), stoi(match[2]));
    velocities.emplace_back(stoi(match[3]), stoi(match[4]));
  }

  long long area = calcArea(positions);
  long long max_area = std::pow(positions.size(), 2);
  long long prev_area = std::numeric_limits<long long>::max();
  while (area > max_area || area < prev_area) {
    std::transform(std::begin(positions), std::end(positions),
      std::begin(velocities), std::begin(positions),
      [](const auto& p, const auto& v) { return p + v ;} );
    prev_area = area;
    area = calcArea(positions);
  }

  std::transform(std::begin(positions), std::end(positions),
    std::begin(velocities), std::begin(positions),
    [](const auto& p, const auto& v) { return p - v ;} );

  const auto min_x = std::min_element(std::begin(positions), std::end(positions), [](const Point& p1, const Point& p2) { return p1.x < p2.x; })->x;
  const auto max_x = std::max_element(std::begin(positions), std::end(positions), [](const Point& p1, const Point& p2) { return p1.x < p2.x; })->x;
  const auto min_y = std::min_element(std::begin(positions), std::end(positions), [](const Point& p1, const Point& p2) { return p1.y < p2.y; })->y;
  const auto max_y = std::max_element(std::begin(positions), std::end(positions), [](const Point& p1, const Point& p2) { return p1.y < p2.y; })->y;

  std::vector<std::vector<char>> canvas(std::abs(max_y - min_y) + 1, std::vector<char>(std::abs(max_x - min_x) + 1, '.'));
  for (const auto& p : positions) {
    canvas[p.y - min_y][p.x - min_x] = '#';
  }

  for (int i = 0; i <= max_y - min_y; i++) {
    for (int j = 0; j <= max_x - min_x ; j++) {
      std::cout << canvas[i][j];
    }
    std::cout << '\n';
  }
  return 0;
}
