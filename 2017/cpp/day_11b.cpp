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

std::vector<std::string> parse_path(const std::string& s) {
  std::vector<std::string> path;
  std::size_t start = 0;
  const std::string delimiter = ",";
  std::size_t end = s.find(delimiter);
  while(end != std::string::npos) {
    path.emplace_back(s.substr(start, end - start));
    start = end + delimiter.size();
    end = s.find(delimiter, start);
  }
  path.emplace_back(s.substr(start, s.size() - start));
  return path;
}

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_11_input" ;  
  std::ifstream file(input);
  std::string line;
  while(std::getline(file, line)) {
    float max_steps = 0;
    const auto path = parse_path(line);
    float x = 0;
    float y = 0;
    for (const auto& ele : path) {
      if (ele == "n") {
        y += 1;
      } else if (ele == "s") {
        y -= 1;
      } else if (ele == "ne") {
        x += 0.5;      
        y += 0.5;      
      } else if (ele == "se") {
        x += 0.5;      
        y -= 0.5;      
      } else if (ele == "nw") {
        x -= 0.5;      
        y += 0.5;      
      } else if (ele == "sw") {
        x -= 0.5;      
        y -= 0.5;      
      }
      const auto abs_x = std::fabs(x);
      const auto abs_y = std::fabs(y);
      if (abs_x > abs_y) {
        max_steps = std::max(max_steps, 2 * abs_y + 2 * (abs_x - abs_y));
      } else {
        max_steps = std::max(max_steps, 2 * abs_x + (abs_y - abs_x));
      }
    }
    std::cout << max_steps << '\n';;
  }
  return 0;
}