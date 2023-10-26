#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <regex>
#include <cmath>
#include <queue>  

int parse_input(const std::string& line) {
  std::size_t start = 24;
  std::cout << '|' <<line.substr(start, line.size() - start) << '|' << '\n';
  return std::stoull(line.substr(start, line.size() - start));
}

bool last_16_same(std::vector<std::size_t>& values) {
  const auto bv0 = std::bitset<64>(values[0]);
  const auto bv1 = std::bitset<64>(values[1]);
  // std::cout << "A: " << bv0.to_string() << '\n';
  // std::cout << "B: " << bv1.to_string() << '\n';
  // for (int i = 0; i< bv0.size(); i++) {
  //   std::cout << bv0[i];
  // }
  // std::cout << '\n';

  for (int i = 0; i < 16; i++) {
    // std::cout << i << ' ' << bv0[i] << ' ' << bv1[i] << '\n';
    if (bv0[i] != bv1[i]) {
      // std::cout << "returning false" << '\n';
      return false;
    }    
  }
  return true;
}

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_15_input" ;  
  std::ifstream file(input);
  std::string line;
  std::vector<std::size_t> values;
  std::vector<std::size_t> multiplication_factor{16807, 48271};
  while(std::getline(file, line)) {
    values.emplace_back(parse_input(line));
  }
  // std::cout << "A: " << values[0] << '\n';
  // std::cout << "B: " << values[1] << '\n';
  // std::cout << values.size() << '\n';
  std::size_t count = 0;
  for (std::size_t idx = 0; idx < 40000000; idx++) {
    values[0] = (values[0] * multiplication_factor[0]) % 2147483647;
    values[1] = (values[1] * multiplication_factor[1]) % 2147483647;
    // std::cout << "A: " << values[0] << '\n';
    // std::cout << "B: " << values[1] << '\n';

    if(last_16_same(values)) {
      // std::cout << "Count increased" << '\n';
      count++;
    } 
  }
  std::cout << count << '\n';
  return 0;
}