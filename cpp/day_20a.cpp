#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

struct Tile {
  int id = -1;
  std::unordered_set<int> neighbour_ids;
  std::vector<std::string> image;
  std::vector<std::vector<std::pair<int, std::string>>> edges_with_orientation;

  void createEdges() {
    std::vector<std::pair<int, std::string>> edges;
    edges.push_back(std::make_pair(0, image.front()));
    edges.push_back(std::make_pair(2, image.back()));
    std::string left, right;
    for(int i  = 0; i < image.size(); i++) {
      left += image[i][0];
      right += image[i][image[0].size() - 1];
    }
    edges.push_back(std::make_pair(3, left));
    edges.push_back(std::make_pair(1, right));
    edges_with_orientation.push_back(edges);
    for(int i = 0; i < edges.size(); ++i) {
      std::reverse(std::begin(edges[i].second), std::end(edges[i].second));
    }
    edges_with_orientation.push_back(edges);
  }
};

int main() {
  std::fstream file{"../input/day_20_input"};
  std::string line;
  Tile new_tile;
  std::unordered_map<int, std::pair<Tile, bool>> tiles;
  std::unordered_map<std::string, std::vector<int>> edge_map; // making map and vector instead of array to future proof
  while (std::getline(file, line)) {
    line.erase(std::remove_if(std::begin(line), std::end(line), [](const char c) {return !isprint(c); }), std::end(line));
    if(line == "") {
      new_tile.createEdges();
      tiles.insert({new_tile.id, std::make_pair(new_tile, false)});
      new_tile = Tile();
      continue;
    }
    else if(line.substr(0, 4) == "Tile") {
      new_tile.id = std::stoi(line.substr(5, line.size() - 5));
    }
    else {
      new_tile.image.push_back(line);
    }
  }
  if(new_tile.id != -1) {
    new_tile.createEdges();
    tiles.insert({new_tile.id, std::make_pair(new_tile, false)});
  }

  for(const auto& [tile_id, tile_and_used] : tiles) {
    for(const auto& edges : tile_and_used.first.edges_with_orientation) {
      for (auto [orient, edge] : edges) {
        if(edge_map.find(edge) == edge_map.end()) {
          edge_map.insert({edge, {tile_id}});
        } else {
          edge_map[edge].push_back(tile_id);
        }
      }
    }
  }

  // CreateBorder
  for(const auto& [edge, ids]: edge_map) {
    for(int i = 0; i < ids.size(); i++) {
      for(int j =  i + 1; j < ids.size(); j++) {
        tiles[ids[i]].first.neighbour_ids.insert(ids[j]);
        tiles[ids[j]].first.neighbour_ids.insert(ids[i]);
      }
    }
  }

  std::unordered_set<int> border;
  std::unordered_set<int> corner_ids;
  for(auto& [id, tile_and_used] : tiles) {
    if (tile_and_used.first.neighbour_ids.size() == 2) {
      border.insert(id);
      corner_ids.insert(id);
    } else if (tile_and_used.first.neighbour_ids.size() == 3) {
      border.insert(id);
    }
  }

  long long product = 1;
  for(const auto corner_id : corner_ids) {
    product *= corner_id;
  }
  std::cout << product << '\n';
  return product;
}
