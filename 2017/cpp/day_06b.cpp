#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

std::string create_config(const std::vector<int>& banks) {
  std::string config;
  for (const auto& bank : banks) {
    config += std::to_string(bank) + '-';
  } 
  return config;
}

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_06_input" ;  
  std::ifstream file(input);
  std::string line;
  // Using vector instead of array to allow for sample input
  std::vector<int> banks;
  std::getline(file, line);
  std::size_t start = 0;
  std::size_t end = line.find(' ', start);
  while(end != std::string::npos) {
    banks.emplace_back(std::stoi(line.substr(start, end - start)));
    start = end + 1;
    end = line.find(' ', start);
  }
  std::size_t count = 0;
  banks.emplace_back(std::stoi(line.substr(start, end - start)));
  std::unordered_map<std::string, std::size_t> seen_configurations;
  auto config = create_config(banks);
  while(seen_configurations.insert({config, count}).second) {
    auto index = std::distance(std::begin(banks), std::max_element(std::begin(banks), std::end(banks)));
    int value = banks[index];
    banks[index] = 0;
    index++;
    index = index % banks.size();
    while (value > 0) {
      banks[index]++;
      value--;
      index++;
      index = index % banks.size();
    }
    config = create_config(banks);
    count++;
  }
  std::cout << count - seen_configurations[config] << '\n';
  return 0;
}