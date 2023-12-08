#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

long long extract_number(const std::string& s, const int delta) {
  std::string number;
  std::size_t start = s.find_first_not_of(' ', delta);
  std::size_t end = s.find(' ', start);
  while (end != std::string::npos) {
    number += s.substr(start, end - start);
    start = s.find_first_not_of(' ', end + 1);
    end = s.find(' ', start);
  }
  number += s.substr(start, s.size() - start);
  return std::stoll(number);
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_06_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::getline(file, line);
  const auto race_time = extract_number(line, 5);
  std::getline(file, line);
  const auto current_record = extract_number(line, 9);

  const double discriminant_sqrt = std::sqrt(race_time * race_time - 4 * (1) * (current_record));
  long long possible_root_1 =  std::floor(race_time + discriminant_sqrt)/2;
  long long possible_root_2 =  std::ceil(race_time - discriminant_sqrt)/2;
  if (possible_root_1 * (race_time  - possible_root_1) <= current_record) possible_root_1--;
  if (possible_root_2 * (race_time  - possible_root_2) <= current_record) possible_root_2++;
  std::cout << (possible_root_1 - possible_root_2 + 1)  << '\n';
  return 0;
}