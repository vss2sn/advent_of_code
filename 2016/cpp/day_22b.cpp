#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <queue> 
#include <ranges>
#include <regex>
#include <unordered_set> 
#include <vector>  
#include <iomanip>

// Solve by hand.
// Code to print map then replicate solve by hand solution 

struct Node {
    int x;
    int y;
    int size;
    int used;
    int avail;
    int p_used;
    std::string name;
};

struct NodeMove {
  Node node;
  int move;
  bool operator == (const NodeMove& nm) const {
    return nm.node.x == node.x && nm.node.y == node.y;
  }
};

struct NodeMoveHash {
  std::size_t operator () (const NodeMove& nm) const {
    return nm.node.x + nm.node.y * 100;
  }
};

int sv_to_int (const std::string_view sv) {
  bool negative = (sv[0] == '-') ? true : false;
  int n = 0;
  for (int i = negative ? 1 : 0; i < sv.size(); i++) {
    n = n * 10 + (sv[i] - '0');
  }
  if (negative) {
    n *= -1;
  }
  return n;
}

const std::array<Node, 4> moves {
  Node(0, 1, 0, 0, 0, 0, ""),
  Node(1, 0, 0, 0, 0, 0, ""),
  Node(0, -1, 0, 0, 0, 0, ""),
  Node(-1, 0, 0, 0, 0, 0, "")
};

int search (const Node& start_node, const Node& goal_node, const std::vector<Node>& nodes, const int max_x, const int max_y) {
  NodeMove start;
  start.node = start_node;
  start.move = 0;
  std::unordered_set<NodeMove, NodeMoveHash> seen;
  std::queue<NodeMove> q;
  q.push(start);
  while (!q.empty()) {
    const auto current = q.front();
    q.pop();
    if (seen.find(current) != seen.end()) continue;
    seen.insert(current);
    if (current.node.x == goal_node.x && current.node.y == goal_node.y) {
      return current.move;
    }
    for (const auto& move : moves) {
      Node next;
      next.x = current.node.x + move.x;
      next.y = current.node.y + move.y;
      if (next.x < 0 || next.x > max_x || next.y < 0 || next.y > max_y) continue;
      next = *std::find_if(std::begin(nodes), std::end(nodes), [&next](const auto& node) {return node.x == next.x && node.y == next.y; });
      if (next.used > 99) continue; // Assume obstacle
      NodeMove next_nm;
      next_nm.node = next;
      next_nm.move = current.move + 1;
      if (seen.find(next_nm) != seen.end()) continue;

      q.push(next_nm);
    }
  }
  return -1;
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_22_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  std::getline(file, line);
  std::getline(file, line);
  std::vector<Node> nodes;
  const std::regex pattern(R"((/dev/grid/node-x([0-9]+)-y([0-9]+))[ ]+([0-9]+)T[ ]+([0-9]+)T[ ]+([0-9]+)T[ ]+([0-9]+)%)");
  int max_x = 0;
  int max_y = 0;
  while(std::getline(file, line)) {
    // std::cout << line << '\n';
    std::smatch match;
    std::regex_search(line, match, pattern);
    // for(const auto ele : match) {
    //   std::cout << ele << ' ';
    // }
    // std::cout << '\n';
    Node node;
    node.name = match[1];
    node.x = std::stoi(match[2]);
    max_x = std::max(node.x, max_x);
    node.y = std::stoi(match[3]);
    max_y = std::max(node.y, max_y);
    node.size = std::stoi(match[4]);
    node.used = std::stoi(match[5]);
    node.avail = std::stoi(match[6]);
    node.p_used = std::stoi(match[7]);
    nodes.push_back(node);
  }
  auto map = std::vector<std::vector<std::pair<int, int>>>(max_y+1, 
                std::vector<std::pair<int, int>>(
                    max_x+1, {0, 0}
                )
            );
  for (const auto& node : nodes) {
    map[node.y][node.x] = {node.used, node.size};
  }
  for (const auto& row : map) {
    for (const auto& ele : row) {
        std::cout << std::setw(3) << ele.first ;//<< "|" << ele.second;
    }
    std::cout << '\n';
  }

  Node empty;
  Node start;
  Node goal;
  for (const auto& node : nodes) {
    if (node.used == 0) { // This might be a small value?
      empty = node;
    } else if (node.x == max_x && node.y == 0) {
      start = node;
    } else if (node.x == 0 && node.y == 0) {
      goal = node;
    }
  }

  // Get the empty node to the top right corner
  // Then move the data in the top right corner to the top left
  // The assumption here is that to move the data one step, the empty node will need a minimum of 5 moves.
  // Start:   |  Move 1:  |  Move 2:  |  Move 3:  |  Move 4:  |  Move 5:  | Repeat    
  // . D E .  |  . D . .  |  . D . .  |  . D . .  |  E D . .  |  D E . .  | 
  // . . . .  |  . . E .  |  . E . .  |  E . . .  |  . . . .  |  . . . .  |  
  // Number of steps -1 as this is not necessary in the first move when moving data out of the top right corner
  // Assume that the minimum is always possible.
  std::cout << search(empty, start, nodes, max_x, max_y) + (search (start, goal, nodes, max_x, max_y) - 1 ) * 5 << '\n';
  return 0;
}

