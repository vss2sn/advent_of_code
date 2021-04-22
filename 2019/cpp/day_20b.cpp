#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>

// TODO(vss): Add const correctness for all maps by using const iterator instead of [] operator
//            Valid for previous days as well

struct hash_pair {
  template <class T1, class T2>
  size_t operator()(const std::pair<T1, T2>& p) const {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);
    return hash1 ^ hash2;
  }
};

struct State {
  // TODO(vss): Add constructor
  std::pair<int, int> p{0,0};
  int level = 0;
  int cost = 0;
  // std::vector<std::string> order;
  bool operator == (const State& s) const {
    return s.p == p && s.level == level;
  }
};

struct hash_state {
  size_t operator()(const State& s) const {
    const auto hash1 = std::hash<int>{}(s.p.first);
    const auto hash2 = std::hash<int>{}(s.p.second);
    const auto hash3 = std::hash<int>{}(s.level);
    return hash1 ^ hash2 ^ hash3;
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
  // // std::cout << grid.size() << ' ' << grid[current.first].size() << '\n';
  return current.first >=0 && current.first < grid.size() && current.second >=0 && current.second <= grid[current.first].size();
}

std::pair<int, int> getSecondKeyCoord(const std::pair<int, int>& current, const std::vector<std::vector<char>>& grid) {
  for (const auto & neighbour : getNeighbours(current)) {
    if (inBounds(neighbour, grid)) {
      if (isupper(grid[neighbour.first][neighbour.second])) {
        return neighbour;
      }
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

void Print(const State& s, const std::string& str = "State") {
  std::cout << str << ": (" << s.p.first << ", " << s.p.second << ") --- " << s.level << '\n';
}

std::vector<std::tuple<std::pair<int, int>, int, int>>  GetReachablePortalsAtSameLevel(const std::vector<std::vector<char>>& map,
  std::unordered_map<std::string, std::vector<std::pair<int, int>>>& teleport_paths,
  std::unordered_map<std::pair<int, int>, std::string, hash_pair>& path_inverse,
  const std::pair<int, int>& start
) {
  std::queue<std::pair<int, int>> points;
  std::unordered_set<std::pair<int, int>, hash_pair> visited;
  std::unordered_map<std::pair<int, int>, int, hash_pair> costs;
  std::unordered_map<std::pair<int, int>, std::pair<int, int>, hash_pair> parent;
  std::vector<std::tuple<std::pair<int, int>, int, int>> reachable;

  points.push(start);
  costs.insert({start, 0});
  visited.insert(start);
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
    }
    if (path_inverse.find(current) != path_inverse.end() && current != start) {
      if (current.second == 2 || current.second == map[current.first].size() - 3 || current.first == 2 || current.first == map.size() - 3) {
        reachable.emplace_back(current, -1, costs[current]);
      } else {
        reachable.emplace_back(current, +1, costs[current]);
      }
    }
  }
  return reachable;
}

long long FindPath(
  const std::vector<std::vector<char>>& map,
  std::unordered_map<std::string, std::vector<std::pair<int, int>>>& teleport_paths,
  std::unordered_map<std::pair<int, int>, std::string, hash_pair>& path_inverse,
  std::unordered_map<std::pair<int, int>, std::vector<std::tuple<std::pair<int, int>, int, int>>, hash_pair>& reachables) {

  State start;
  start.p = teleport_paths["AA"][0];
  start.level = 0;
  start.cost = 0;

  State dest;
  dest.p = teleport_paths["ZZ"][0];
  dest.level = 0;

  std::queue<State> q;
  std::unordered_set<State, hash_state>  visited;
  q.push(start);

  while(!q.empty()) {
    const auto current = q.front();
    q.pop();
    visited.insert(current);
    for (const auto& [n, delta_l, cost] : reachables[current.p]) {
      State neighbour;
      neighbour.p = n;
      neighbour.level = current.level;
      neighbour.cost = current.cost + cost;
      // neighbour.order = current.order;
      // neighbour.order.emplace_back(path_inverse[current.p]);
      if (visited.find(neighbour) != visited.end()) continue;
      q.push(neighbour);
      if (neighbour == dest) {
        return current.cost + cost;
      }
      if (neighbour.level == 0 && delta_l == -1) continue;
      if (neighbour.p == start.p || neighbour.p == dest.p) continue;
      const auto n_complement = (n == teleport_paths[path_inverse[n]][0]) ? teleport_paths[path_inverse[n]][1] : teleport_paths[path_inverse[n]][0];
      State neighbour_complement;
      neighbour_complement.p = n_complement;
      neighbour_complement.level = current.level + delta_l;
      neighbour_complement.cost = current.cost + cost + 1;
      // neighbour_complement.order = neighbour.order;
      q.push(neighbour_complement);
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
    input_line.erase(std::remove_if(std::begin(input_line), std::end(input_line), [](auto c) { return !isprint(c); }), std::end(input_line));
    grid.emplace_back(input_line.begin(), input_line.end());
  }

  // Solve
  auto [teleport_paths, path_inverse] = GetTeleportationPaths(grid);
  std::unordered_map<std::pair<int, int>, std::vector<std::tuple<std::pair<int, int>, int, int>>, hash_pair> reachables;
  for (const auto& [p, s] : path_inverse) {
    reachables[p] = GetReachablePortalsAtSameLevel(grid, teleport_paths, path_inverse, p);
  }
  long long steps = FindPath(grid, teleport_paths, path_inverse, reachables);
  std::cout << steps << '\n';
  return steps;
}
