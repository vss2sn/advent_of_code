#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

int find_next_value(std::vector<int>& value_sequence) {
  // std::cout << "Value sequence: ";
  // for (const auto& ele : value_sequence) {
  //   std::cout << ele << ' ';
  // }
  // std::cout << '\n';
  if (value_sequence.size() == 1) {
    value_sequence.push_back(value_sequence.back());
    return value_sequence.back();
  }
  std::vector<int> new_value_sequence;
  new_value_sequence.reserve(value_sequence.size() - 1);
  for (int i = 0; i < value_sequence.size() - 1; i++) {
    new_value_sequence.push_back(value_sequence[i+1] - value_sequence[i]);
  }
  if (std::any_of(value_sequence.begin(), value_sequence.end(), [](const auto& ele) {return ele != 0; })) {
    const auto value_to_add = find_next_value(new_value_sequence); 
    value_sequence.push_back(value_sequence.back() + value_to_add);
    return value_sequence.back();
  } 
  value_sequence.push_back(value_sequence.back());
  return value_sequence.back();
}

std::vector<int> extract_number(const std::string& s) {
  std::vector<int> numbers;
  std::size_t start = 0;
  std::size_t end = s.find(' ', start);
  while (end != std::string::npos) {
    numbers.push_back(std::stoi(s.substr(start, end - start)));
    start = end + 1;
    end = s.find(' ', start);
  }
  numbers.push_back(std::stoi(s.substr(start, s.size() - start)));
  return numbers;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_09_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<std::vector<int>> value_sequences;
  while(std::getline(file, line)) {
    if (line.empty()) continue;
    value_sequences.push_back(extract_number(line));
  } 
  int total = 0;
  for (auto& value_sequence : value_sequences) {
    std::reverse(value_sequence.begin(), value_sequence.end());
    total += find_next_value(value_sequence);
  }
  std::cout << total << '\n';
  return 0;
}