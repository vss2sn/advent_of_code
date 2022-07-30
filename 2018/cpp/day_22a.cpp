#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using map2d = std::vector<std::vector<unsigned long long>>;

void update(map2d& geo_index, map2d& erosion, const int depth) {
  for (int row = 0; row < geo_index.size(); row++) {
    erosion[row][0] = (geo_index[row][0] + depth) % 20183;
  }
  for (int col = 0; col < geo_index[0].size(); col++) {
    erosion[0][col] = (geo_index[0][col] + depth) % 20183;
  }

  for (int row = 1; row < erosion.size(); row++) {
    for (int col = 1; col < erosion[0].size(); col++) {
      geo_index[row][col] = erosion[row - 1][col] * erosion[row][col - 1];
      erosion[row][col] = (geo_index[row][col] + depth) % 20183;
    }
  }
  geo_index[erosion.size() - 1][erosion[0].size() - 1] = 0;
  erosion[erosion.size() - 1][erosion[0].size() - 1] = (depth) % 20183;
}

void draw_map2d(const map2d& map) {
  std::cout << "Map2d" << '\n';
  for (int row = 0; row < map.size(); row++) {
    for (int col = 0; col < map[0].size(); col++) {
      std::cout << map[row][col] << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

void update_cave (const map2d& erosion, map2d& cave) {
  for (int row = 0; row < erosion.size(); row++) {
    for (int col = 0; col < erosion[0].size(); col++) {
      cave[row][col] = erosion[row][col] % 3;
    }
  }
}

unsigned long long calculate_risk (const map2d& cave) {
  unsigned long long risk = 0;
  for (int row = 0; row < cave.size(); row++) {
    for (int col = 0; col < cave[0].size(); col++) {
      risk += cave[row][col];
    }
  }
  return risk;
}

void draw_cave (const map2d& cave) {
  std::cout << "Cave" << '\n';
  for (int row = 0; row < cave.size(); row++) {
    for (int col = 0; col < cave[0].size(); col++) {
      if (cave[row][col] == 0) {
        std::cout << '.';
      } else if (cave[row][col] == 1) {
        std::cout << '=';
      } else {
        std::cout << '|';
      }
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_22_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::ifstream file(input);
  std::getline(file, line);
  const int depth = std::stoi(line.substr(7, line.size() - 7));

  std::getline(file, line);
  const auto delimiter = ',';
  const auto delimiter_index = line.find(delimiter);
  const auto n_cols = std::stoi(line.substr(8, delimiter_index - 8)) + 1;
  const auto n_rows = std::stoi(line.substr(delimiter_index + 1, line.size() - delimiter_index)) + 1;

  auto geo_index = map2d(n_rows, std::vector<unsigned long long>(n_cols, 0));
  for (int row = 0; row < geo_index.size(); row++) {
    geo_index[row][0] = row * 48271;
  }
  for (int col = 0; col < geo_index[0].size(); col++) {
    geo_index[0][col] = col * 16807;
  }

  auto erosion = map2d(n_rows, std::vector<unsigned long long>(n_cols, 0));
  auto cave = map2d(n_rows, std::vector<unsigned long long>(n_cols, 0));

  update(geo_index, erosion, depth);
  update_cave(erosion, cave);

  const auto risk = calculate_risk(cave);
  std::cout << risk << '\n';
  return risk;
}
