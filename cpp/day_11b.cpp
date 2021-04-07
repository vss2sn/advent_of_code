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
constexpr int n_tolerance = 5;
const std::vector<std::pair<int, int>> increments {{0,1}, {1,0}, {0,-1}, {-1,0},
{1,1}, {-1,-1}, {1,-1}, {-1,1}};

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
  for(const auto& increment:increments) {
    int new_row = row + increment.first;
    int new_col = col + increment.second;
    bool found = false; // found a chair/occupied
    while (!found) {
      if(new_row >= 0 && new_row < charmap.size() && new_col >= 0 && new_col < charmap[0].size()) {
        if(charmap[new_row][new_col] == chair) {
          found = true;
        } else if(charmap[new_row][new_col] == occupied) {
          found = true;
          count++;
        } else {
          new_row += increment.first;
          new_col += increment.second;
        }
      } else {
        break; // not setting found to true, as not found a chair
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
  if(charmap[row][col] == occupied && nmap[row][col] >= n_tolerance) {
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
