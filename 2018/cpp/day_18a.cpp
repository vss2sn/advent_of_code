#include <cassert>
#include <queue>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cassert>
#include <unordered_map>
#include <unordered_set>

#include <chrono>
#include <thread>

using namespace std::literals::chrono_literals;

struct SurroundingTypes {
  int n_open = 0;
  int n_wooded = 0;
  int n_yard = 0;
  void reset() {
    n_open = 0;
    n_wooded = 0;
    n_yard = 0;
  }
  friend std::ostream& operator << (std::ostream& os, const SurroundingTypes& s);
};

std::ostream& operator << (std::ostream& os, const SurroundingTypes& s) {
  os << "(" << s.n_open << ", " << s.n_wooded << ", " << s.n_yard << ") ";
  return os;
}

template<typename T2D>
void print(const T2D& t2d) {
  for (const auto& row : t2d) {
    for (const auto & ele : row ) {
      std::cout << ele;
    }
    std::cout << '\n';
  }
}

void updateSurroundings(const std::vector<std::string>& area, std::vector<std::vector<SurroundingTypes>>& surroundings, const std::vector<std::vector<int>>& deltas) {
  for (int row = 1; row < area.size() - 1; row++) {
    for (int col = 1; col < area[0].size() - 1; col++) {
      surroundings[row][col].reset();
      for (const auto& delta : deltas) {
        const int new_row = row + delta[0];
        const int new_col = col + delta[1];
        if (area[new_row][new_col] == '#') {
          surroundings[row][col].n_yard++;
        } else if (area[new_row][new_col] == '.') {
          surroundings[row][col].n_open++;
        } else if (area[new_row][new_col] == '|') {
          surroundings[row][col].n_wooded++;
        }
      }
    }
  }
}

void updateArea(std::vector<std::string>& area, const std::vector<std::vector<SurroundingTypes>>& surroundings, const std::vector<std::vector<int>>& deltas) {
  for (int row = 1; row < area.size() - 1; row++) {
    for (int col = 1; col < area[0].size() - 1; col++) {
      const auto& s = surroundings[row][col];
      auto& a = area[row][col];
      if (a == '.' && s.n_wooded >=3) {
        a = '|';
      } else if (a == '|' && s.n_yard >=3) {
        a = '#';
      } else if (a == '#' && !(s.n_yard >= 1 && s.n_wooded >= 1)) {
        a = '.';
      }
    }
  }
}

SurroundingTypes count(const std::vector<std::string>& area) {
  SurroundingTypes total;
  for (int row = 1; row < area.size() - 1; row++) {
    for (int col = 1; col < area[0].size() - 1; col++) {
      if (area[row][col] == '#') {
        total.n_yard++;
      } else if (area[row][col] == '|') {
        total.n_wooded++;
      } else if (area[row][col] == '.') {
        total.n_open++;
      }
    }
  }
  return total;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_18_input";
  int n_minutes = 10;
  if (argc > 1) {
    input = argv[1];
  }
  if (argc > 2) {
    n_minutes = std::stoi(argv[2]);
  }

  std::string line;
  std::ifstream file(input);
  std::vector<std::string> area;
  area.emplace_back();
  while(std::getline(file, line)) {
    area.emplace_back("0" + line + "0");
  }
  area[0] = std::string(area[1].size(), '0');
  area.emplace_back(std::string(area[1].size(), '0'));

  std::vector<std::vector<SurroundingTypes>> surroundings(area.size(), std::vector<SurroundingTypes>(area[0].size()));

  const std::vector<std::vector<int>> deltas {
    {0, 1},
    {1, 0},
    {-1, 0},
    {0, -1},
    {1,1},
    {1,-1},
    {-1,-1},
    {-1, 1}
  };
  // print(area);
  for (int minute = 1; minute <= n_minutes; minute++) {
    updateSurroundings(area, surroundings, deltas);
    // print(surroundings);
    updateArea(area, surroundings, deltas);
    // print(area);
    // std::cout << '\n';
  }
  const auto st = count(area);
  std::cout << st.n_wooded * st.n_yard << '\n';
  return 0;
}
