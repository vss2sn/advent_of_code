#include <array>
#include <fstream>
#include <functional>  
#include <iostream>
#include <ranges>
#include <string>
#include <vector>  
#include <algorithm>

bool is_trap(const bool l, const bool c, const bool r) {
    if (l && c && !r) return true;
    if (!l && c && r) return true; 
    if (l && !c && !r) return true; 
    if (!l && !c && r) return true; 
    return false;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_18_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  while(std::getline(file, line)) {
    int count = 0;
    std::cout << "Input " << line << '\n';
    std::vector<std::vector<bool>> map(1, std::vector<bool>());
    for (const char c : line) {
        if (c == '.') {
            count++;
            map[0].push_back(false);
        } else {
            map[0].push_back(true);
        }
    }
    while(map.size() < 40) {
        const auto& prev = map.back();
        std::vector<bool> next_row;
        next_row.reserve(prev.size());
        next_row.push_back(is_trap(false, prev[0], prev[1]));
        if (!next_row.back()) count++;
        for (int i = 1; i < prev.size() - 1; i++) {
            next_row.push_back(is_trap(prev[i-1], prev[i], prev[i+1]));
            if (!next_row.back()) count++;
        }
        next_row.push_back(is_trap(prev[prev.size()-2], prev[prev.size()-1], false));
        if (!next_row.back()) count++;
        map.push_back(next_row);
    }
    for (const auto& row : map) {
        for (const auto ele : row) {
            std::cout << (ele ? '^' : '.');
        }
        std::cout << '\n';
    }
    std::cout << count << '\n';
  }
  return 0;
}