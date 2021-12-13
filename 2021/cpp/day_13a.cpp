#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

// Can use a tree to optimize this for the general case,
// or a counter instead of a bool

struct Point {
  int row, col;
  Point (const int row, const int col) : row(row), col(col) {}
  friend std::ostream& operator << (std::ostream& os, const Point& p);
  bool operator == (const Point& p) const {
    return row == p.row && col == p.col;
  }
};

std::ostream& operator << (std::ostream& os, const Point& p) {
  os << "(" << p.row << ", " << p.col << ")";
  return os;
}

struct hash_point {
  std::size_t operator () (const Point& p) const {
    return p.row << 5 + p.col;
  }
};

int main(int argc, char * argv[]) {
  std::string input = "../input/day_13_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::vector<Point> marked;
  while(std::getline(file, line)) {
    if(line.size() != 0) {
      const auto d = line.find(",");
      marked.emplace_back(std::stoi(line.substr(0, d)), std::stoi(line.substr(d + 1, line.size() - d)));
    } else {
      break;
    }
  }

  std::getline(file, line);
  if (line[11] == 'x') {
    for (auto& p : marked) {
      const auto x = std::stoi(line.substr(13, line.size() - 13));
      if (p.row > x) {
        p.row = x - (p.row - x);
      }
    }
  } else {
    for (auto& p : marked) {
      const auto y = std::stoi(line.substr(13, line.size() - 13));
      if (p.col > y) {
        p.col = y - (p.col - y);
      }
    }
  }

  std::unordered_set<Point, hash_point> s;
  // Apparently better as using the default constructer of unordered_set
  // as that constructs the element before checking if it already exists
  for (const auto& p : marked) {
    s.insert(p);
  }
  std::cout << s.size() << '\n';
  return 0;
}
