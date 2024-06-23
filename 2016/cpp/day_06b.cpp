#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

int main(int argc, char* argv[]) {
  std::string input = "../input/day_06_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::string line;
  std::fstream file(input);
  std::vector<std::unordered_map<char, int>> frequencies;
  while(std::getline(file, line)) {
    while(frequencies.size() < line.size()) {
        frequencies.emplace_back();
    }
    for (std::size_t idx = 0; idx < line.size(); idx++) {
        frequencies[idx][line[idx]]++;
    }
  }
  std::string least_frequent(frequencies.size(), '0');
  for (std::size_t idx = 0; idx < frequencies.size(); idx++) {
    int min_f = std::numeric_limits<int>::max();
    for (const auto& [c, f] : frequencies[idx]) {
      if (f < min_f) {
        min_f = f;
        least_frequent[idx] = c;
      }    
    }
  }
  std::cout << least_frequent << '\n';
  return 0;
}