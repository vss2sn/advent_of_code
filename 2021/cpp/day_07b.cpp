#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

long long total_fuel_spent (const std::vector<long long>& positions, const long long final_position) {
  long long sum = 0;
  for (const auto& ele : positions) {
    sum += (std::abs(ele - final_position) * (std::abs(ele - final_position) + 1)) / 2;
  }
  return sum;
}

int  main(int  argc, char * argv[]) {
  std::string input = "../input/day_07_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::getline(file, line);
  std::vector<long long > crab_positions;

  std::size_t start = 0;
  std::size_t end = line.find(',', start);
  long long average_pos = 0;
  while (end != std::string::npos) {
    const long long position = std::stoll(line.substr(start, end - start));
    crab_positions.emplace_back(position);
    average_pos += position;
    start = end + 1;
    end = line.find(',', start);
  }
  const long long position = std::stoll(line.substr(start, line.size() - start));
  crab_positions.emplace_back(position);
  average_pos += position;
  average_pos /= crab_positions.size();

  std::cout << std::min(total_fuel_spent(crab_positions, average_pos), total_fuel_spent(crab_positions, average_pos + 1)) << '\n';

  return 0;
}
