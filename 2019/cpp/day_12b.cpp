#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <vector>

long long gcd(const long long a, const long long b) {
  if (a == 0) {
    return b;
  }
  return gcd(b % a, a);
}

long long lcm(const long long a, const long long b) {
  return (a == 0 || b == 0) ? 0 : (a * b / gcd(a, b));
}

long long lcm(const std::vector<long long>& vals) {
  auto lcm_val = vals[0];
  for (int i = 1; i < vals.size(); i++) {
    lcm_val = lcm(vals[i], lcm_val);
  }
  return lcm_val;
}

std::array<long long, 3> convertLineToCoordinates(std::string line) {
  line.erase(std::remove_if(std::begin(line), std::end(line),
                            [](auto c) { return !isprint(c); }),
             std::end(line));
  std::array<long long, 3> point;
  const std::regex coord_pattern(R"(<x=(.+), y=(.+), z=(.+)>)");
  std::smatch pattern_match;
  std::regex_search(line, pattern_match, coord_pattern);
  for (int i = 0; i < 3; i++) {
    point[i] = std::stoi(pattern_match[i + 1]);
  }
  return point;
}

void SimulateTimeStep(std::vector<std::array<long long, 3>>& positions,
                      std::vector<std::array<long long, 3>>& velocities) {
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
  std::vector<std::array<long long, 3>> positions;
  while (std::getline(file, line)) {
    positions.push_back(convertLineToCoordinates(line));
  }

  // Solve
  std::array<long long, 3> temp;
  for (auto ele : temp) ele = 0;
  std::vector<std::array<long long, 3>> velocities(positions.size());
  for (auto ele : velocities) ele = temp;

  const auto init_positions = positions;
  const auto init_velocities = velocities;
  std::vector<long long> freqs(3, 0);
  long long iteration = 0;
  while (std::any_of(std::begin(freqs), std::end(freqs),
                     [](const long long freq) { return freq == 0; })) {
    SimulateTimeStep(positions, velocities);
    iteration++;
    for (int coord = 0; coord < 3; coord++) {
      if (freqs[coord] != 0) continue;
      bool back_to_init = true;
      for (int i = 0; i < positions.size(); i++) {
        if (positions[i][coord] != init_positions[i][coord] ||
            velocities[i][coord] != init_velocities[i][coord]) {
          back_to_init = false;
          break;
        }
      }
      if (back_to_init) {
        freqs[coord] = iteration;
      }
    }
  }
  auto lcm_val = lcm(freqs);
  std::cout << lcm_val << '\n';
  return lcm_val;
}
