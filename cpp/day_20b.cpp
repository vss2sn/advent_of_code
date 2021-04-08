#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Tile {
  bool used = false;
  bool flipped = false;
  int id = -1;
  int rotation = 0;
  std::unordered_set<int> neighbour_ids;
  std::vector<std::string> image;
  std::vector<std::string> edges;  // always in order up, right, bottom, left
  // the sides are always read left to right and up to down

  void createEdges() {
    std::string left, right;
    for (int i = 0; i < image.size(); i++) {
      left += image[i][0];
      right += image[i][image[0].size() - 1];
    }
    edges.push_back(image.front());
    edges.push_back(right);
    edges.push_back(image.back());
    edges.push_back(left);
  }

  std::string& getRightEdge() { return edges[1]; }

  std::string& getBottomEdge() { return edges[2]; }

  void Rotate() {
    std::rotate(edges.rbegin(), edges.rbegin() + 1, edges.rend());
    std::reverse(std::begin(edges[0]), std::end(edges[0]));
    std::reverse(std::begin(edges[2]), std::end(edges[2]));
    rotation += 1;
    rotation %= 4;
  }

  void Flip() {
    std::swap(edges[0], edges[1]);
    std::swap(edges[2], edges[3]);
    for (auto& edge : edges) {
      std::reverse(std::begin(edge), std::end(edge));
    }
    flipped = !flipped;
  }

  void applyRotationsAndFlipToImage() {
    if (flipped) {
      auto new_image = image;
      for (int i = 0; i < image.size(); i++) {
        for (int j = 0; j < image[i].size(); j++) {
          new_image[image[i].size() - j - 1][image.size() - i - 1] =
              image[i][j];
        }
      }
      image = new_image;
    }
    for (int i = 0; i < rotation; i++) {
      auto new_image = image;
      for (int i = 0; i < image.size(); i++) {
        for (int j = 0; j < image[i].size(); j++) {
          new_image[j][image.size() - i - 1] = image[i][j];
        }
      }
      image = new_image;
    }
  }
};

void RotateImage(std::vector<std::string>& image) {
  auto new_image = image;
  for (int i = 0; i < image.size(); i++) {
    for (int j = 0; j < image[i].size(); j++) {
      new_image[j][image.size() - i - 1] = image[i][j];
    }
  }
  image = new_image;
}

void FlipImage(std::vector<std::string>& image) {
  auto new_image = image;
  for (int i = 0; i < image.size(); i++) {
    for (int j = 0; j < image[i].size(); j++) {
      new_image[image[i].size() - j - 1][image.size() - i - 1] = image[i][j];
    }
  }
  image = new_image;
}

// the tile has to match the edge passed in along a particular orientation, ie,
// edge_dir so if the edge passed in is a right edge of the prev tile, the
// edge_dir should be 3, for the right edge of the current tile 0 = up, 1 =
// right, 2 = left and 3 = right
void OrientTileToMatchOnSide(const std::string& edge, Tile& cur_tile,
                             const int edge_dir) {
  for (int i = 0; i < 4; ++i) {
    if (edge == cur_tile.edges[edge_dir]) {
      return;
    } else {
      cur_tile.Rotate();
    }
  }
  cur_tile.Flip();
  for (int i = 0; i < 4; ++i) {
    if (edge == cur_tile.edges[edge_dir]) {
      return;
    } else {
      cur_tile.Rotate();
    }
  }
}

