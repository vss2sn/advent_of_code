#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_01_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::istream_iterator<int> start(file), end;
  const std::vector<int> masses(start, end);

  // Solve
  long long total_fuel = 0;
  constexpr int divisor = 3;
  constexpr int subtract = 2;
  for (const auto& mass : masses) {
    total_fuel += mass / divisor - subtract;
  }

  // STL variant
  // long long total_fuel = std::accumulate(start, end, 0, [](long long
  // sum_fuel, const int ele) { return sum_fuel + getRecursiveFuel(ele);} );

  std::cout << total_fuel << '\n';
  return total_fuel;
}
