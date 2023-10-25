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

void parse_input(const std::string& line, std::unordered_map<int, int>& map) {
  std::size_t start = 0;
  const std::string delimiter = ": ";
  std::size_t end = line.find(delimiter);
  const auto depth = std::stoi(line.substr(start, end - start));
  start = end + delimiter.size();
  end = line.find(delimiter, start);
  const auto range = std::stoi(line.substr(start, end - start));
  map[depth] = range;
}

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_13_input" ;  
  std::ifstream file(input);
  std::string line;
  std::unordered_map<int, int> map;
  while(std::getline(file, line)) {
    parse_input(line, map);
  }
  int total_severity = 0;
  for (const auto& [depth, range] : map) {
    if (depth % ((range - 1) * 2) == 0) {
      total_severity += depth * range;
    }
  }
  std::cout << total_severity << '\n';
  return 0;
}