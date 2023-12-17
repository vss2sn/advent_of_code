#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue> 
#include <string>
#include <unordered_set>
#include <vector>

struct Point {
  int row;
  int col;

  Point(const int row, const int col) : row(row), col(col) {}

  Point operator + (const Point& p) const {
    return Point (p.row + row,  p.col + col);
  }

  bool operator == (const Point& p) const {
    return p.row == row && p.col == col;
  }
};

struct State {
  Point position = Point(0,0);
  int direction = 0;
  int moves_in_dir =  0;
  std::size_t heat_loss = 0;

  bool operator == (const State& state) const {
    return state.position == position && state.moves_in_dir == moves_in_dir && state.direction == direction; 
    // heat loss should not need a comparison as using a priority queue
  }

  friend std::ostream& operator << (std::ostream& os, const State& s);
};

std::ostream& operator << (std::ostream& os, const State& s) {
  os << "(" << s.position.row << "," << s.position.col << ") " << s.direction << ' ' << s.moves_in_dir;
  return os;
}

struct Comparator  {
  bool operator() (const State& s1, const State& s2) const {
    return s1.heat_loss > s2.heat_loss;
  }
};

struct hasher {
  std::size_t operator() (const State& state) const {
    return state.position.row + state.position.col;
  }
};

const std::vector<Point> motions ={
  Point(-1,0),
  Point(0,1),
  Point(1,0),
  Point(0, -1),
};

bool in_limits(const std::vector<std::vector<int>>& map, const int row, const int col) {
  return row >= 0 && row < map.size() && col >= 0 && col < map[0].size();
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_17_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<std::vector<int>> map;
  while(std::getline(file, line)) {
    map.emplace_back();
    for (const auto c : line) {
      map.back().emplace_back(c - '0');
    }
  }

  std::priority_queue<State, std::vector<State>, Comparator> pq;
  std::unordered_set<State, hasher> seen;
  State state;
  state.position = Point(0,0);
  state.direction = 1;
  state.moves_in_dir = 0;
  state.heat_loss = 0;
  pq.push(state);
  state.direction = 2;
  pq.push(state);
  const auto destination = Point(map.size()  - 1, map[0].size() -1);
  while(!pq.empty()) {
    state = pq.top();
    // std::cout << state.position.row << ' ' << state.position.col << '\n';
    pq.pop();
    if (seen.find(state) != seen.end())  {
      continue;
    }

    seen.insert(state);
    if (state.position == destination) {
      std::cout << state.heat_loss << '\n';
      return 0;
    }
    // Move left
    {
      State new_state;
      new_state.direction = (state.direction - 1 + 4) % 4;
      new_state.position = state.position + motions[new_state.direction];
      if (in_limits(map, new_state.position.row, new_state.position.col)) {
        new_state.heat_loss  = state.heat_loss + map[new_state.position.row][new_state.position.col];
        new_state.moves_in_dir = 0;
        pq.push(new_state);
      }
    }
    // Move right
    {
      State new_state;
      new_state.direction = (state.direction + 1 + 4) % 4;
      new_state.position = state.position + motions[new_state.direction];
      if (in_limits(map, new_state.position.row, new_state.position.col)) {
        new_state.heat_loss  = state.heat_loss + map[new_state.position.row][new_state.position.col];
        new_state.moves_in_dir = 0;
        pq.push(new_state);
      }
    }
    // Move Straight
    {
      if (state.moves_in_dir < 2) {
        State new_state = state;
        new_state.position = state.position + motions[state.direction];
        if (in_limits(map, new_state.position.row, new_state.position.col)) {
          new_state.heat_loss  = state.heat_loss + map[new_state.position.row][new_state.position.col];
          new_state.moves_in_dir++;
          pq.push(new_state);
        }
      }
    }
  }
  std::cout << "The destination was not reached." << '\n';
  return 0;
}