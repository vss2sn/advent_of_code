#include <algorithm>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

std::pair<int, int> ParseToGetCoordinates(const std::string& directions) {
  std::size_t index = 0;
  std::pair<int, int> coordinates{0, 0};
  while (index < directions.size()) {
    if (directions.substr(index, 1) == "e") {
      coordinates.first += 2;
      index += 1;
    } else if (directions.substr(index, 1) == "w") {
      coordinates.first -= 2;
      index += 1;
    } else if (directions.substr(index, 2) == "ne") {
      coordinates.first += 1;
      coordinates.second += 1;
      index += 2;
    } else if (directions.substr(index, 2) == "nw") {
      coordinates.first -= 1;
      coordinates.second += 1;
      index += 2;
    } else if (directions.substr(index, 2) == "se") {
      coordinates.first += 1;
      coordinates.second -= 1;
      index += 2;
    } else if (directions.substr(index, 2) == "sw") {
      coordinates.first -= 1;
      coordinates.second -= 1;
      index += 2;
    }
  }
  return coordinates;
}

int main() {
  std::fstream file{"../input/day_24_input"};
  std::string line;
  std::map<std::pair<int, int>, bool> tiles;
  while (std::getline(file, line)) {
    line.erase(std::remove_if(std::begin(line), std::end(line),
                              [](const char c) { return !isprint(c); }),
               std::end(line));
    if (line == "") continue;
    const auto coordinates = ParseToGetCoordinates(line);
    if (auto it = tiles.find(coordinates); it != tiles.end()) {
      tiles[coordinates] = !tiles[coordinates];
    } else {
      tiles.insert({coordinates, false});
    }
  }
  auto count = std::count_if(
      std::begin(tiles), std::end(tiles),
      [](const auto tile_with_val) { return !tile_with_val.second; });
  std::cout << count << '\n';
  return count;
}
