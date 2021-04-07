#include <algorithm>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>

std::pair<int, int> ParseToGetCoordinates(const std::string& directions) {
  std::size_t index = 0;
  std::pair<int, int> coordinates{0, 0};
  while(index < directions.size()) {
    if(directions.substr(index, 1) == "e") {
      coordinates.first += 2;
      index += 1;
    } else if(directions.substr(index, 1) == "w") {
      coordinates.first -= 2;
      index += 1;
    } else if(directions.substr(index, 2) == "ne") {
      coordinates.first += 1;
      coordinates.second += 1;
      index += 2;
    } else if(directions.substr(index, 2) == "nw") {
      coordinates.first -= 1;
      coordinates.second += 1;
      index += 2;
    } else if(directions.substr(index, 2) == "se") {
      coordinates.first += 1;
      coordinates.second -= 1;
      index += 2;
    } else if(directions.substr(index, 2) == "sw") {
      coordinates.first -= 1;
      coordinates.second -= 1;
      index += 2;
    }
  }
  return coordinates;
}

const std::vector<std::pair<int, int>> additions {
  {2, 0},
  {-2, 0},
  {1, 1},
  {1, -1},
  {-1, 1},
  {-1, -1}
};


void AddRequredTiles(std::map<std::pair<int, int>, bool>& tiles){
  std::set<std::pair<int, int>> to_add;
  for(auto& [coordinates, white_up] : tiles) {
    for (const auto& addition : additions) {
      const std::pair<int, int> new_coordinates{coordinates.first + addition.first, coordinates.second + addition.second};
      if(tiles.find(new_coordinates) == tiles.end()) {
        to_add.insert(new_coordinates);
      }
    }
  }
  for(const auto& coordinates : to_add) {
    tiles[coordinates] = true;
  }
}

void MakeArtLive(std::map<std::pair<int, int>, bool>& tiles) {
  std::map<std::pair<int, int>, bool> to_flip;
  for(auto& [coordinates, white_up] : tiles) {
    int count = 0;
    for (const auto& addition : additions) {
      const std::pair<int, int> new_coordinates{coordinates.first + addition.first, coordinates.second + addition.second};
      if(auto it = tiles.find(new_coordinates); it != tiles.end()) {
        if(it->second == false) {
          count++;
        }
      }
    }
    if (!white_up && (count == 0 || count  > 2)) {
      to_flip[coordinates] = true;
    } else if (white_up && count == 2) {
      to_flip[coordinates] = true;
    }
  }
  for (auto& [coordinates, white_up] : tiles) {
    if(to_flip[coordinates]) {
      white_up = !white_up;
    }
  }
}

int main() {
  std::fstream file{"../input/day_24_input"};
  std::string line;
  std::map<std::pair<int, int>, bool> tiles;
  while(std::getline(file, line)) {
    line.erase(std::remove_if(std::begin(line), std::end(line), [](const char c) {return !isprint(c);}), std::end(line));
    if(line == "") continue;
    const auto coordinates = ParseToGetCoordinates(line);
    if(auto it = tiles.find(coordinates); it != tiles.end()) {
      tiles[coordinates] = !tiles[coordinates];
    } else {
      tiles.insert({coordinates, false});
    }
  }

  for(int i = 0; i < 100; i++) {
    AddRequredTiles(tiles);
    MakeArtLive(tiles);
  }

  auto count = std::count_if(std::begin(tiles), std::end(tiles),
    [](const auto tile_with_val) {return !tile_with_val.second; });
  std::cout << count << '\n';
  return count;
}
