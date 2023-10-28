#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <regex>
#include <bitset>
#include <climits>

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_17_input" ;  
  std::ifstream file(input);
  std::string line;
  std::getline(file, line); 
  std::vector<int> buffer{0};
  buffer.reserve(2018);
  int current_position = 0;
  const int number = std::stoi(line);
  for (int i = 1 ; i <= 2017; i++) {
    const int rem = (current_position + number + 1) % buffer.size();
    if (rem == buffer.size()) {
      buffer.push_back(i);
    } else {
      buffer.insert(std::next(std::begin(buffer) + rem), i);
    }
    current_position = rem;
    // std::cout << i << " inserted at " << current_position << '\n';
    // for (const auto ele : buffer ) {
    //   std::cout << ele << ' ';
    // }
    // std::cout << '\n';
  }
  std::cout << buffer [(current_position + 2) % buffer.size()] << '\n';
  return 0;
}