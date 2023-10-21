
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <regex>

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_09_input" ;  
  std::ifstream file(input);
  std::string line;
  while(std::getline(file, line)) {
    int idx = 0;
    bool is_in_garbage = false;
    int level = 0;
    int score = 0;
    int groups = 0;
    int char_count = 0;
    while(idx < line.size()) {
      if (!is_in_garbage) {
        if (line[idx] == '{') {
          level++;
          score +=  level;
          groups++;
        } 
        else if (line[idx] == '}') {
          level--;
        } 
        else if (line[idx] == '<') {
          is_in_garbage = true;
        } 
      } else {
        if (line[idx] == '!') {
          idx++;
        } else if (line[idx] == '>') {
          is_in_garbage = false;
        } else {
          char_count++;
        }
      }
      idx++;
    }
    std::cout << "groups: " << groups << '\n';
    std::cout << "score: " << score << '\n';
    std::cout << "char_count: " << char_count << '\n';
    std::cout << '\n';
  }
  return 0;
}