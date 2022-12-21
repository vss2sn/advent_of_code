#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using Point = std::array<int, 3>;

struct PointHash {
  std::size_t operator () (const Point& p) const {
    std::size_t ans = 0;
    for (const auto ele : p) {
      ans = ans * 10 + ele;
    }
    return ans;
  }
};

int main(int argc, char * argv[]) {
  std::string input = "../input/day_18_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::string line;
  std::fstream file(input);

  std::unordered_map<Point, int, PointHash> points;

  const auto get_neighbours = [] (const Point& p) {
    return std::array<Point, 6>{{
      Point{{p[0]-1, p[1], p[2]}},
      Point{{p[0]+1, p[1], p[2]}},
      Point{{p[0], p[1]-1, p[2]}},
      Point{{p[0], p[1]+1, p[2]}},
      Point{{p[0], p[1], p[2]-1}},
      Point{{p[0], p[1], p[2]+1}}
    }};
  };

  while(std::getline(file, line)) {
    std::size_t start = 0;
    std::size_t end = line.find(',');
    Point p;
    int index = 0;
    while(end != std::string::npos) {
      p[index] = std::stoi(line.substr(start, end - start));
      start = end + 1;
      end = line.find(',', start);
      index++;
    }
    p[index] = std::stoi(line.substr(start, end - start));
    points[p] = 0;
    for (const auto& neighbour: get_neighbours(p)) {
      if (const auto it = points.find(neighbour); it != points.end()) {
        points[p]++;
        it->second += 1;
      }
    }
  }
  int total_sa = points.size() * 6;
  for (const auto& [point, n_neighbours] : points) {
    total_sa -= n_neighbours;
  }
  std::cout << total_sa << '\n';
  return 0;
}
