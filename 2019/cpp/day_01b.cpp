#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int getRecursiveFuel(const int mass) {
  constexpr int divisor = 3;
  constexpr int subtract = 2;
  constexpr int minimum_mass = divisor * subtract;
  if (mass < minimum_mass) {
    return 0;
  }
  const int fuel = mass / divisor - subtract;
  return fuel + getRecursiveFuel(fuel);
}

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
  for (const auto& mass : masses) {
    total_fuel += getRecursiveFuel(mass);
  }
  std::cout << total_fuel << '\n';
  return total_fuel;
}
