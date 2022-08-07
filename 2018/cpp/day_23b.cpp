#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <cassert>
#include <cmath>


struct NanoBot {
  long long x, y, z, r;
  NanoBot (const long long x, const long long y, const long long z, const long long r) : x(x), y(y), z(z), r(r) {}
};

struct Point {
  long long x, y, z;

  Point (const long long x, const long long y, const long long z) : x(x), y(y), z(z) {}

  bool operator == (const Point& b) const {
    return x == b.x && y == b.y && z == b.z;
  }

  bool operator < (const Point& b) const {
    return x < b.x ||
           x == b.x && y < b.y ||
           x == b.x && y == b.y && z < b.z;
  }
};

int main(int argc, char* argv[]) {
  std::string input = "../input/day_23_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::ifstream file(input);
  const std::regex pattern(R"(<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>, r=([0-9]+))");
  std::vector<NanoBot> bots;
  while(std::getline(file, line)) {
    std::smatch match;
    std::regex_search(line, match, pattern);
    bots.emplace_back(std::stoll(match[1]), std::stoll(match[2]), std::stoll(match[3]), std::stoll(match[4]));
  }

  const auto max_x = std::max_element(std::begin(bots), std::end(bots), [](const auto& b1, const auto& b2) {return b1.x < b2.x; })->x;
  const auto max_y = std::max_element(std::begin(bots), std::end(bots), [](const auto& b1, const auto& b2) {return b1.y < b2.y; })->y;
  const auto max_z = std::max_element(std::begin(bots), std::end(bots), [](const auto& b1, const auto& b2) {return b1.z < b2.z; })->z;
  const auto min_x = std::min_element(std::begin(bots), std::end(bots), [](const auto& b1, const auto& b2) {return b1.x < b2.x; })->x;
  const auto min_y = std::min_element(std::begin(bots), std::end(bots), [](const auto& b1, const auto& b2) {return b1.y < b2.y; })->y;
  const auto min_z = std::min_element(std::begin(bots), std::end(bots), [](const auto& b1, const auto& b2) {return b1.z < b2.z; })->z;

  auto scale = ((long long)(1) << (long long)(std::log2(max_x - min_x + max_y - min_y + max_z - min_z) + 1));
  std::vector<Point> points {Point(min_x, min_y, min_z)};

  while (true) {
    std::vector<Point> new_points;
    size_t max_n_bots_in_range = 0;
    for (const auto& point : points) {
      const auto n_bots_in_range = std::count_if(
        std::begin(bots),
        std::end(bots),
        [&scale=std::as_const(scale), &point=std::as_const(point)](const auto& bot) {
          return (std::abs(point.x - bot.x) + std::abs(point.y - bot.y) + std::abs(point.z - bot.z)) <= (bot.r + scale);
        }
      );
      if (n_bots_in_range != 0 && n_bots_in_range == max_n_bots_in_range) {
       new_points.push_back(point);
      } else if (n_bots_in_range > max_n_bots_in_range) {
       max_n_bots_in_range = n_bots_in_range;
       new_points.clear();
       new_points.push_back(point);
      }
    }
    if (scale == 0) {
      std::swap(points, new_points);
      break;
    }
    points.clear();
    scale /= 2;
    if(scale == 0) {
      std::swap(points, new_points);
    } else {
      for(const auto &point : new_points) {
        for(long long dx = -scale; dx <= scale; dx += scale) {
          for(long long dy = -scale; dy <= scale; dy += scale) {
            for(long long dz = -scale; dz <= scale; dz += scale) {
              points.emplace_back(point.x + dx, point.y + dy, point.z + dz);
            }
          }
        }
      }
      std::sort(points.begin(), points.end());
      points.erase(std::unique(points.begin(), points.end()), points.end());
    }
  }

  auto min_distance = std::numeric_limits<long long>::max();
  for(const auto &point : points) {
    min_distance = std::min(
      min_distance,
      std::abs(point.x) + std::abs(point.y) + std::abs(point.z)
    );
  }
  std::cout << min_distance << '\n';
  return min_distance;
}
