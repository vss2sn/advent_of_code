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

struct Node_dist_pair {
  Node_dist_pair(const std::string& node, const int dist) : node(node), dist(dist) {}
  Node_dist_pair& operator = (const Node_dist_pair&) = default;
  // Node_dist_pair&& operator = (Node_dist_pair&&) = default;
  std::string node;
  int dist;

  bool operator < (const Node_dist_pair& np) const {
    return dist < np.dist;
  }

  bool operator > (const Node_dist_pair& np) const {
    return dist > np.dist;
  }

  bool operator == (const Node_dist_pair& np) const {
    return node == np.node;
  }
};

struct Comparator  {
  bool operator() (const Node_dist_pair& np1, const Node_dist_pair& np2) const {
    return np1.dist > np2.dist;
  }
};

std::unordered_map<std::string, int> find_distances(
  std::unordered_map<std::string, std::vector<std::string>>& map,
  const std::string source
) {
  std::unordered_map<std::string, int> distances;
  std::priority_queue<Node_dist_pair, std::vector<Node_dist_pair>, Comparator> pq;
  std::unordered_set<std::string> visited;
  pq.push(Node_dist_pair(source, 0));
  while(!pq.empty()) {
    const auto current = pq.top();
    // std::cout<< "Popped " << current.node << ", " << current.dist << '\n';
    pq.pop();
    if (visited.find(current.node) != visited.end()) continue;
    visited.insert(current.node);
    distances[current.node] = current.dist;
    for (const auto& next_node : map[current.node]) {
      // std::cout<< "Pushing " << next_node << ", " << current.dist + 1 << '\n';
      pq.push(Node_dist_pair(next_node, current.dist + 1));
    }
  }
  // for (const auto & [next_node, nvm] : map) {
  //   assert(distances.find(next_node) != distances.end());
  // }
  return distances;
}

void print_path(const std::vector<std::string>& path) {
  for (const auto& node : path) {
    std::cout << node << ' ';
  }
  std::cout << '\n';
}

void dfs(std::vector<std::string>& path, std::vector<std::vector<std::string>>& paths,
  std::unordered_set<std::string>& visited, std::unordered_map<std::string, int>& flow_rates,
  std::unordered_map<std::string, std::vector<std::string>>& map, const std::string& current, const int time,
  std::unordered_map<std::string, std::unordered_map<std::string, int>>& distances) {
  if (time == 0) {
    // print_path(path);
    paths.push_back(path);
    return;
  }
  for (const auto& [next_node, next_dist] : distances[current]) {
    if (visited.find(next_node) != visited.end()) continue;
    if (time - next_dist - 1 < 0) {
      // print_path(path);
      paths.push_back(path);
      continue;
    }
    if (flow_rates[next_node] == 0) continue;
    visited.insert(next_node);
    path.push_back(next_node);
    dfs(path, paths, visited, flow_rates, map, next_node, time - next_dist - 1, distances);
    visited.erase(next_node);
    path.pop_back();
  }
  // print_path(path);
  paths.push_back(path);
  return;
}

std::vector<std::vector<std::string>> get_all_paths(
  std::unordered_map<std::string, int> flow_rates,
  std::unordered_map<std::string, std::vector<std::string>> map,
  const std::string& source,
  const int time,
  std::unordered_map<std::string, std::unordered_map<std::string, int>>& distances
) {
  std::vector<std::vector<std::string>> paths;
  std::vector<std::string> path{source};
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

int calc_flow(const std::vector<std::string>& path, int time, std::unordered_map<std::string, int>& flow_rates,
  std::unordered_map<std::string, std::unordered_map<std::string, int>>& distances
) {
  int score = 0;
  for (int i = 0; i < path.size() - 1; i++) {
    // std::cout<< path[i] << ' ';
    const auto prev = path[i];
    const auto next = path[i+1];
    time = time - (distances[prev][next]) - 1;
    score += flow_rates[next  ] * time;
  }
  // std::cout<< '\n';
  return score;
}

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
    // std::cout<< start_valve << ": ";
    // for (const auto ele : map[start_valve]) {
    //   std::cout<< ele << ' ';
    // }
    // std::cout<< '\n';
  }

  std::unordered_map<std::string, std::unordered_map<std::string, int>> distances;
  for (const auto & node : nodes) {
    distances[node] = find_distances(map, node);
  }
  // for (const auto& [start_node, distance] : distances) {
  //   std::cout<< start_node << ": ";
  //   for (const auto& [node, dist] : distance) {
  //     std::cout<< "(" << node << ", " <<  dist <<  ")" << ' ';
  //   }
  //   std::cout<< '\n';
  // }

  // for (const auto& [start_node, distance] : distances) {
  //   for (const auto& [node, dist] : distance) {
  //     assert (dist == distances[node][start_node]);
  //     for (const auto& [node2, dist2] : distances[node]) {
  //       assert (dist <= distances[node][node2] + distances[node2][start_node]);
  //     }
  //   }
  // }
  // exit  (0);
  const std::string start = "AA";
  const int time = 30;
  const auto paths = get_all_paths(flow_rates, map, start, time, distances);
  int max_flow = 0;
  int max_path = -1;
  for (int i = 0; i < paths.size(); i++) {
    const auto new_flow = calc_flow(paths[i], time, flow_rates, distances);
    if (new_flow > max_flow) {
      max_path = i;
      max_flow = new_flow;
    }
  }

  // std::cout << '\n';
  // print_path(paths[max_path]);
  std::cout << max_flow << '\n';
  return 0;
}
