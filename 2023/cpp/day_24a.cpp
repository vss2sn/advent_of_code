#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <limits>
#include <queue>
#include <regex> 
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>
#include <cassert> 

// constexpr double limit_min = 7;
// constexpr double limit_max = 27;

constexpr double limit_min = 200000000000000;
constexpr double limit_max = 400000000000000;

struct Line {
  // ax + by = c
  double a;
  double b;
  double c;
};

struct Hailstone {
  std::array<double, 3> position;
  std::array<double, 3> velocity; 
};

Hailstone parse_input(const std::string& line) {
  const std::regex pattern(R"((-?[0-9]+),(-?[0-9]+),(-?[0-9]+)@(-?[0-9]+),(-?[0-9]+),(-?[0-9]+))");
  std::smatch match;
  std::regex_search(line, match, pattern); 
  // std::cout << '|' << line << '|' << '\n';
  // std::cout << match[0] << '\n';
  Hailstone hs;
  hs.position[0] = std::stod(match[1]);
  hs.position[1] = std::stod(match[2]);
  hs.position[2] = std::stod(match[3]);
  hs.velocity[0] = std::stod(match[4]);
  hs.velocity[1] = std::stod(match[5]);
  hs.velocity[2] = std::stod(match[6]);
  return hs;
}

Line calculate_trajectory(const Hailstone& hs) {
  Line line;
  if (hs.velocity[0] == 0) {
    line.a = 0;
    line.b = 1;
    line.c = hs.position[0];
  } else {
    line.a = -hs.velocity[1];
    line.b = hs.velocity[0];
    line.c = line.b * hs.position[1] + line.a * hs.position[0];
    // line.is_vertical = false;
    // std::cout << -line.a/line.b << ' ' << hs.velocity[1]/hs.velocity[0] << '\n';
    assert(-line.a/line.b == hs.velocity[1]/hs.velocity[0]);
    // std::cout << line.a << " x + " << line.b << " y  = " << line.c << '\n';

    // std::cout << hs.position[1] << ' ' << line.m * hs.position[0] + line.c << ' ' << std::fabs(hs.position[1] - line.m * hs.position[0] - line.c) << '\n';
    // assert(std::fabs(hs.position[1] - line.m * hs.position[0] - line.c) < 0.00001);
  }
  return line;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_24_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<Hailstone> hailstones;
  std::vector<Line> trajectories;
  while(std::getline(file, line)) {
    line.erase(std::remove_if(line.begin(), line.end(), [](const char c) {return c == ' ';}), line.end());
    hailstones.emplace_back(parse_input(line));
    trajectories.emplace_back(calculate_trajectory(hailstones.back()));
  }

  int n_intersections = 0;
  for (int i = 0; i < trajectories.size(); i++) {
    const auto& line_1 = trajectories[i];
    for (int j = i+1; j < trajectories.size(); j++) {
      const auto& line_2 = trajectories[j];
      const auto determinant = (line_1.a * line_2.b) - (line_2.a * line_1.b);
      // std::cout << "\nStart: " << '(' << i << "," << j << ')' << '\n';
      // std::cout << determinant << '\n';
      if (determinant == 0) continue;
      const auto x = ((line_1.c * line_2.b) - (line_2.c * line_1.b)) / determinant;
      const auto y = ((line_1.a * line_2.c) - (line_2.a * line_1.c)) / determinant;
      if ((x - hailstones[i].position[0])/hailstones[i].velocity[0] < 0 ) {
        // std::cout << "intersected in the past" << '\n';
        continue;
      }
      if ((x - hailstones[j].position[0])/hailstones[j].velocity[0] < 0 ) {
        // std::cout << "intersected in the past" << '\n';
        continue;
      }
      // std::cout << x << ' ' << y  << '\n';
      if (x >= limit_min && x <= limit_max && y >= limit_min && y <= limit_max) {
        // std::cout << "Incrementing" << '\n';
        n_intersections++;
      }
    }
  }
  std::cout << n_intersections << '\n';
  return 0;
}
