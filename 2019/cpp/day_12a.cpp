#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <vector>

std::array<int, 3> convertLineToCoordinates(std::string line) {
  line.erase(std::remove_if(std::begin(line), std::end(line),
                            [](auto c) { return !isprint(c); }),
             std::end(line));
  std::array<int, 3> point;
  const std::regex coord_pattern(R"(<x=(.+), y=(.+), z=(.+)>)");
  std::smatch pattern_match;
  std::regex_search(line, pattern_match, coord_pattern);
  for (int i = 0; i < 3; i++) {
    point[i] = std::stoi(pattern_match[i + 1]);
  }
  return point;
}

void SimulateTimeStep(std::vector<std::array<int, 3>>& positions,
                      std::vector<std::array<int, 3>>& velocities) {
  for (int i = 0; i < positions.size(); i++) {
    for (int j = i + 1; j < positions.size(); j++) {
      for (int coord = 0; coord < 3; coord++) {
        if (positions[i][coord] < positions[j][coord]) {
          velocities[i][coord] += 1;
          velocities[j][coord] -= 1;
        } else if (positions[i][coord] > positions[j][coord]) {
          velocities[i][coord] -= 1;
          velocities[j][coord] += 1;
        }
      }
    }
  }
  for (int i = 0; i < positions.size(); i++) {
    for (int coord = 0; coord < 3; coord++) {
      positions[i][coord] += velocities[i][coord];
    }
  }
}

int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_12_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string line;
  std::vector<std::array<int, 3>> positions;
  while (std::getline(file, line)) {
    positions.push_back(convertLineToCoordinates(line));
  }
  std::vector<std::array<int, 3>> velocities(positions.size());
  for (size_t time_step = 0; time_step < 1000; time_step++) {
    SimulateTimeStep(positions, velocities);
  }

  int energy = 0;
  for (int i = 0; i < positions.size(); i++) {
    energy +=
        std::accumulate(std::begin(positions[i]), std::end(positions[i]), 0,
                        [](const int total, const int ele) {
                          return std::abs(total) + std::abs(ele);
                        }) *
        std::accumulate(std::begin(velocities[i]), std::end(velocities[i]), 0,
                        [](const int total, const int ele) {
                          return std::abs(total) + std::abs(ele);
                        });
  }
  std::cout << energy << '\n';
}
