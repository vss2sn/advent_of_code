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
  const auto path = parse_path(line);
    float x = 0;
    float y = 0;
    for (const auto& ele : path) {
      std::cout << "Move: |" << ele << '|' << '\n';
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
      std::cout << "At: (" << x << ',' << y << ')' << '\n';
    }
    x = std::fabs(x);
    y = std::fabs(y);
    if (x > y) {
      std::cout << "n steps: " << 2 * y + 2 * (x - y) << '\n';
    } else {
      std::cout << "n steps: " << 2 * x + (y - x) << '\n';
    }
  }
  return 0;
}