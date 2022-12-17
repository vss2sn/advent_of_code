#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// #include <cassert>

// Probably faster comparisons at teh end if sorted vectors are used instead of sets

struct NodeHasher {
  std::size_t operator () (const std::string& node) const {
    return (node[0] - 'A') * 26 + (node[1] - 'A');
  }
};

using Path = std::unordered_set<std::string, NodeHasher>;

struct PathFlowPair {
  PathFlowPair(const Path& path, const int flow) : path(path), flow(flow) {}
  Path path;
  int flow;
};


struct PathHasher {
  std::size_t operator () (const Path& path) const {
    size_t ans = 0;
    for (const auto& ele : path) {
      ans = ans * 26 + (ele[0] - 'A') * 26 + (ele[1] - 'A');
    }
    return ans;
  }
};

struct NodeDistPair {
  NodeDistPair(const std::string& node, const int dist) : node(node), dist(dist) {}
  NodeDistPair& operator = (const NodeDistPair&) = default;
  // NodeDistPair&& operator = (NodeDistPair&&) = default;
  std::string node;
  int dist;

  bool operator < (const NodeDistPair& np) const {
    return dist < np.dist;
  }

  bool operator > (const NodeDistPair& np) const {
    return dist > np.dist;
  }

  bool operator == (const NodeDistPair& np) const {
    return node == np.node;
  }
};

struct Comparator  {
  bool operator() (const NodeDistPair& np1, const NodeDistPair& np2) const {
    return np1.dist > np2.dist;
  }
};

std::unordered_map<std::string, int> find_distances(
  std::unordered_map<std::string, std::vector<std::string>>& map,
  const std::string source
) {
  std::unordered_map<std::string, int> distances;
  std::priority_queue<NodeDistPair, std::vector<NodeDistPair>, Comparator> pq;
  std::unordered_set<std::string> visited;
  pq.push(NodeDistPair(source, 0));
  while(!pq.empty()) {
    const auto current = pq.top();
    // // std::cout<< "Popped " << current.node << ", " << current.dist << '\n';
    pq.pop();
    if (visited.find(current.node) != visited.end()) continue;
    visited.insert(current.node);
    distances[current.node] = current.dist;
    for (const auto& next_node : map[current.node]) {
      // // std::cout<< "Pushing " << next_node << ", " << current.dist + 1 << '\n';
      pq.push(NodeDistPair(next_node, current.dist + 1));
    }
  }
  // for (const auto & [next_node, nvm] : map) {
  //   assert(distances.find(next_node) != distances.end());
  // }
  return distances;
}

void print_path(const Path& path) {
  for (const auto& node : path) {
    std::cout << node << ' ';
  }
  std::cout << '\n';
}

void dfs(
  Path& path,
  std::unordered_map<Path, int, PathHasher>& paths,
  std::unordered_set<std::string>& visited,
  std::unordered_map<std::string, int>& flow_rates,
  std::unordered_map<std::string, std::vector<std::string>>& map,
  const std::string& current,
  const int time,
  std::unordered_map<std::string, std::unordered_map<std::string, int>>& distances,
  int flow = 0
) {
  if (time == 0) {
    if (auto it = paths.find(path); it == paths.end() || it->second < flow) {
      // print_path(path);
      // std::cout << flow << '\n';
      paths[path] = flow;
    }
    return;
  }
  flow += flow_rates[current] * (time);
  for (const auto& [next_node, next_dist] : distances[current]) {
    if (visited.find(next_node) != visited.end()) continue;
    if (time - next_dist - 1 < 0) {
      // print_path(path);
      // // std::cout << flow << '\n';
      if (auto it = paths.find(path); it == paths.end() || it->second < flow) {
        // print_path(path);
        // std::cout << flow << '\n';
        paths[path] = flow;
      }
      continue;
    }
    if (flow_rates[next_node] == 0) continue;
    visited.insert(next_node);
    path.insert(next_node);
    dfs(path, paths, visited, flow_rates, map, next_node, time - next_dist - 1, distances, flow);
    visited.erase(next_node);
    path.erase(next_node);
  }
  if (auto it = paths.find(path); it == paths.end() || it->second < flow) {
    // print_path(path);
    // std::cout << flow << '\n';
    paths[path] = flow;
  }
  flow -= flow_rates[current] * (time);
  return;
}

