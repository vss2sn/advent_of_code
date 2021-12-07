#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

inline long long total_fuel_spent (const std::vector<long long>& positions, const long long final_position) {
  long long sum = 0;
  for (const auto& ele : positions) {
    sum += std::abs(ele - final_position);
  }
  return sum;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_07_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::getline(file, line);
  std::vector<long long> crab_positions;

  std::size_t start = 0;
  std::size_t end = line.find(',', start);
  while (end != std::string::npos) {
    crab_positions.emplace_back(std::stoll(line.substr(start, end - start)));
    start = end + 1;
    end = line.find(',', start);
  }
  crab_positions.emplace_back(std::stoll(line.substr(start, line.size() - start)));
  std::sort(std::begin(crab_positions), std::end(crab_positions));

  if (crab_positions.size() % 2 == 0) {
    // Account for rounding errors
    const long long s1 = total_fuel_spent(crab_positions, (crab_positions[crab_positions.size()/2] + crab_positions[crab_positions.size()/2 + 1])/2);
    const long long s2 = total_fuel_spent(crab_positions, (crab_positions[crab_positions.size()/2] + crab_positions[crab_positions.size()/2 + 1])/2 + 1);
    const long long s3 = total_fuel_spent(crab_positions, (crab_positions[crab_positions.size()/2] + crab_positions[crab_positions.size()/2 + 1])/2 - 1);
    std::cout << std::min(std::min(s1, s2), s3) << '\n';
  } else {
    std::cout << total_fuel_spent(crab_positions, crab_positions[crab_positions.size()/2 + 1]) << '\n';
  }
  return 0;
}
