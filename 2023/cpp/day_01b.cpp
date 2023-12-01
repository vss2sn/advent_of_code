#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char * argv[]) {
  std::string input = "../input/day_01_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  
  std::vector<std::string> numbers  {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

  std::vector<int> calibration_values;
  while(std::getline(file, line)) {
    std::vector<int> digits;
    for (int i = 0; i < line.size(); i++) {
        const auto c = line[i];
        if (c >= '0' && c <= '9') {
            digits.push_back(c - '0');
        }
        for (int j = 0; j < numbers.size(); j++) {
            const auto& n = numbers[j];
            if (line.size() >= i + n.size()) {
                if (line.substr(i, n.size()) == n) {
                    digits.push_back(j+1);
                }
            }
        }
    }
    calibration_values.push_back(digits[0] * 10 + digits.back());  
  }


  const auto sum = std::accumulate(std::begin(calibration_values), std::end(calibration_values), 0);
  std::cout << sum << '\n';
  return 0;
}
