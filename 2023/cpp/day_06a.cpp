#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

std::vector<int> extract_numbers(const std::string& s, const int delta) {
  std::vector<int> numbers;
  std::size_t start = s.find_first_not_of(' ', delta);
  std::size_t end = s.find(' ', start);
  while (end != std::string::npos) {
    numbers.push_back(std::stoi(s.substr(start, end - start)));
    start = s.find_first_not_of(' ', end + 1);
    end = s.find(' ', start);
  }
  numbers.push_back(std::stoi(s.substr(start, s.size() - start)));
  return numbers;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_06_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::getline(file, line);
  const auto race_times = extract_numbers(line, 5);
  std::getline(file, line);
  const auto current_records = extract_numbers(line, 9);

  std::size_t ans = 1;
  for (int i = 0; i < current_records.size(); i++) {
    const double discriminant_sqrt = std::sqrt(race_times[i] * race_times[i] - 4 * (1) * (current_records[i]));
    int possible_root_1 =  std::floor(race_times[i] + discriminant_sqrt)/2;
    int possible_root_2 =  std::ceil(race_times[i] - discriminant_sqrt)/2;
    if (possible_root_1 * (race_times[i]  - possible_root_1) <= current_records[i]) possible_root_1--;
    if (possible_root_2 * (race_times[i]  - possible_root_2) <= current_records[i]) possible_root_2++;
    ans *= (possible_root_1 - possible_root_2 + 1);
  }
  std::cout << ans  << '\n';
  return 0;
}