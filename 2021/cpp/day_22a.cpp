#include <fstream>
#include <iostream>
#include <regex>

#include <unordered_set>

struct Point {
  int x, y, z;
  Point(const int x, const int y, const int z) : x(x), y(y), z(z) {}
  bool operator == (const Point& p) const {
    return x == p.x && y == p.y && z == p.z;
  }

  Point operator + (const Point& p) const {
    return Point(x + p.x, y + p.y, z + p.z);
  }

  Point operator - (const Point& p) const {
    return Point(x - p.x, y - p.y, z - p.z);
  }

  bool operator < (const Point& p) {
    return (z < p.z) ||
      (z == p.z && y < p.y) ||
      (z == p.z && y == p.y && x < p.x);
  }
  friend std::ostream& operator << (std::ostream& os, const Point& p);
};

std::ostream& operator << (std::ostream& os, const Point& p) {
  os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
  return os;
}

struct hash_point {
  std::size_t operator () (const Point& p) const {
    return p.z * 1000 * 1000 + p.y * 1000 + p.x;
  }
};

void print(const std::vector<Point>& v) {
  for (const auto& ele : v) {
    std::cout << ele << '\n';
  }
  std::cout << '\n';
}


int main(int argc, char * argv[]) {
  std::string input = "../input/day_22_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::unordered_set<Point, hash_point> pts;

  const std::regex mask_pattern(R"(([onf]+) x=([-0123456789]+)..([-0123456789]+),y=([-0123456789]+)..([-0123456789]+),z=([-0123456789]+)..([-0123456789]+))");

  while (std::getline(file, line)) {
    std::smatch mask_match;

    std::regex_search(line, mask_match, mask_pattern);
    const bool on = (std::string(mask_match[1]) == "on");
    if (std::stoi(mask_match[2]) < -50 || 50 <= std::stoi(mask_match[3])) {
      continue;
    }

    for (int x = std::stoi(mask_match[2]); x <= std::stoi(mask_match[3]); x++) {
      for (int y = std::stoi(mask_match[4]); y <= std::stoi(mask_match[5]); y++) {
        for (int z = std::stoi(mask_match[6]); z <= std::stoi(mask_match[7]); z++) {
          if (on) pts.insert(Point(x, y, z));
          else pts.erase(Point(x, y, z));
        }
      }
    }
  }
  std::cout << pts.size() << '\n';
  return 0;
}
