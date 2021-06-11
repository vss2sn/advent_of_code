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

struct pair3 {
  int x, y, z;
};

struct hash_pair3 {
  template <class T1, class T2>
  size_t operator()(const pair3& p) const {
    auto hash1 = std::hash<T1>{}(p.x);
    auto hash2 = std::hash<T2>{}(p.y);
    auto hash3 = std::hash<T2>{}(p.z);
    return hash1 ^ hash2 ^ hash3;
  }
};


struct hash_pair {
  template <class T1, class T2>
  size_t operator()(const Pair& p) const {
    auto hash1 = std::hash<T1>{}(p.x);
    auto hash2 = std::hash<T2>{}(p.y);
    return hash1 ^ hash2;
  }
};

std::vector<pair3> getNeighbours(const std::vector<std::vector<std::string>>& grids, const int level, const int row, const int col) {
  const auto&  grid = grids[0];
  const int half_row_size = grid.size()/2;
  const int half_col_size = grid[0].size()/2;
  const int row_size = grid.size();
  const int col_size = grid[0].size();
  if (row == half_row_size && col == half_col_size) {
    return {};
  }
  else if (row == 1 and col == 1) {
    return {{level, row+1, col}, {level, row, col+1}, {level+1, half_row_size, half_col_size-1}, {level+1, half_row_size-1, half_col_size}};
  }
  else if (row == 1 and col == col_size-2) {
    return {{level, row+1, col}, {level, row, col-1}, {level+1, half_row_size, half_col_size+1}, {level+1, half_row_size-1, half_col_size}};
  }
  else if (row == row_size-2 and col == 1) {
    return {{level, row-1, col}, {level, row, col+1}, {level+1, half_row_size+1, half_col_size}, {level+1, half_row_size, half_col_size-1}};
  }
  else if (row == row_size-2 and col == col_size-2) {
    return {{level, row-1, col}, {level, row, col-1}, {level+1, half_row_size+1, half_col_size}, {level+1, half_row_size, half_col_size+1}};
  }
  else if (row == 1) {
    return {{level, row+1, col}, {level, row, col+1}, {level, row, col - 1}, {level+1, half_row_size-1, half_col_size}};
  }
  else if (row == row_size-2) {
    return {{level, row-1, col}, {level, row, col+1}, {level, row, col - 1}, {level+1, half_row_size+1, half_col_size}};
  }
  else if (col == 1) {
    return {{level, row+1, col}, {level, row-1, col}, {level, row, col+1}, {level+1, half_row_size, half_col_size-1}};
  }
  else if (col == col_size-2) {
    return {{level, row+1, col}, {level, row-1, col}, {level, row, col-1}, {level+1, half_row_size, half_col_size+1}};
  }
  else if (row == half_row_size && col == half_col_size-1) {
    std::vector<pair3> neighbours = {{level, row + 1, col}, {level, row-1, col}, {level, row, col-1}};
    for (int i = 1; i < row_size - 1; i++) {
      neighbours.push_back({level - 1, i, 1});
    }
    return neighbours;
  }
  else if (row == half_row_size && col == half_col_size+1) {
    std::vector<pair3> neighbours = {{level, row + 1, col}, {level, row-1, col}, {level, row, col+1}};
    for (int i = 1; i < row_size - 1; i++) {
      neighbours.push_back({level - 1, i, col_size-2});
    }
    return neighbours;
  }
  else if (row == half_row_size-1 && col == half_col_size) {
    std::vector<pair3> neighbours = {{level, row, col+1}, {level, row, col-1}, {level, row-1, col}};
    for (int i = 1; i < col_size - 1; i++) {
      neighbours.push_back({level - 1, 1, i});
    }
    return neighbours;
  }
  else if (row == half_row_size+1 && col == half_col_size) {
    std::vector<pair3> neighbours = {{level, row, col+1}, {level, row, col-1}, {level, row+1, col}};
    for (int i = 1; i < col_size - 1; i++) {
      neighbours.push_back({level - 1, row_size-2, i});
    }
    return neighbours;
  }
  else {
    return {{level, row+1, col}, {level, row, col+1}, {level, row, col - 1}, {level, row-1, col}};
  }
}

int countNeighbouringBugs(const std::vector<std::vector<std::string>>& grids, const int level, const int row, const int col) {
  const auto neighbours = getNeighbours(grids, level, row, col);
  if (neighbours.size() > 8) {
    std::cout << "WH" << neighbours.size() << '\n';
    exit(0);
  }
  int count = 0;
  for (const auto& neighbour : neighbours) {

    if(grids[neighbour.x][neighbour.y][neighbour.z] == '#') {
      count += 1;
    }
  }
  // std::cout << count << '\n';
  return count;
}

void updateGrid(std::vector<std::vector<std::string>>& grids, const std::vector<std::vector<std::vector<int>>>& bug_count) {
  for (int level = 1; level < grids.size() - 1; level++) {
    for (int row = 1; row < grids[0].size() -1; row++) {
      for (int col = 1; col < grids[0][0].size() -1; col++) {
        if (grids[level][row][col] == '#' && bug_count[level][row][col] != 1) {
          grids[level][row][col] = '.';
        } else if (grids[level][row][col] == '.' && (bug_count[level][row][col] == 1 || bug_count[level][row][col] == 2)) {
          grids[level][row][col] = '#';
        }
      }
    }
  }
}

int countBugs(const std::vector<std::vector<std::string>>&  grids) {
  int count = 0;
  for (int level = 1; level < grids.size() - 1; level++) {
    for (int row = 1; row < grids[0].size() -1; row++) {
      for (int col = 1; col < grids[0][0].size() -1; col++) {
        if (grids[level][row][col] == '#') {
          count += 1;
        }
      }
    }
  }
  return count;
}

int main(int argc, char * argv[]) {
  std::vector<std::string> grid;
  std::string input = "../input/day_24_input";
  if (argc > 1) {
    input = argv[1];
  }


  std::ifstream file(input);
  std::string line;
  while (std::getline(file, line)) {
    line.erase(std::remove_if(std::begin(line), std::end(line), [](auto c) { return !isprint(c); }), std::end(line));
    if (line.empty()) break;
    grid.emplace_back('.' + line + '.');
  }

  grid.insert(std::begin(grid), std::string(grid[0].size(), '.'));
  grid.emplace_back(std::string(grid[0].size(), '.'));
  const auto empty_grid = std::vector<std::string>(grid.size(), std::string(grid[0].size(), '.'));
  auto grids = std::vector<std::vector<std::string>>(203, empty_grid);
  grids[101] = grid;

  auto bug_count = std::vector<std::vector<std::vector<int>>>(grids.size(), std::vector<std::vector<int>>(grids[0].size(), std::vector<int>(grids[0][0].size(), 0)));

  for (int time_step = 0; time_step < 200; time_step++) {
    // std::cout << time_step << '\n';
    // PrintGrid(grids[101]);
    auto prev_bug_count = bug_count;
    for (int level = 1; level < grids.size() -1 ; level++) {
      for (int row = 1; row < grids[0].size() -1; row++) {
        for (int col = 1; col < grids[0][0].size() -1; col++) {
          // std::cout << level << " " << row << " " << col << '\n';
          bug_count[level][row][col] = countNeighbouringBugs(grids, level, row, col);
        }
      }
    }
    if (bug_count == prev_bug_count) {
      std::cout << "No chage" << '\n';
    }
    updateGrid(grids, bug_count);
  }
  int count = countBugs(grids);
  std::cout << "Count: " << count << '\n';
  return count;
}
