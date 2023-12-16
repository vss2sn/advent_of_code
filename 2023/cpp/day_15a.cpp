#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int calculate_hash(const std::string& line) {
  int value = 0;
  for (const auto c : line) {
    value += int(c);
    value *= 17;
    value %= 256;
  }
  return value;
}

std::vector<std::string> parse(const std::string& s) {
  std::vector<std::string> substrs;
  std::size_t start = 0;
  std::size_t end = s.find(',', start);
  while (end != std::string::npos) {
    substrs.push_back(s.substr(start, end - start));
    start = end + 1;
    end = s.find(',', start);
  }
  substrs.push_back(s.substr(start, s.size() - start));
  return substrs;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_15_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::getline(file, line);
  std::size_t total = 0;
  for (const auto& substr : parse(line)) {
    total += calculate_hash(substr);
  }
  std::cout << total << '\n';
  return 0;
}