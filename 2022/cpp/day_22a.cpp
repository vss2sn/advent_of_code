#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

struct Point {
  Point (const long long row = 0, const long long col = 0) : row(row), col(col) {}
  long long row, col;

  bool operator == (const Point& p) const {
    return p.row == row && p.col == col;
  }

  friend std::ostream& operator<<(std::ostream& os, const Point& p);
};

std::ostream& operator<<(std::ostream& os, const Point& p) {
  os << '(' << p.row << ' ' << p.col << ')' << '\n';
  return os;
}

Point move(const std::vector<std::string>& grid, const int dir, const int n, const Point& start) {
  int i = 0;
  auto current = start;
  while (i < n) {
    auto prev_acceptable = current;
    if (dir == 0) {
      current.col++;
      if (current.col >= grid[0].size()) current.col = 0;
      if(grid[current.row][current.col] == '#') {
        current.col--;
        break;
      }
      while (grid[current.row][current.col] == ' ') {
        current.col++;
        if (current.col >= grid[0].size()) current.col = 0;
      }
    } else if (dir == 1) {
      current.row++;
      if (current.row >= grid.size()) current.row = 0;
      if(grid[current.row][current.col] == '#') {
        current.row--;
        break;
      }
      while (grid[current.row][current.col] == ' ') {
        current.row++;
        if (current.row >= grid.size()) current.row = 0;
      }
    } else if (dir == 2) {
      current.col--;
      if (current.col < 0) current.col = grid[0].size()-1;
      if(grid[current.row][current.col] == '#') {
        current.col++;
        break;
      }
      while (grid[current.row][current.col] == ' ') {
        current.col--;
        if (current.col < 0) current.col = grid[0].size()-1;
      }
    } else if (dir == 3) {
      current.row--;
      if (current.row < 0) current.row = grid.size()-1;
      if(grid[current.row][current.col] == '#') {
        current.row++;
        break;
      }
      while (grid[current.row][current.col] == ' ') {
        current.row--;
        if (current.row < 0) current.row = grid.size()-1;
      }
    }
    if(grid[current.row][current.col] == '#') {
      current = prev_acceptable;
    }
    i++;
    // std::cout << current << '\n';
  }
  return current;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_22_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<std::string> grid;
  int dir = 0;
  while(std::getline(file, line)) {
    if (line == "") break;
    grid.push_back(line);
  }
  std::size_t max_len = 0;
  for (const auto& row : grid) {
    max_len = std::max(max_len, row.size());
  }
  for (auto& row : grid) {
    if (row.size() < max_len) {
      row.resize(max_len, ' ');
    }
    // std::cout << '|' << row << '|' << '\n';
  }

  auto start = Point();
  for (int i = 0; i < grid[0].size(); i++) {
    if(grid[0][i] == '.') {
      start.row = 0;
      start.col= i;
      break;
    }
  }
  // std::cout << start;
  // exit(0);

  std::getline(file, line);

  const std::string path = line;
  // std::cout << line << '\n';
  std::vector<std::size_t> direction_indixes;
  for (int i = 0; i < path.size(); i++) {
    if (path[i] == 'L' || path[i] == 'R') {
      direction_indixes.push_back(i);
    }
  }

  std::size_t start_i = 0;
  auto current = start;
  for (int i = 0; i < direction_indixes.size(); i++) {
    const int delta_val = std::stoi(path.substr(start_i, direction_indixes[i] - start_i));
    // std::cout << delta_val << '\n';
    current = move(grid, dir, delta_val, current);
    // std::cout <<  "Now at: " << current << '\n';
    const char delta_dir = path[direction_indixes[i]];
    // std::cout << '|' << delta_dir << '|' << '\n';
    dir += (delta_dir == 'R' ? 1 : -1);
    dir += 4;
    dir = dir % 4;
    start_i = direction_indixes[i]+1;
  }
  if (direction_indixes.back() != path.size()) {
    const int delta_val = std::stoi(path.substr(start_i, path.size() - start_i));
    current = move(grid, dir, delta_val, current);
    // std::cout << current << '\n';
  }
  // std::cout << dir << '\n';
  std::cout << 1000 * (current.row+1)  + 4 * (current.col+1) + dir << '\n';
  return 0;
}
