#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set> 

enum class Type {
    WALL, SPACE
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

std::array<int, 31> get_bit_masks () {
    std::array<int, 31> bit_masks;
    int bit_mask = 1;
    for (int i = 0; i < 31; i++) {
        bit_masks[i] = bit_mask;
        bit_mask <<= 1;
    }
    return bit_masks;
}

struct Point {
    Point (const int x, const int y) : x(x), y(y) {}
    int x;
    int y;
};

struct PointAndMove {
    PointAndMove(const int x = 0, const int y = 0, const int move = 0) : p(Point(x, y)), move(move) {}
    Point p;
    int move;
    bool operator == (const PointAndMove& pm) const {
        return p.x == pm.p.x && p.y == pm.p.y;
    }
};

struct pm_hasher {
    std::size_t operator () (const PointAndMove& pm) const {
        return pm.p.x << 5 + pm.p.y;
    }
};

int main(int argc, char* argv[]) {
  std::string input = "../input/day_13_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  std::getline(file, line);
  const int designers_fav_n = std::stoi(line);
  const std::array<int, 31> bit_masks = get_bit_masks();
  const auto get_type = [designers_fav_n, &bit_masks](const int y, const int x) {
    const int n = x*x + 3*x + 2*x*y + y + y*y + designers_fav_n;
    int count = 0;
    for (const auto& bit_mask : bit_masks) {
        // std::cout << bit_mask << '\n';
        count += ((n & bit_mask) != 0) ? 1 : 0;
    }
    // std::cout << '(' << x << ',' << y << " | " << n << " | " << count << " | " << static_cast<int>((count % 2 == 0) ? Type::SPACE : Type::WALL) << '\n'; 
    return (count % 2 == 0) ? Type::SPACE : Type::WALL;
  };
  const std::array<Point, 4> moves {
    Point(0,1),
    Point(1,0),
    Point(0,-1),
    Point(-1, 0)
  };
  const auto in_bounds = [](const Point& p) {
    return p.x >= 0 && p.y >= 0;
  };
  std::unordered_set<PointAndMove, pm_hasher> seen;
  std::queue<PointAndMove> q;
  PointAndMove pm;
  pm.p = Point(1,1);
  pm.move = 0;
  q.push(pm);
  // for (int i = 0; i < 10; i++) {
  //   for (int j = 0; j < 10; j++) {
  //       std::cout << ((get_type(i, j) == Type::WALL) ? '#' :  '.');
  //   }
  //   std::cout << '\n';
  // }
  while(!q.empty()) {
    const auto current = q.front();
    q.pop();
    if (seen.find(current) != seen.end()) continue;
    seen.insert(current);
    if (current.p.x == 39 && current.p.y == 31) {
        // std::cout << "Found" << '\n';
        std::cout << current.move << '\n';
        return 0;
    }
    for (const auto& move : moves) {
        PointAndMove next;
        next.p = Point(current.p.x + move.x, current.p.y + move.y);
        next.move = current.move + 1;
        if (!in_bounds(next.p)) continue;
        if (get_type(next.p.x, next.p.y) == Type::WALL) continue;
        if (seen.find(next) != seen.end()) continue;
        q.push(next);
    }
  }
  std::cout << "Not found " << '\n';
  return 0;
}