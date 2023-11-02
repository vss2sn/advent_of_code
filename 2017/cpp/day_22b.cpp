#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <regex>
#include <cassert>   
#include <cmath>

struct Coord2D {
  int row;
  int col;

  Coord2D(const int row = 0, const int col = 0) : row(row) , col(col) {}

  Coord2D operator + (const Coord2D& c) const {
    Coord2D ans;
    ans.row = row + c.row;
    ans.col = col + c.col;
    return ans;
  }

bool operator == (const Coord2D& c) const {
  return row == c.row && col == c.col;
}



  Coord2D operator - (const Coord2D& c) const {
    Coord2D ans;
    ans.row = row - c.row;
    ans.col = col - c.col;
    return ans;
  }

  Coord2D operator += (const Coord2D& c) {
    row += c.row;
    col += c.col;
    return *this;
  }
};

struct hasher {
  std::size_t operator()(const Coord2D& c) const {
    return 1000000 * c.row + c.col;
  }
};

const std::vector<Coord2D> motion {
  Coord2D(-1,0),
  Coord2D(0,-1),
  Coord2D(1,0),
  Coord2D(0,1),
};

void move(Coord2D& current, const int dir) {
  current.row += motion[dir].row;
  current.col += motion[dir].col;
}

enum class State {
  CLEAN,
  WEAKENED,
  INFECTED,
  FLAGGED
};

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_22_input" ;  
  std::ifstream file(input);
  std::string line;
  std::unordered_map<Coord2D, State, hasher> state;
  int n_cols = 0;
  int n_rows = 0;
  while(std::getline(file, line)) {
    if (n_cols == 0) {
      n_cols = line.size(); 
    }
    for (int i = 0; i < line.size(); i++) {
      if (line[i] == '#') {
        state.insert({Coord2D(n_rows, i), State::INFECTED});
      }
    }
    n_rows++;
  }
  int count = 0;
  int dir = 0;
  auto current = Coord2D((n_rows)/2, (n_cols)/2);
  for (int iteration = 0; iteration < 10000000; iteration++) {
    // std::cout << "Iteration: " << iteration << " --> Infected: " << count << '\n';
    // if (iteration == 100) {
    //   std::cout << iteration << ": " << count << '\n';
    //   exit(0);
    // } 
    if (auto it = state.find(current); it != state.end()) {
      if (it->second == State::INFECTED) {
        // std::cout << "Current node (" << current.row << ", " << current.col << ") is infected" << '\n';
        dir--;
        dir += 4;
        dir %= 4;
        it->second = State::FLAGGED;
        // std::cout << "Current node (" << current.row << ", " << current.col << ") is now flagged" << '\n';
      } else if (it->second == State::WEAKENED) {
        // std::cout << "Current node (" << current.row << ", " << current.col << ") is weakened" << '\n';
        it->second = State::INFECTED;
        count++;
        // std::cout << "Current node (" << current.row << ", " << current.col << ") is now infected" << '\n';
      } else if (it->second == State::FLAGGED) {
        // std::cout << "Current node (" << current.row << ", " << current.col << ") is flagged" << '\n';
        dir += 2;
        dir %= 4;
        it->second = State::CLEAN;
        state.erase(it);
        // std::cout << "Current node (" << current.row << ", " << current.col << ") is now cleaned" << '\n';
      }
    } else { // CLEAN
      // std::cout << "Current node (" << current.row << ", " << current.col << ") is clean" << '\n';
      dir += 1;
      dir %= 4;
      state.insert({current, State::WEAKENED});
      // std::cout << "Current node (" << current.row << ", " << current.col << ") is now weakened" << '\n';
    }
    // std::cout << "Moving " << dir << '\n';
    move(current, dir);
  }
  std::cout << count << '\n';

  return 0;
}