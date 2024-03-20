#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue> 
#include <string>
#include <unordered_set>
#include <vector>
#include <cassert> 
#include <memory>

constexpr int n = 64;

struct PointState {
  int row;
  int col;
  int step;

  PointState(const int row = 0, const int col = 0, const int step = 0) : row(row), col(col), step(step) {}

  PointState operator + (const PointState& p) const {
    return PointState (p.row + row,  p.col + col, p.step + step);
  }

  bool operator == (const PointState& p) const {
    return p.row == row && p.col == col && p.step == step;
  }
};

struct hasher {
  std::size_t operator() (const PointState& ps) const {
    return ps.row + ps.col;
  }
};

const std::vector<PointState> motions {
  PointState(-1,0,1),
  PointState(0,1,1),
  PointState(1,0,1),
  PointState(0, -1,1),
};

int main(int argc, char * argv[]) {
  std::string input = "../input/day_21_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<std::string> map;
  int step = 0;
  PointState S;
  while(std::getline(file, line)) {
    map.push_back(line);
    for (int i = 0; i < line.size(); i++) {
      if (line[i] == 'S') {
        S.row = map.size() - 1;
        S.col = i;
        S.step = 0;
      }
    }
  }
  std::queue<PointState> queue;
  std::unordered_set<PointState, hasher> seen;
  std::unordered_set<PointState, hasher> end_states;
  queue.push(S);
  while(!queue.empty()) {
    const auto current = queue.front();
    queue.pop();
    if (seen.find(current) != seen.end()) {
      continue;
    }
    if (current.step > n) continue;
    if (current.step == n) end_states.insert(current);
    seen.insert(current);
    for (const auto& motion : motions) {
      const auto new_state = current + motion;
      if (new_state.row >= map.size() || new_state.row < 0 || new_state.col < 0 || new_state.col >= map[0].size()) {
        continue;
      }
      if (map[new_state.row][new_state.col] == '#') {
        continue;
      }
      queue.push(new_state);
    }
  }
  for (const auto & end_state : end_states) {
    map[end_state.row][end_state.col] = 'O';
  }
  int count = 0;
  for (const auto& row : map) {
    std::cout << row << '\n';
    for (const auto c : row) {
      if (c == 'O') {
        count++;
      }
    }
  }
  std::cout << count << '\n';
  return 0;
}