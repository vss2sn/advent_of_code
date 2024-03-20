#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue> 
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

// IDEA:
// Convert the maze to a graph where 
// nodes in the graph are points with more than 2 adjacent points that are not forests  
// edges in the graph connect 2 nodes (no nodes in the middle of an edge)
// Run dfs on the graph

struct Point {
  int row;
  int col;
  std::size_t dist = 0;

  Point(const int row = 0, const int col = 0) : row(row), col(col) {}

  Point operator + (const Point& p) const {
    return Point (p.row + row,  p.col + col);
  }

  bool operator == (const Point& p) const {
    return p.row == row && p.col == col;
  }
};

struct Edge {
  Point start;
  Point end;
  std::size_t dist;
};

struct hasher_point {
  std::size_t operator() (const Point& p) const {
    return p.row + p.col;
  }
};


const std::vector<Point> motions ={
  Point(-1,0),
  Point(0,1),
  Point(1,0),
  Point(0, -1),
};

bool in_limits(const std::vector<std::string>& map, const int row, const int col) {
  return row >= 0 && row < map.size() && col >= 0 && col < map[0].size();
}

void dfs(std::stack<Point>& stack,  
         std::unordered_map<Point, std::vector<Edge>, hasher_point>& node_to_edges_map, 
         std::size_t& max_dist, 
         const Point& dest, 
         std::vector<std::vector<bool>>& seen) {
  if (!stack.empty()) {
    const Point current = stack.top();
    seen[current.row][current.col] = true;
    if (current == dest) {
      max_dist = std::max(current.dist, max_dist);
      seen[current.row][current.col] = false;
    } else{
      for (const auto& edge : node_to_edges_map[current]) {
        auto next = edge.end;
        next.dist = current.dist + edge.dist;
        if(!seen[next.row][next.col]) {
          stack.push(next);
          dfs(stack, node_to_edges_map, max_dist, dest, seen);
          stack.pop();
        }
      }
    }
    seen[current.row][current.col] = false;
  }
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_23_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<std::string> map;
  while(std::getline(file, line)) {
    map.emplace_back(line);
  }

  Point start;
  start.row = 0;
  for (int col = 0; col < map[0].size(); col++) {
    if (map[0][col] == '.') {
      start.col = col;
      break;
    }
  }

  Point dest;
  dest.row = map.size() - 1;
  for (int col = 0; col < map[dest.row].size(); col++) {
    if (map[dest.row][col] == '.') {
      dest.col = col;
      break;
    }
  }
  
  // IDEA:
  // Convert the maze to a graph where 
  // nodes in the graph are points with more than 2 adjacent points that are not forests  
  // edges in the graph connect 2 nodes (no nodes in the middle of an edge)
  // Run dfs on the graph
  
  // Find nodes
  std::stack<Point> stack;
  std::vector<std::vector<bool>> seen(map.size(), std::vector<bool>(map[0].size(), false));
  std::unordered_set<Point, hasher_point> nodes{start, dest}; // Assumes start and end have only 1 open adjacent point
  stack.push(start);
  while (!stack.empty()) {
    const Point current = stack.top();
    stack.pop();
    seen[current.row][current.col] = true;
    int count = 0;
    for (const auto& motion : motions) {
      const auto next = current + motion;
      if(in_limits(map, next.row, next.col) && map[next.row][next.col] != '#') {
        // std::cout << "Considering: " << next.row << ',' << next.col << '\n';
        count++;
        if(!seen[next.row][next.col]) stack.push(next);
      } 
    }
    if (count > 2) {
      nodes.insert(current);
      // std::cout << "NODE: " << current.row << ',' << current.col << '\n';
    }
  }

  // Find the length of each edge in the graph where an edge connects 2 nodes
  // There are no nodes in the middle of an edge
  std::unordered_map<Point, std::vector<Edge>, hasher_point> node_to_edges_map;
  for (const auto& node : nodes) {
    seen = std::vector<std::vector<bool>> (map.size(), std::vector<bool>(map[0].size(), false));
    while(!stack.empty()) stack.pop();
    stack.push(node);
    while (!stack.empty()) {
      const Point current = stack.top();
      stack.pop();
      seen[current.row][current.col] = true;
      for (const auto& motion : motions) {
        auto next = current + motion;
        next.dist = current.dist + 1;
        if(in_limits(map, next.row, next.col) && map[next.row][next.col] != '#' && !seen[next.row][next.col]) {
          if (nodes.find(next) == nodes.end()) {
            stack.push(next);
          } else{
            Edge e;
            e.start = node;
            e.end = next;
            e.dist = next.dist;
            node_to_edges_map[node].push_back(e);
            e.start = next;
            e.end = node;
            node_to_edges_map[node].push_back(e);
          }
        } 
      }
    }
  }

  // Run DFS on the nodes using the precalculated edge distances
  seen = std::vector<std::vector<bool>> (map.size(), std::vector<bool>(map[0].size(), false));
  stack.push(start);
  std::size_t max_dist = 0;
  dfs(stack, node_to_edges_map, max_dist, dest, seen);
  std::cout << max_dist << '\n';
  return 0;
}