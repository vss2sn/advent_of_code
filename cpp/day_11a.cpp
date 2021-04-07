#include <fstream>
#include <iostream>
#include <vector>

constexpr int size_of_conv_left = 1;
constexpr int size_of_conv_right = 1;
constexpr int size_of_conv_up = 1;
constexpr int size_of_conv_down = 1;
constexpr char chair = 'L';
constexpr char occupied = '#';
constexpr char floor = '.';

template<typename T>
void PrintMap(const T& map) {
  std::cout << "----------------" << '\n';
  for(int row = 0; row < map.size(); row++) {
    for(int col = 0; col < map[0].size(); col++) {
      std::cout << map[row][col] << ' ';
    }
    std::cout << '\n';
  }
  std::cout << "----------------" << '\n';
}

bool updateNMapAtCoord(const std::vector<std::string>& charmap,
                       std::vector<std::vector<int>>& nmap,
                       const int row,
                       const int col) {
  int count = 0;
  for(int i = row - size_of_conv_up; i <= row + size_of_conv_down; ++i) {
    if(i < 0 || i >= charmap.size()) continue;
    for (int j = col - size_of_conv_left; j <= col + size_of_conv_down; ++j) {
      if(j < 0 || j >= charmap[0].size()) continue;
      if (i == row && j == col) continue;
      if(charmap[i][j] == occupied ) {
        ++count;
      }
    }
  }
  if(nmap[row][col] != count) {
    nmap[row][col] = count;
    return true;
  }
  return false;
}

bool updateCharMapAtCoord(std::vector<std::string>& charmap,
                          const std::vector<std::vector<int>>& nmap,
                          const int row,
                          const int col) {
  if(charmap[row][col] == occupied && nmap[row][col] >= 4) {
    charmap[row][col] = chair;
    return true;
  } else if(charmap[row][col] == chair && nmap[row][col] == 0) {
    charmap[row][col] = occupied;
    return true;
  }
  return false;
}

int main() {
  std::ifstream file{"../input/day_11_input"};
  std::string line;
  std::vector<std::string> charmap;
  while(std::getline(file, line)) {
    charmap.push_back(line);
  }
  const int n_rows = charmap.size();
  const int n_cols = charmap[0].size();

  std::vector<std::vector<int>> nmap(n_rows, std::vector<int>(n_cols, -1));
  for(int row = 0; row < n_rows; row++) {
    for(int col = 0; col < n_cols; col++) {
      updateNMapAtCoord(charmap, nmap, row, col);
    }
  }

  for(int row = 0; row < n_rows; row++) {
    for(int col = 0; col < n_cols; col++) {
      updateCharMapAtCoord(charmap, nmap, row, col);
    }
  }

  bool changed = true;
  while(changed) {
    changed = false;
    for(int row = 0; row < n_rows; row++) {
      for(int col = 0; col < n_cols; col++) {
        if(updateNMapAtCoord(charmap, nmap, row, col)) {
          changed = true;
        }
      }
    }

    for(int row = 0; row < n_rows; row++) {
      for(int col = 0; col < n_cols; col++) {
        updateCharMapAtCoord(charmap, nmap, row, col);
      }
    }
  }

  int count = 0;
  for(int row = 0; row < n_rows; row++) {
    for(int col = 0; col < n_cols; col++) {
      if(charmap[row][col] == occupied) ++count;
    }
  }

  std::cout << count << '\n';
  return count;
}
