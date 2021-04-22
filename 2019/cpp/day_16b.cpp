#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <numeric>

int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_16_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string input_line;
  std::getline(file, input_line);
  std::cout << input_line << '\n';
  std::vector<int> signal;
  for (const char c : input_line) {
    signal.emplace_back(c - '0');
  }
  const auto init_signal = signal;

  // Solve
  const int offset = std::stoi(input_line.substr(0, 7));
  for (int i = 1; i < 10000; i++) {
    std::copy(std::begin(init_signal), std::end(init_signal), std::back_inserter(signal));
  }
  const std::vector<int> pattern{0,1,0,-1};

  constexpr int n_phases = 100;
  for (int i = 0; i < n_phases; i++) {
    long long sm = std::accumulate(std::begin(signal) + offset, std::end(signal), 0, std::plus<long long>());
    for (int j = offset; j < signal.size(); j++) {
      const auto sm_i = sm;
      sm -= signal[j];
      signal[j] = sm_i % 10;
    }
  }

  long long msg = 0;
  for (int i = 0 ; i < 8; i++) {
    msg = msg * 10 + signal[offset + i];
  }

  std::cout << msg << '\n';
  return msg;
}