bool backtrack(const int puzzle_size, const int count,
               std::vector<std::vector<int>>& grid,
               std::unordered_map<std::string, std::vector<int>>& edge_map,
               std::unordered_map<int, Tile>& tiles) {
  if (count == tiles.size()) return true;
  const int row = count / puzzle_size;
  const int col = count % puzzle_size;
  if (col == 0) {  // row and col is never 0 as set the tile for 0,0 before
                   // calling backtrack
    const auto& bottom_edge = tiles[grid[row - 1][col]].getBottomEdge();
    const auto& ids = edge_map[bottom_edge];
    for (const auto id : ids) {
      if (id == grid[row - 1][0] || tiles[id].used) continue;
      auto& cur_tile = tiles[id];
      cur_tile.used = true;
      grid[row][col] = cur_tile.id;
      OrientTileToMatchOnSide(bottom_edge, cur_tile, 0);
      if (backtrack(puzzle_size, count + 1, grid, edge_map, tiles)) {
        return true;
      } else {
        cur_tile.used = false;
        grid[row][col] = -1;
      }
    }
  } else if (row == 0) {
    const auto& right_edge = tiles[grid[row][col - 1]].getRightEdge();
    const auto& ids = edge_map[right_edge];
    for (const auto id : ids) {
      if (id == grid[row][col - 1] || tiles[id].used) continue;
      auto& cur_tile = tiles[id];
      cur_tile.used = true;
      grid[row][col] = cur_tile.id;
      OrientTileToMatchOnSide(right_edge, cur_tile, 3);
      if (backtrack(puzzle_size, count + 1, grid, edge_map, tiles)) {
        return true;
      } else {
        cur_tile.used = false;
        grid[row][col] = -1;
      }
    }
  } else if (row != 0) {
    const auto& bottom_edge = tiles[grid[row - 1][col]].getBottomEdge();
    const auto& ids = edge_map[bottom_edge];
    for (const auto id : ids) {
      if (id == grid[row - 1][col] || tiles[id].used) continue;
      auto& cur_tile = tiles[id];
      cur_tile.used = true;
      grid[row][col] = cur_tile.id;
      OrientTileToMatchOnSide(bottom_edge, cur_tile, 0);
      const auto& right_edge = tiles[grid[row][col - 1]].getRightEdge();
      if (cur_tile.edges[3] == right_edge &&
          backtrack(puzzle_size, count + 1, grid, edge_map, tiles)) {
        return true;
      } else {
        cur_tile.used = false;
        grid[row][col] = -1;
      }
    }
  }
  return false;
}

long long CheckForPattern(const std::vector<std::string>& complete_image,
                          const std::vector<std::string>& pattern) {
  std::size_t max_row_size = 0;
  std::vector<std::vector<int>> relevant_points(pattern.size());
  for (int i = 0; i < pattern.size(); i++) {
    for (int j = 0; j < pattern[i].size(); j++) {
      max_row_size = std::max(max_row_size, pattern[i].size());
      if (pattern[i][j] == '#') {
        relevant_points[i].push_back(j);
      }
    }
  }
  long long count = 0;
  for (int i = 0; i < complete_image.size() - relevant_points.size(); i++) {
    for (int j = 0; j < complete_image[i].size() - max_row_size; j++) {
      bool match = true;
      for (int pi = 0; pi < relevant_points.size(); pi++) {
        for (int pj = 0; pj < relevant_points[pi].size(); pj++) {
          if (complete_image[i + pi][j + relevant_points[pi][pj]] != '#') {
            match = false;
            break;
          }
        }
        if (!match) break;
      }
      if (match) count++;
    }
  }
  return count;
}

