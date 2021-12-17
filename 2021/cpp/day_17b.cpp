#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <unordered_set>
#include <vector>

struct Point {
  int y, x;
  Point(const int x, const int y) : x(x), y(y) {}
  bool operator == (const Point& p) const {
    return y == p.y && x == p.x;
  }
  bool operator < (const Point& p) const {
    return y < p.y || (y == p.y  && x < p.x);
  }
  friend std::ostream& operator << (std::ostream& os, const Point& p);
};

std::ostream& operator << (std::ostream& os, const Point& p) {
  os << "(" << p.y << ", " << p.x << ")";
  return os;
}

struct hash_point {
  std::size_t operator () (const Point& p) const {
    return p.y * p.x;
  }
};

void print(const std::vector<Point>& v) {
  for (const auto& ele : v) {
    std::cout << ele << '\n';
  }
  std::cout << '\n';
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_17_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::getline(file, line);
  const std::regex mask_pattern(R"(target area: x=([-0123456789]+)..([-0123456789]+), y=([-0123456789]+)..([-0123456789]+))");
  std::smatch mask_match;
  std::regex_search(line, mask_match, mask_pattern);

  std::vector<Point> velocity_components;
  // Iterate over every x coordinate
  for (auto x = std::stoi(mask_match[1]); x <= std::stoi(mask_match[2]); x++) {
    // Minimum x velocity needed to reach target x cordinate
    // vx + (vx-1) + (vx-2) ... = x at time steps 1, 2, 3, ...
    // => n * vx - ((n-1) * n)/2 = x ... equ(1)
    int min_vx = x;
    while ((min_vx * (min_vx + 1)) > (2 * std::abs(x))) {
      min_vx--;
    }
    std::vector<std::pair<int, int>> vx_n_values;
    std::vector<int> inf_drops;
    for(auto vx = min_vx; vx <= x; vx++) {
      int  n = 1; // n = number of steps
      while (n < vx) {
        // Using equ (1) check whether at time step n,
        // the distance travelled is exactly = x
        if (x == (vx * (n) - (n * (n-1)) / 2)) {
          vx_n_values.emplace_back(vx, n);
        }
        n++;
      }
      // When n = vx + 1, the velocity reaches 0, based on equ (1)
      // After that it remains 0, so check whether once the velocity is 0,
      // the distance travelled = the x coordinate
      if (2 * x == (vx * (vx+1))) {
        inf_drops.emplace_back(vx);
      }
    }

    // For a given pair of x-velocities and # of steps (vx, n)
    // check if the y coordinate reached is one of the y coordinates in the target area
    for (const auto& [vx, n] : vx_n_values) {
      for (auto y = std::stoi(mask_match[3]); y <= std::stoi(mask_match[4]); y++) {
        // vy + (vy-1) + (vy-2) ... = y at time steps 1, 2, 3, ...
        // n * vy - (n * (n-1))/2 = y ... equ (2)
        // Rearraging above
        // vy = (2 * y + (n * n-1)) / (2 * n),
        // Note that vy can be negative, unlike vx
        // vy has to be an integer
        if ((2 * y + n * (n - 1)) % (2 * n) != 0) continue;
        const auto vy = (2 * y + n * (n - 1)) / (2 * n);
        velocity_components.emplace_back(vx, vy);
      }
    }

    // For every start vx that becomes zero in the target area,
    // iterate over all possible y coordinates for points in the target area
    // and check whether at time step = n, the y coordinate reached matches the one
    // in the iteration
    for (const auto vx : inf_drops) {
      for (auto y = std::stoi(mask_match[3]); y <= std::stoi(mask_match[4]); y++) {
        // if vy < -(std::abs(y) + 1), it'll overshoot the target y
        // when initial vy is > 0, for every time step it reduces by 1, until it hits 0,
        // then increases by 1. Hence when it reaches a y = 0, vy becomes -vy.
        // after which it will become -vy - 1. If -vy - 1 < y (which is -ve) it'll overshoot
        // the selected y coordinate, hence the maximum initial vy is vy + 1
        for (int vy = -(std::abs(y) + 1); vy <= std::abs(y) + 1; vy++) {
          auto n = vx;
          while (-(n * (n-1))/2 + (n * vy) >= y) {
            // Using equ (2), check whether the y coordinate reached is exactly
            // the target y coordinate
            if(-(n * (n-1))/2 + (n * vy) == y) {
              velocity_components.emplace_back(vx, vy);
              break;
            }
            n++;
          }
        }
      }
    }
  }

  // Remove duplicates
  // std::sort(std::begin(velocity_components), std::end(velocity_components));
  // velocity_components.erase(std::unique(std::begin(velocity_components), std::end(velocity_components)), std::end(velocity_components));
  std::unordered_set<Point, hash_point> s;
  for (const auto& ele : velocity_components) {
    s.insert(ele);
  }
  std::cout << s.size() << '\n';
  return 0;
}
