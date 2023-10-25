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

// Can modify to add only numbers that are not multiples of a pre-existing number
// and/or remove a number if its a multiple of a new number, 
// but can also sort the vector to achieve the same effect
void parse_input(const std::string& line, std::vector<std::pair<int, int>>& map) {
  std::size_t start = 0;
  const std::string delimiter = ": ";
  std::size_t end = line.find(delimiter);
  const auto depth = std::stoi(line.substr(start, end - start));
  start = end + delimiter.size();
  end = line.find(delimiter, start);
  const auto range = std::stoi(line.substr(start, end - start));
  map.emplace_back(depth, range);

}

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_13_input" ;  
  std::ifstream file(input);
  std::string line;
  std::vector<std::pair<int, int>> map;
  while(std::getline(file, line)) {
    parse_input(line, map);
  }
  int start_second = 0;
  std::sort(std::begin(map), std::end(map), [](const auto& p1, const auto& p2) {return p1.first < p2.first;});
  while(true) {
    bool success = true;
    for (const auto& [depth, range] : map) {
      if ((depth + start_second) % ((range - 1) * 2) == 0) {
        start_second++;
        success = false;
        break;
      }
    }
    if (success) {
      std::cout << "Successful with start at: " << start_second << '\n';
      return 0;
    }
  }
  return 0;
}