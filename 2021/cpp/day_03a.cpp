#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

inline int convert_to_binary(const std::vector<int>& bin_vec) {
  const int s = bin_vec.size();
  int bin_val = 0;
  for (int i = 0; i < s; i++) {
    bin_val += (bin_vec[s - i - 1]) << i;
  }
  return bin_val;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_03_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  int n = 0;
  std::getline(file, line);
  n++;
  const size_t s = line.size();
  std::vector<int> digits(s);
  for (int i = 0; i < line.size(); i++) {
    digits[i] = line[i] - '0';
  }
  while(std::getline(file, line)) {
    n++;
    for (int i = 0; i < s; i++) {
      digits[i] += line[i] - '0';
    }
  }

  for (auto& ele : digits) {
    ele = (ele * 2) / n;
  }

  const int gamma = convert_to_binary(digits);
  for (auto& digit : digits) {
    digit = std::abs(1 - digit);
  }
  const int eps = convert_to_binary(digits);

  std::cout << gamma * eps << '\n';
  return 0;
}