int main() {
  std::fstream file{"../input/day_20_input"};
  std::string line;
  Tile new_tile;
  std::unordered_map<int, Tile> tiles;
  std::unordered_map<std::string, std::vector<int>> edge_map;
  while (std::getline(file, line)) {
    line.erase(std::remove_if(std::begin(line), std::end(line),
                              [](const char c) { return !isprint(c); }),
               std::end(line));
    if (line == "") {
      new_tile.createEdges();
      tiles.insert({new_tile.id, new_tile});
      new_tile = Tile();
      continue;
    } else if (line.substr(0, 4) == "Tile") {
      new_tile.id = std::stoi(line.substr(5, line.size() - 5));
    } else {
      new_tile.image.push_back(line);
    }
  }

  if (new_tile.id != -1) {
    new_tile.createEdges();
    tiles.insert({new_tile.id, new_tile});
  }

  for (auto& [tile_id, tile] : tiles) {
    for (auto& edge : tile.edges) {
      if (edge_map.find(edge) == edge_map.end()) {
        edge_map.insert({edge, {tile_id}});
        std::string r_edge(std::rbegin(edge), std::rend(edge));
        edge_map.insert({r_edge, {tile_id}});
      } else {
        edge_map[edge].push_back(tile_id);
        std::string r_edge(std::rbegin(edge), std::rend(edge));
        edge_map[r_edge].push_back(tile_id);
      }
    }
  }

  // Create Border
  for (const auto& [edge, ids] : edge_map) {
    for (int i = 0; i < ids.size(); i++) {
      for (int j = i + 1; j < ids.size(); j++) {
        tiles[ids[i]].neighbour_ids.insert(ids[j]);
        tiles[ids[j]].neighbour_ids.insert(ids[i]);
      }
    }
  }

  std::unordered_set<int> corner_ids;
  for (auto& [id, tile] : tiles) {
    if (tile.neighbour_ids.size() == 2) {
      corner_ids.insert(id);
    }
  }

  bool found = false;
  long long count = 1;
  long long puzzle_size = std::sqrt(tiles.size());
  std::vector<std::vector<int>> grid(puzzle_size,
                                     std::vector<int>(puzzle_size, -1));
  // Using the corners as starting points, though the algorithm will work
  // starting form any tile
  for (const auto corner_id : corner_ids) {
    grid[0][0] = corner_id;
    tiles[corner_id].used = true;
    for (int i = 0; i < 4; ++i) {
      if (backtrack(puzzle_size, count, grid, edge_map, tiles)) {
        found = true;
        break;
      }
      tiles[corner_id].Rotate();
    }
    if (found) break;
    tiles[corner_id].Flip();
    for (int i = 0; i < 4; ++i) {
      if (backtrack(puzzle_size, count, grid, edge_map, tiles)) {
        found = true;
        break;
      }
      tiles[corner_id].Rotate();
    }
    if (found)
      break;
    else
      tiles[corner_id].used = false;
  }

  // Create complete image and remove edges
  std::vector<std::string> complete_image(
      (tiles[grid[0][0]].image.size() - 2) * (puzzle_size), "");
  for (int i = 0; i < puzzle_size; i++) {
    for (int j = 0; j < puzzle_size; j++) {
      tiles[grid[i][j]].applyRotationsAndFlipToImage();
      const auto& tile_img = tiles[grid[i][j]].image;
      for (int k = 0; k < (tile_img.size() - 2); k++) {
        complete_image[i * (tile_img.size() - 2) + k] +=
            tile_img[k + 1].substr(1, tile_img[k].size() - 2);
      }
    }
  }

  const std::vector<std::string> pattern = {
      "                  # ", "#    ##    ##    ###", " #  #  #  #  #  #   "};

  long long monster_size = 0;
  for (const auto& row : pattern) {
    for (const auto& ele : row) {
      if (ele == '#') {
        monster_size++;
      }
    }
  }

  long long n_monsters = 0;
  for (int i = 0; i < 2; i++) {    // flip
    for (int j = 0; j < 4; j++) {  // rotation
      n_monsters = CheckForPattern(complete_image, pattern);
      if (n_monsters > 0) break;
      RotateImage(complete_image);
    }
    if (n_monsters > 0) break;
    FlipImage(complete_image);
  }

  long long roughness = 0;
  for (int i = 0; i < complete_image.size(); i++) {
    for (int j = 0; j < complete_image[i].size(); j++) {
      if (complete_image[i][j] == '#') {
        roughness++;
      }
    }
  }

  roughness -= n_monsters * monster_size;
  std::cout << roughness << '\n';
  return roughness;
}
