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

struct Coord {
  int row;
  int col;
};

Coord get_next_point(const int dir, const Coord& current_point) {
  Coord next_point;
  if (dir == 3) {
    next_point.row = current_point.row;
    next_point.col = current_point.col + 1;
  } else if (dir == 0) {
    next_point.row = current_point.row - 1;
    next_point.col = current_point.col;
  } else if (dir == 1) {
    next_point.row = current_point.row;
    next_point.col = current_point.col - 1;
  } else if (dir == 2) {
    next_point.row = current_point.row + 1;
    next_point.col = current_point.col;
  } else {
    std::cout << "This should not happen" << '\n';
    exit(0);
  }
  return next_point;
}

bool in_map(const Coord& current, const std::vector<std::string>& map) {
  return current.row >= 0 && current.row < map.size() && current.col >= 0 && current.col < map[0].size(); 
}

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_19_input" ;  
  std::ifstream file(input);
  std::string line;
  std::vector<std::string> map;
  while(std::getline(file, line)) {
    map.push_back(line);
  }
  Coord current;
  current.row = 0;
  int dir = 2;
  std::string letters;
  for (int i = 0; i < map[0].size(); i++) {
    if (map[0][i] == '|') {
      current.col = i;
      break;
    }
  }
  std::size_t steps = 0;
  while(true) {
    steps++;
    // std::cout << current.row << ',' << current.col << ": " << map[current.row][current.col] << '\n';
    // Check if this assumption is always true or whether a turn can be the only option when at a point that is a letter
    if(map[current.row][current.col] != '+') {
      current = get_next_point(dir, current);
    } else {
      const auto current_dir = dir;
      dir = (dir + 1) % 4;
      while(true) {
        if (dir == current_dir) {
          // std::cout << "Reached end" << '\n';
          std::cout << steps << '\n';
          return 0;
        }
        if ((dir + current_dir) % 2 == 0) {
          dir = (dir + 1) % 4;
          continue;
        }
        const auto new_point = get_next_point(dir, current);
        if (in_map(new_point, map) && map[new_point.row][new_point.col] != ' ') {
          current = new_point;
          break;
        }
        dir = (dir + 1) % 4;
      }
    }
    if (isalpha(map[current.row][current.col]) && !isdigit(map[current.row][current.col])) {
      letters += map[current.row][current.col];
    }
    if (map[current.row][current.col] == ' ') {
        // std::cout << "Reached end" << '\n';
        std::cout << steps << '\n';
        return 0;
    }
  }

  return 0;
}