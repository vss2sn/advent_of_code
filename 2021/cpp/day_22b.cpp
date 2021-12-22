#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

// Idea:
// Check if a an instruction is to switch on
// Check every instruction after that for an overlap
// If overlap exists, create lists of overlap
// If overlap in overlap list, create list of overlaps of overlap
// And so on
// Substract the number of points at each depth from the previous depth to get total number of points unchanged

struct Point {
  long long x, y, z;
  Point(const long long x, const long long y, const long long z) : x(x), y(y), z(z) {}
  bool operator == (const Point& p) const {
    return x == p.x && y == p.y && z == p.z;
  }

  Point operator + (const Point& p) const {
    return Point(x + p.x, y + p.y, z + p.z);
  }

  Point operator - (const Point& p) const {
    return Point(x - p.x, y - p.y, z - p.z);
  }

  bool operator < (const Point& p) const {
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

void print(const std::vector<Point>& v) {
  for (const auto& ele : v) {
    std::cout << ele << '\n';
  }
  std::cout << '\n';
}

struct Cuboid {
  Point p1 = Point(0,0,0);
  Point p2 = Point(0,0,0);
  bool on = false;

  long long get_n_points() const {
    return (std::abs(p1.x - p2.x) + 1) * (std::abs(p1.y - p2.y) + 1) * (std::abs(p1.z - p2.z) + 1);
  }
};


std::tuple<bool, Cuboid> overlap(const Cuboid& c1, const Cuboid& c2) {
  // Leaving as is for readability
  if (c1.p2.x < c2.p1.x) return {false, Cuboid()};
  if (c1.p1.x > c2.p2.x) return {false, Cuboid()};
  const auto x1 = std::max(c1.p1.x, c2.p1.x);
  const auto x2 = std::min(c1.p2.x, c2.p2.x);

  if (c1.p2.y < c2.p1.y) return {false, Cuboid()};
  if (c1.p1.y > c2.p2.y) return {false, Cuboid()};
  const auto y1 = std::max(c1.p1.y, c2.p1.y);
  const auto y2 = std::min(c1.p2.y, c2.p2.y);

  if (c1.p2.z < c2.p1.z) return {false, Cuboid()};
  if (c1.p1.z > c2.p2.z) return {false, Cuboid()};
  const auto z1 = std::max(c1.p1.z, c2.p1.z);
  const auto z2 = std::min(c1.p2.z, c2.p2.z);

  if (x1 <= x2 && y1 <= y2 && z1 <= z2) {
    auto c = Cuboid();
    c.p1 = Point(x1, y1, z1);
    c.p2 = Point(x2, y2, z2);
    return {true, c};
  }
  return {false, Cuboid()};
}

long long count_unchanged(const Cuboid& c, const std::vector<Cuboid>& cuboids, const int next_index) {
  long long total_area = c.get_n_points();
  std::vector<Cuboid> sub_cuboids_changed;
  sub_cuboids_changed.reserve(cuboids.size() - next_index + 1);

  for (int i = next_index; i < cuboids.size(); i++) {
    const auto [overlap_found, overlap_c] = overlap(c, cuboids[i]);
    if (!overlap_found) continue;
    sub_cuboids_changed.push_back(overlap_c);
  }

  for (int sub_c_i = 0; sub_c_i < sub_cuboids_changed.size(); sub_c_i++) {
    const auto& c = sub_cuboids_changed[sub_c_i];
    total_area -= count_unchanged(c, sub_cuboids_changed,sub_c_i+1);
  }

  return total_area;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_22_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  const std::regex mask_pattern(R"(([onf]+) x=([-0123456789]+)..([-0123456789]+),y=([-0123456789]+)..([-0123456789]+),z=([-0123456789]+)..([-0123456789]+))");

  std::vector<Cuboid> instructions;
  while (std::getline(file, line)) {
    std::smatch mask_match;
    std::regex_search(line, mask_match, mask_pattern);
    const bool on = (std::string(mask_match[1]) == "on");
    auto c = Cuboid();
    c.p1 = Point(std::stoll(mask_match[2]), std::stoll(mask_match[4]), std::stoll(mask_match[6]));
    c.p2 = Point(std::stoll(mask_match[3]), std::stoll(mask_match[5]), std::stoll(mask_match[7]));
    c.on = on;
    instructions.push_back(c);
  }

  long long n_cubes_on = 0;
  for (int step = 0; step < instructions.size(); step++) {
    auto& c = instructions[step];
    if (!c.on) {
      continue;
    }
    n_cubes_on += count_unchanged(c, instructions, step+1);
  }

  std::cout << n_cubes_on << '\n';

  return 0;
}
