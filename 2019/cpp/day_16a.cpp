#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

long long getOutputValueForRow(const std::vector<int>& signal, const std::vector<int>& pattern, const long long row) {
  long long pattern_index = 0;
  long long current_in_pattern_index = 1;
  if (row == 0) {
    pattern_index = 1;
    current_in_pattern_index = 0;
  }
  long long  total = 0;
  for (long long i = 0; i < signal.size(); i++) {
    // std::cout << "Multiplying: " << signal[i] << " and " << pattern[pattern_index] << '\n';
    total += signal[i] * pattern[pattern_index];
    current_in_pattern_index++;
    if (current_in_pattern_index == row + 1) {
      pattern_index++;
      if (pattern_index == pattern.size()) {
        pattern_index = 0;
      }
      current_in_pattern_index = 0;
    }
  }
  total = std::abs(total);
  return total % 10;
}

std::vector<int> FFTPhase(const std::vector<int>& signal, const std::vector<int>& pattern) {
  std::vector<int> new_signal;
  new_signal.reserve(signal.size());
  for (long long row = 0; row < signal.size(); row++) {
    new_signal.emplace_back(getOutputValueForRow(signal, pattern, row));
  }
  return new_signal;
}

int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_16_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string input_line;
  std::getline(file, input_line);
  std::vector<int> signal;
  for (const char c : input_line) {
    signal.emplace_back(c - '0');
  }

  const std::vector<int> pattern{0,1,0,-1};

  // Solve
  constexpr int n_phases = 100;
  for (int i = 0; i < n_phases; i++) {
    signal = FFTPhase(signal, pattern);
  }

  for (int i = 0; i < 8; i++) {
    std::cout << signal[i];
  }
  std::cout << '\n';
  return 0;
}
