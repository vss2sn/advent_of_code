#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

struct Point {
  Point (const long long row, const long long col) : row(row), col(col) {}
  long long row, col;

  bool operator == (const Point& p) const {
    return p.row == row && p.col == col;
  }
};

struct Sensor {
  long long row;
  long long col;
  long long distance_to_closest;
};

int main(int argc, char * argv[]) {
  std::string input = "../input/day_15_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::vector<Sensor> sensors;
  std::vector<Point> beacons;
  const std::regex pattern(R"(Sensor at x=(-?[0-9]+), y=(-?[0-9]+): closest beacon is at x=(-?[0-9]+), y=(-?[0-9]+))");
  while (std::getline(file, line)) {
    std::smatch match;
    std::regex_match(line, match, pattern);
    const auto beacon = Point(std::stoll(match[4]), std::stoll(match[3]));
    auto sensor = Sensor();
    sensor.row = std::stoll(match[2]);
    sensor.col = std::stoll(match[1]);
    sensor.distance_to_closest = std::abs(sensor.row - beacon.row) + std::abs(sensor.col-beacon.col);
    sensors.push_back(sensor);
    beacons.push_back(beacon);
  }

  constexpr long long grid_max = 4000000;
  std::sort(std::begin(sensors), std::end(sensors), [](const auto& s1, const auto& s2) {return s1.col < s2.col;});

  for (long long row = 0; row <= grid_max; row++) {
    for (long long col = 0; col <= grid_max; col++) {
      bool found = true;
      for (const auto& sensor : sensors) {
        if (std::abs(sensor.row - row) + std::abs(sensor.col - col) <= sensor.distance_to_closest) {
          const auto delta_col = sensor.distance_to_closest - std::abs(sensor.row - row);
          if (sensor.col >= col) {
            col += std::abs(sensor.col - col) + delta_col;
          } else {
            col += -std::abs(sensor.col - col) + delta_col;
          }
          found = false;
          break;
        }
      }
      if (found) {
        std::cout << col * 4000000 + row << '\n';
        return 0;
      }
    }
  }
  return 0;
}