std::unordered_map<Path, int,PathHasher> get_all_paths(
  std::unordered_map<std::string, int> flow_rates,
  std::unordered_map<std::string, std::vector<std::string>> map,
  const std::string& source,
  const int time,
  std::unordered_map<std::string, std::unordered_map<std::string, int>>& distances
) {
  std::unordered_map<Path, int,PathHasher> paths;
  Path path;
  // path.insert(source);
  std::unordered_set<std::string> visited;
  visited.insert(source);
  dfs(path, paths, visited, flow_rates, map, source, time, distances);
  return paths;
}

std::vector<std::string> extract_valves(const std::string& valves_line) {
  std::vector<std::string> valves;
  size_t start = 0;
  size_t end = valves_line.find(", ");
  while (end != std::string::npos) {
    valves.push_back(valves_line.substr(start, end - start));
    start = end + 2;
    end = valves_line.find(", ", start);
  }
  valves.push_back(valves_line.substr(start, end - start));
  return valves;
}

// int calc_flow(const Path& path, int time, std::unordered_map<std::string, int>& flow_rates,
//   std::unordered_map<std::string, std::unordered_map<std::string, int>>& distances
// ) {
//   int score = 0;
//   for (int i = 0; i < path.size() - 1; i++) {
//     const auto prev = path[i];
//     const auto next = path[i+1];
//     time = time - (distances[prev][next]) - 1;
//     score += flow_rates[next  ] * time;
//   }
//   return score;
// }

int main(int argc, char * argv[]) {
  std::string input = "../input/day_16_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  const std::regex pattern(R"(Valve ([A-Z]+) has flow rate=([0-9]+); ([a-z ]+) ([A-Z, ]+))");
  std::vector<std::string> nodes;
  std::unordered_map<std::string, int> flow_rates;
  std::unordered_map<std::string, std::vector<std::string>> map;

  while (std::getline(file, line)) {
    if (line == "") continue;
    std::smatch match;
    std::regex_match(line, match, pattern);
    const auto start_valve = match[1];
    nodes.push_back(start_valve);
    const auto flow_rate = std::stoi(match[2]);
    flow_rates[start_valve] = flow_rate;
    if (match[3] == "tunnel leads to valve") {
      map[start_valve] = std::vector<std::string>{match[4]};
    } else {
      map[start_valve] = extract_valves(match[4]);
    }
    // // std::cout<< start_valve << ": ";
    // for (const auto ele : map[start_valve]) {
    //   // std::cout<< ele << ' ';
    // }
    // // std::cout<< '\n';
  }

  std::unordered_map<std::string, std::unordered_map<std::string, int>> distances;
  for (const auto & node : nodes) {
    distances[node] = find_distances(map, node);
  }

  // Print graph
  // for (const auto& [start_node, distance] : distances) {
  //   for (const auto& [node, dist] : distance) {
  //     // std::cout<< "(" << node << ", " <<  dist <<  ")" << ' ';
  //   }
  //   // std::cout<< '\n';
  // }

  // Sanity check; triangle inequality
  // for (const auto& [start_node, distance] : distances) {
  //   for (const auto& [node, dist] : distance) {
  //     assert (dist == distances[node][start_node]);
  //     for (const auto& [node2, dist2] : distances[node]) {
  //       assert (dist <= distances[node][node2] + distances[node2][start_node]);
  //     }
  //   }
  // }

  const std::string start = "AA";
  const int time = 26;
  auto paths = get_all_paths(flow_rates, map, start, time, distances);
  bool no_intersection = true;
  int max_flow = 0;
  for (const auto& [path1, flow1] : paths) {
    for (const auto& [path2, flow2] : paths) {
      no_intersection = true;
      for (const auto& ele : path1) {
        if (path2.find(ele) != path2.end()) {
          no_intersection = false;
          break;
        }
      }
      if (no_intersection) {
        max_flow = std::max(max_flow, flow1 + flow2);
      }
    }
  }
  std::cout << max_flow << '\n';
  return 0;
}
