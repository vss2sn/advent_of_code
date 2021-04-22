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

constexpr int n_keys = 26;

struct Point {
  Point (const int x = 0, const int y = 0, const int dist = 0, const long long path = 0, const std::vector<char>& order = {}) : x(x), y(y), dist(dist), path(path), order(order) {}
  Point (const Point&) = default;
  Point (Point&&) = default;
  Point& operator=(const Point&) = default;
  Point& operator=(Point&&) = default;

  bool operator==(const Point& p) {
    return p.x == x && p.y ==y;
  }

  int x;
  int y;
  int dist;
  long long path;
  std::vector<char> order;
};

std::vector<Point> getMoves() {
  std::vector<Point> moves;
  moves.emplace_back(+1, 0);
  moves.emplace_back(-1, 0);
  moves.emplace_back(0, +1);
  moves.emplace_back(0, -1);
  return moves;
}

bool inBounds(const Point& current, const std::vector<std::vector<char>>& map) {
  return current.y >=0 && current.y < map.size() && current.x >=0 && current.x <= map[0].size();
}

std::vector<Point> Dijkstra(const std::vector<std::vector<char>>& map, const Point& start) {
  std::queue<Point> points;
  std::set<std::tuple<int, int>> visited;
  std::vector<Point> reachable;
  points.push(start);

  while (!points.empty()) {
    const auto current = points.front();
    points.pop();
    visited.insert({current.x, current.y});
    for (const auto& move : getMoves()) {
      const Point neighbour(current.x + move.x, current.y + move.y, current.dist + 1);
      if (!inBounds(neighbour, map)) continue;
      if (map[neighbour.y][neighbour.x] == '#') continue;
      if (visited.find({neighbour.x, neighbour.y}) != visited.end()) continue;
      if (map[neighbour.y][neighbour.x] == '.') {
        points.push(neighbour);
      } else if (map[neighbour.y][neighbour.x] != '#') {
        if (std::find(reachable.begin(), reachable.end(), neighbour) == reachable.end()) {
          reachable.push_back(neighbour);
        }
      }
    }
  }
  return reachable;
}

struct CompareDist {
  bool operator()(const Point& p1, const Point& p2) {
    return p1.dist > p2.dist;
  }
};

long long BFS(const std::vector<std::vector<char>>& map, const Point& start, std::map<char, std::vector<Point>>& reachables) {
  // State of point = x, y, and order of previously visited points, ie path
  std::priority_queue<Point,std::vector<Point>, CompareDist> q;
  std::set<std::tuple<int, int, long long>> visited; // Need unique state
  q.push(start);
  visited.insert({start.x, start.y, start.path});
  int count = 0;
  while(!q.empty()) {
    const auto cur = q.top();
    q.pop();
    std::bitset<n_keys> path = cur.path;
    if (path.all()) {
      return cur.dist;
    }
    for(const auto& reachable_point : reachables[map[cur.y][cur.x]]) {
      auto temp = cur.order;
      temp.emplace_back(map[cur.y][cur.x]);

      Point new_pt(reachable_point.x, reachable_point.y, cur.dist + reachable_point.dist, cur.path, temp);
      if (visited.find({new_pt.x, new_pt.y, new_pt.path}) != visited.end()) continue;
      if (isupper(map[new_pt.y][new_pt.x]) && !(path[map[new_pt.y][new_pt.x] - 'A'])) continue;
      bool changed = false;
      if (islower(map[new_pt.y][new_pt.x]) && !path[map[new_pt.y][new_pt.x] - 'a']) {
        changed = true;
        path[map[new_pt.y][new_pt.x] - 'a'] = 1;
      }
      visited.insert({new_pt.x, new_pt.y, new_pt.path});
      new_pt.path = path.to_ullong();
      q.push(new_pt);
      if (changed) {
        path[map[new_pt.y][new_pt.x] - 'a'] = 0;
      }
    }
  }
  return 0;
}

int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_18_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string input_line;
  std::vector<std::vector<char>> map;
  std::map<char, Point> keys_and_doors;
  Point start;
  while(std::getline(file, input_line)) {
    map.emplace_back(input_line.begin(), input_line.end());
    for (int i = 0; i < input_line.size(); i++) {
      if (input_line[i] == '@') {
        start.x = i;
        start.y = map.size() - 1;
      } else if (input_line[i] != '.' && input_line[i] != '#') {
          keys_and_doors.insert({input_line[i], Point(i, map.size() - 1)});
      }
    }
  }

  // Solve
  std::map<char, std::vector<Point>> reachables;
  reachables.insert({'@', Dijkstra(map, start)});
  for (const auto [c, pc] : keys_and_doors) {
    reachables.insert({c, Dijkstra(map, pc)});
  }
  const long long min_steps = BFS(map, start, reachables);
  std::cout << min_steps << '\n';
  return min_steps;
}
