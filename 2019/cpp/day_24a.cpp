#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <fstream>

void PrintGrid(const std::vector<std::string>& view) {
  for(const auto & row : view) {
    for (const auto c : row) {
      std::cout << c;
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

struct Pair {
  int x, y;
};

struct hash_pair {
  template <class T1, class T2>
  size_t operator()(const Pair& p) const {
    auto hash1 = std::hash<T1>{}(p.x);
    auto hash2 = std::hash<T2>{}(p.y);
    return hash1 ^ hash2;
  }
};


int main(int argc, char * argv[]) {
  std::vector<std::string> grid;
  std::string input = "../input/day_24_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::unordered_set<long long> scores;

  std::ifstream file(input);
  std::string line;
  while (std::getline(file, line)) {
    line.erase(std::remove_if(std::begin(line), std::end(line), [](auto c) { return !isprint(c); }), std::end(line));
    if (line.empty()) break;
    grid.emplace_back('.' + line + '.');
  }

  grid.insert(std::begin(grid), std::string(grid[0].size(), '.'));
  grid.emplace_back(std::string(grid[0].size(), '.'));

  auto bug_count = std::vector<std::vector<int>>(grid.size(), std::vector<int>(grid[0].size(), 0));
  const auto deltas = std::vector<Pair> {
    {1,0},
    {0,1},
    {-1,0},
    {0,-1}
  };

  long long score = 0;
  long long c_val = 1;

  for (int row = 1; row < grid.size() -1; row++) {
    for (int col = 1; col < grid[0].size() -1; col++) {
      if (grid[row][col] == '#') {
        score += c_val;
      }
      c_val *= 2;
    }
  }


  while (scores.find(score) == scores.end()) {
    PrintGrid(grid);
    std::cout << '\n';
    scores.insert(score);
    for (int row = 1; row < grid.size() -1; row++) {
      for (int col = 1; col < grid[0].size() -1; col++) {
        int count = 0;
        for (const auto& delta : deltas) {
          if (grid[row + delta.x][col + delta.y] == '#') {
            count++;
          }
          bug_count[row][col] = count;
        }
      }
    }
    for (int row = 1; row < grid.size() -1; row++) {
      for (int col = 1; col < grid[0].size() -1; col++) {
        if (grid[row][col] == '#' && bug_count[row][col] != 1) {
          grid[row][col] = '.';
        } else if (grid[row][col] == '.' && (bug_count[row][col] == 1 || bug_count[row][col] == 2)) {
          grid[row][col] = '#';
        }
      }
    }

    c_val = 1;
    score = 0;

    for (int row = 1; row < grid.size() -1; row++) {
      for (int col = 1; col < grid[0].size() -1; col++) {
        if (grid[row][col] == '#') {
          score += c_val;
        }
        c_val *= 2;
      }
    }
  }
  std::cout << "Score: " << score << '\n';
  return score;
}
