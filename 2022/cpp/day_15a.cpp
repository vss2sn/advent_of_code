#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>

struct Point {
  Point (const long long row, const long long col) : row(row), col(col) {}
  long long row, col;

  bool operator == (const Point& p) const {
    return p.row == row && p.col == col;
  }
};

struct PointHash {
  std::size_t operator() (const Point& p) const {
    return p.row * p.col;
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
    sensor .distance_to_closest = std::abs(sensor.row - beacon.row) + std::abs(sensor.col-beacon.col);
    sensors.push_back(sensor);
    beacons.push_back(beacon);
  }

  constexpr long long row = 2000000;
  std::unordered_set<Point,PointHash> empty;
  for (const auto& sensor : sensors) {
    if ((sensor.row + sensor.distance_to_closest) >= row &&
        (sensor.row - sensor.distance_to_closest) <= row) {
      const auto delta_col = sensor.distance_to_closest - std::abs(sensor.row - row);
      for (long long col = sensor.col - delta_col; col <= sensor.col + delta_col; col++) {
        empty.emplace(row, col);
      }
    }
  }

  for (const auto& beacon : beacons) {
    if (empty.find(beacon) != empty.end()) {
      empty.erase(beacon);
    }
  }

  std::cout << empty.size() << '\n';
  return 0;
}
