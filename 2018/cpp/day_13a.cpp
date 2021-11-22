#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Point {
  int x, y;

  Point& operator += (const Point& v) { x += v.x; y += v.y; return *this; }
  Point& operator -= (const Point& v) { x -= v.x; y -= v.y; return *this; }
};
bool operator == (const Point& a, const Point& b) { return a.y == b.y && a.x == b.x; }
bool operator != (const Point& a, const Point& b) { return a.y != b.y || a.x != b.x; }
bool operator <  (const Point& a, const Point& b) { return (a.y != b.y) ? (a.y < b.y) : (a.x < b.x); }


static constexpr std::array<Point, 4> Dir = {
  Point{  0, -1 }, // north
  Point{  1,  0 }, // east
  Point{  0,  1 }, // south
  Point{ -1,  0 }, // west
};

static constexpr int North = 0;
static constexpr int East  = 1;
static constexpr int South = 2;
static constexpr int West  = 3;

struct Cart {
  Point pos;
  int dir;
  int next_turn = -1;

  void turn() {
    dir = (dir + 4 + next_turn) & 0x3;
    next_turn = ((next_turn + 2) % 3) - 1;
  }

  void move() {
    pos += Dir[dir];
  }
};


bool order_by_pos(const Cart& a, const Cart& b)
{
  if (a.pos != b.pos)
    return (a.pos < b.pos);
  return (a.dir != b.dir) ? (a.dir < b.dir) : (a.next_turn < b.next_turn);
}


struct Map {
  std::vector<std::string> grid;
  std::vector<Cart> carts;

  // Returns index of the first cart which crashed, or -1 if no crashes yet.
  int tick() {
    std::sort(carts.begin(), carts.end(), order_by_pos);
    for (int i = 0; i < carts.size(); i++) {
      carts[i].move();
      switch (grid[carts[i].pos.y][carts[i].pos.x]) {
      case '/':  carts[i].dir ^= 1; break;
      case '\\': carts[i].dir ^= 3; break;
      case '+':  carts[i].turn(); break;
      default: break;
      }
      for (int j = 0; j < carts.size(); j++) {
        if (j != i && carts[j].pos == carts[i].pos) {
          return i;
        }
      }
    }
    return -1;
  }
};


int main(int argc, char* argv[]) {
  std::string input = "../input/day_13_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string line;

  Map map;
  while (std::getline(file, line)) {
    map.grid.push_back(line);
  }

  Point pos;
  for (pos.y = 0; pos.y < map.grid.size(); pos.y++) {
    for (pos.x = 0; pos.x < map.grid[pos.y].size(); pos.x++) {
      switch (map.grid[pos.y][pos.x]) {
      case '^':
        map.carts.push_back(Cart{ pos, North, -1 });
        map.grid[pos.y][pos.x] = '|';
        break;
      case '>':
        map.carts.push_back(Cart{ pos, East, -1 }); break;
        map.grid[pos.y][pos.x] = '-';
        break;
      case 'v':
        map.carts.push_back(Cart{ pos, South, -1 }); break;
        map.grid[pos.y][pos.x] = '|';
        break;
      case '<':
        map.carts.push_back(Cart{ pos, West, -1 }); break;
        map.grid[pos.y][pos.x] = '-';
        break;
      default:
        break;
      }
    }
  }

  int crashed = map.tick();
  while (crashed == -1) {
    crashed = map.tick();
  }
  std::cout << map.carts[crashed].pos.x << "," << map.carts[crashed].pos.y << '\n';
  return 0;
}
