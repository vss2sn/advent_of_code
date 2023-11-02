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

int main(int argc, char* argv[]) {
  const std::string input = (argc > 1) ? argv[1] : "../input/day_22_input" ;  
  std::ifstream file(input);
  std::string line;
  std::unordered_set<Coord2D, hasher> infected;
  int n_cols = 0;
  int n_rows = 0;
  while(std::getline(file, line)) {
    if (n_cols == 0) {
      n_cols = line.size(); 
    }
    for (int i = 0; i < line.size(); i++) {
      if (line[i] == '#') {
        infected.insert(Coord2D(n_rows, i));
      }
    }
    n_rows++;
  }
  int count = 0;
  int dir = 0;
  auto current = Coord2D((n_rows)/2, (n_cols)/2);
  // std::cout << 
  for (int iteration = 0; iteration < 10000; iteration++) {
    // std::cout << "Iteration: " << iteration << " --> Infected: " << count << '\n';
    // if (iteration == 7) {
    //   std::cout << iteration << ": " << count << '\n';
    //   exit(0);
    // } else 
    // if (iteration == 70) {
    //   std::cout << iteration << ": " << count << '\n';
    //   exit(0);
    // } 
    if (infected.find(current) != infected.end()) {
      // std::cout << "Current node (" << current.row << ", " << current.col << ") is infected" << '\n';
      dir--;
      dir += 4;
      dir %= 4;
      infected.erase(current);
    } else {
      // std::cout << "Current node (" << current.row << ", " << current.col << ") is NOT infected" << '\n';
      dir += 1;
      dir %= 4;
      infected.insert(current);
      count++;
    }
    // std::cout << "It is now " << ((infected.find(current) != infected.end()) ? "infected" : "not infected") << '\n';
    move(current, dir);
  }
  std::cout << count << '\n';

  return 0;
}