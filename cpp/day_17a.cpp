#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

constexpr int n_steps = 6;

int getActiveNeigbourCount(const std::vector<std::vector<std::string>>& volume, const int plane, const int row, const int col) {
  int count = 0;
  for (int d_plane = -1; d_plane < 2; d_plane++) {
    const int new_plane = plane + d_plane;
    if (new_plane < 0 || new_plane >= volume.size()) continue;
    for (int d_row = -1; d_row < 2; d_row++) {
      const int new_row = row + d_row;
      if (new_row < 0 || new_row >= volume[new_plane].size()) continue;
      for (int d_col = -1; d_col < 2; d_col++) {
        const int new_col = col + d_col;
        if (new_col < 0 || new_col >= volume[new_plane][new_row].size()) continue;
        if (volume[new_plane][new_row][new_col] == '#' && !(plane == new_plane && col == new_col && row == new_row)) count++;
      }
    }
  }
  return count;
}

int main() {
  std::fstream file{"../input/day_17_input"};
  std::string line;
  std::string inactive_part = std::string(n_steps, '.');

  std::vector<std::string> g_plane;
  while (std::getline(file, line)) {
    line.erase(std::remove_if(std::begin(line), std::end(line), [](auto c) { return !isprint(c); }), std::end(line) );
    if(line == "") continue;
    g_plane.push_back(inactive_part + line + inactive_part);
  }

  for(int i =0; i < n_steps; ++i) {
    g_plane.push_back(std::string(g_plane[0].size(), '.'));
  }

  for(int i =0; i < n_steps; ++i) {
    g_plane.insert(g_plane.begin(), std::string(g_plane[n_steps].size(), '.'));
  }

  std::vector<std::string> inactive_plane{g_plane.size(), std::string(g_plane[0].size(), '.')};
  std::vector<std::vector<std::string>> volume;
  for (int i = 0; i < n_steps; i++) {
    volume.push_back(inactive_plane);
  }
  volume.push_back(g_plane);
  for (int i = 0; i < n_steps; i++) {
    volume.push_back(inactive_plane);
  }

  std::vector<std::vector<std::vector<int>>> neighbour_count{volume.size(), std::vector<std::vector<int>>(volume[0].size(), std::vector<int>(volume[0][0].size(), 0))};
  for (int step = 0; step < n_steps; step++) {

    for (int plane = 0; plane < volume.size(); plane++) {
      for(int row = 0; row < volume[plane].size(); row++) {
        for (int col = 0; col < volume[plane][row].size(); col++) {
          neighbour_count[plane][row][col] = getActiveNeigbourCount(volume, plane, row, col);
        }
      }
    }

    for (int plane = 0; plane < volume.size(); plane++) {
      for(int row = 0; row < volume[plane].size(); row++) {
        for (int col = 0; col < volume[plane][row].size(); col++) {
          if(volume[plane][row][col] == '#' && !(neighbour_count[plane][row][col] == 2 || neighbour_count[plane][row][col] == 3)) {
            volume[plane][row][col] = '.';
          } else if (volume[plane][row][col] == '.' && neighbour_count[plane][row][col] == 3) {
            volume[plane][row][col] = '#';
          }
        }
      }
    }
  }

  int count = 0;
  for(const auto& plane : volume) {
    for(const auto& line : plane) {
      count += std::count_if(std::begin(line), std::end(line), [](const char c) { return c == '#'; });
    }
  }

  std::cout << count << '\n';
  return 0;
}
