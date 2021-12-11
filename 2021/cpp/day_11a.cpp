#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Point {
  int row, col;
  Point (const int row, const int col) : row(row), col(col) {}
  friend std::ostream& operator << (std::ostream& os, const Point& p);
};

std::ostream& operator << (std::ostream& os, const Point& p) {
  os << "(" << p.row << ", " << p.col << ")";
  return os;
}

template<typename T>
void print(std::vector<std::vector<T>>& v) {
  for (const auto& row : v) {
    for(const auto ele : row) {
      std::cout << ele;
    }
    std::cout << '\n';
  }
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_11_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::vector<std::vector<int>> energy;
  while(std::getline(file, line)) {
    energy.emplace_back();
    for (const auto c : line) {
      energy.back().emplace_back(c - '0');

    }
  }

  const auto get_neighbours = [](const int row, const int col){
    return std::vector<Point> {
      Point(row + 1, col),
      Point(row - 1, col),
      Point(row + 1, col + 1),
      Point(row - 1, col + 1),
      Point(row + 1, col - 1),
      Point(row - 1, col - 1),
      Point(row, col + 1),
      Point(row, col - 1),
    };
  };

  const auto in_bounds = [&energy] (const Point& p){
    return p.row >=0 && p.row < energy.size() &&
           p.col >=0 && p.col < energy[0].size();
  };

  constexpr int n = 100;
  long long count = 0;
  for (int i = 0; i < n; i++) {
    bool flash = true;
    std::vector<std::vector<bool>> already_flashed(energy.size(), std::vector<bool>(energy[0].size(), false));
    for (int row = 0; row < energy.size(); row++) {
      for (int col = 0; col < energy[0].size(); col++) {
        energy[row][col]++;
      }
    }
    while(flash) {
      flash = false;
      for (int row = 0; row < energy.size(); row++) {
        for (int col = 0; col < energy[0].size(); col++) {
          if (energy[row][col] > 9 && !already_flashed[row][col]) {
            already_flashed[row][col] = true;
            energy[row][col] = 0;
            flash = true;
            count++;
            for (const auto& nbr : get_neighbours(row, col)) {
              if (in_bounds(nbr) && !already_flashed[nbr.row][nbr.col]) {
                energy[nbr.row][nbr.col]++;
              }
            }
          }
        }
      }
    }
  }
  std::cout << count << '\n';
  return 0;
}
