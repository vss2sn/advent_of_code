#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <bitset>
#include <queue>
#include <vector>

struct hash_pair {
  template <class T1, class T2>
  size_t operator()(const std::pair<T1, T2>& p) const {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);
    return hash1 ^ hash2;
  }
};

std::vector<std::pair<int, int>> getNeighbours(const std::pair<int, int>& p) {
  std::vector<std::pair<int, int>> neighbours;
  neighbours.emplace_back(p.first + 1, p.second);
  neighbours.emplace_back(p.first - 1, p.second);
  neighbours.emplace_back(p.first, p.second + 1);
  neighbours.emplace_back(p.first, p.second - 1);
  return neighbours;
}

bool inBounds(const std::pair<int, int>& current, const std::vector<std::vector<char>>& grid) {
  // std::cout << grid.size() << ' ' << grid[current.first].size() << '\n';
  return current.first >=0 && current.first < grid.size() && current.second >=0 && current.second <= grid[current.first].size();
}

std::pair<int, int> getSecondKeyCoord(const std::pair<int, int>& current, const std::vector<std::vector<char>>& grid) {
  for (const auto & neighbour : getNeighbours(current)) {
    // std::cout << neighbour.first << ", " << neighbour.second << '\n';
    if (inBounds(neighbour, grid)) {
      // std::cout << neighbour.first << ", " << neighbour.second << ' ' << grid[neighbour.first][neighbour.second] << '\n';
      if (isupper(grid[neighbour.first][neighbour.second])) {
        return neighbour;
      }
    } else {
      // std::cout << "out " << '\n';
    }
  }
  return {-1,-1};
}

std::pair<int, int> getMazePointofKey(
  const std::vector<std::vector<char>>& grid,
  const std::pair<int, int>& p1,
  const std::pair<int, int>& p2) {
  for (const auto & neighbour : getNeighbours(p1)) {
    if (inBounds(neighbour, grid)) {
      if (grid[neighbour.first][neighbour.second] == '.') {
        return neighbour;
      }
    }
  }
  for (const auto & neighbour : getNeighbours(p2)) {
    if (inBounds(neighbour, grid)) {
      if (grid[neighbour.first][neighbour.second] == '.') {
        return neighbour;
      }
    }
  }
  return {-1,-1};
}

std::tuple<
  std::unordered_map<std::string, std::vector<std::pair<int, int>>>,
  std::unordered_map<std::pair<int, int>, std::string, hash_pair>
>
GetTeleportationPaths(const std::vector<std::vector<char>>& grid) {
  std::unordered_map<std::string, std::vector<std::pair<int, int>>> paths;
  std::unordered_map<std::pair<int, int>, std::string, hash_pair> path_inverse;
  std::unordered_map<std::pair<int, int>, std::string, hash_pair> coor_to_name;
  for (int row = 0; row < grid.size(); row++) {
    for (int col = 0; col < grid[row].size(); col++) {
      if (isupper(grid[row][col])) {
        const auto key_p1 = std::make_pair(row, col);
        if (coor_to_name.find(key_p1) != coor_to_name.end()) continue;
        std::pair<int, int> key_p2 = getSecondKeyCoord(key_p1, grid);
        std::string key_s = std::string(1, grid[row][col]) + std::string(1, grid[key_p2.first][key_p2.second]);
        coor_to_name.insert({key_p1, key_s});
        coor_to_name.insert({key_p2, key_s});
        const auto connetion_to_maze = getMazePointofKey(grid, key_p1, key_p2);
        if (paths.find(key_s) == paths.end()) {
          paths.insert({key_s, {}});
        }
        paths[key_s].push_back(connetion_to_maze);
        path_inverse[connetion_to_maze]=key_s;
      }
    }
  }
  return {paths, path_inverse};
}

long long DijkstraWithTeleport(const std::vector<std::vector<char>>& map,
  std::unordered_map<std::string, std::vector<std::pair<int, int>>>& teleport_paths,
  std::unordered_map<std::pair<int, int>, std::string, hash_pair>& path_inverse
) {
  std::queue<std::pair<int, int>> points;
  std::unordered_set<std::pair<int, int>, hash_pair> visited;
  std::unordered_map<std::pair<int, int>, int, hash_pair> costs;
  const auto start = teleport_paths["AA"][0];
  std::cout << "(" << start.first << ", " << start.second << ")" << '\n';
  // exit(0);
  const auto dest = teleport_paths["ZZ"][0];
  points.push(start);
  costs.insert({start, 0});

  while (!points.empty()) {
    const auto current = points.front();
    points.pop();
    visited.insert(current);
    for (const auto& neighbour : getNeighbours(current)) {
      if (!inBounds(neighbour, map)) continue;
      if (map[neighbour.first][neighbour.second] != '.') continue;
      if (visited.find(neighbour) != visited.end()) continue;
      points.push(neighbour);
      costs.insert({neighbour, costs[current] + 1});
      if (neighbour == dest) return costs[current] + 1;
    }
    std::cout << "Current: " << current.first << ' ' << current.second << '\n';
    if (path_inverse.find(current) != path_inverse.end()) {
      const auto& s = path_inverse[current];
      const auto& opts = teleport_paths[s];
      std::cout << "teleport possible" << '\n';
      std::pair<int, int> neighbour;
      if (current == opts[0]) {
        neighbour = opts[1];
      } else {
        neighbour = opts[0];
      }
      if (!inBounds(neighbour, map)) continue;
      if (map[neighbour.first][neighbour.second] != '.') continue;
      if (visited.find(neighbour) != visited.end()) continue;
      points.push(neighbour);
      costs.insert({neighbour, costs[current] + 1});
      if (neighbour == dest) return costs[current] + 1;
    }
  }
  return 0;
}


int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_20_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string input_line;
  std::vector<std::vector<char>> grid;

  while(std::getline(file, input_line)) {
    input_line.erase(std::remove_if(std::begin(input_line), std::end(input_line),
                              [](auto c) { return !isprint(c); }),
               std::end(input_line));
    grid.emplace_back(input_line.begin(), input_line.end());
    for(const char c : grid.back()) {
      std::cout << c;
    }
    std::cout << '\n';
  }

  // std::unordered_map<std::string, std::array<std::pair<int, int>, 2>>
  auto [teleport_paths, path_inverse] = GetTeleportationPaths(grid);
  // for (const auto& [key_s, locs] : teleport_paths) {
  //   std::cout << key_s << ": ";
  //   for (const auto& loc : locs) {
  //     std::cout << "(" << loc.first << ", " << loc.second << ")" << ' ';
  //   }
  //   std::cout << '\n';
  // }
  //
  for (const auto& [p, key_s] : path_inverse) {
    std::cout << "(" << p.first << ", " << p.second << ")" << ": ";
    std::cout << key_s;
    std::cout << '\n';
  }

  const long long steps = DijkstraWithTeleport(grid, teleport_paths, path_inverse);
  std::cout << steps << '\n';
  return steps;
}
