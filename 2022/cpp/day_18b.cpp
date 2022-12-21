#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// TODO: Optimize

using Point = std::array<int, 3>;

enum class Missing {
  X,
  Y,
  Z
};

struct PointHash {
  std::size_t operator () (const Point& p) const {
    std::size_t ans = 0;
    for (const auto ele : p) {
      ans = ans * 10 + ele;
    }
    return ans;
  }
};

struct TupleHash {
  std::size_t operator () (const std::tuple<int, int, Missing>& t) const  {
    return std::get<0>(t) * std::get<1>(t) * (static_cast<int>(std::get<2>(t))+1);
  }
};

std::array<Point, 6> get_neighbours (const Point& p) {
  return std::array<Point, 6>{{
    Point{{p[0]-1, p[1], p[2]}},
    Point{{p[0]+1, p[1], p[2]}},
    Point{{p[0], p[1]-1, p[2]}},
    Point{{p[0], p[1]+1, p[2]}},
    Point{{p[0], p[1], p[2]-1}},
    Point{{p[0], p[1], p[2]+1}}
  }};
};

void print_point(const Point& p) {
  for (auto ele : p)
  std::cout << ele << ' ' ;
  std::cout <<'\n';
}

// Check whether the space is enclosed using bfs
// check_if_enclosed function removes points from surrounding points if the they are not enclosed
// which is found by checking whether anypoint seen in the search does not belong to both
// points and surrounding_points. As the space has been filled in using max and min
// before calling this function this can only occur when the points trail off into open space
void check_if_enclosed (
  std::unordered_map<Point, int, PointHash>& points,
  std::unordered_map<Point, int, PointHash>& surrounding_points,
  Point p
) {
  // std::cout << surrounding_points.size() << '\n';
  // print_point(p);
  std::queue<Point> q;
  std::unordered_set<Point, PointHash> visited;
  q.push(p);
  bool is_enclosed = true;
  int surface_area = 0;
  while (!q.empty()) {
    const auto c = q.front();
    q.pop();
    if (visited.find(c) != visited.end()) continue;
    visited.insert(c);
    for(const auto& neighbour : get_neighbours(c)){
      if (visited.find(neighbour) != visited.end()) continue;
      if (surrounding_points.find(neighbour) == surrounding_points.end() &&
          points.find(neighbour) == points.end()) {
          is_enclosed = false;
      }
      if (surrounding_points.find(neighbour) != surrounding_points.end()) {
        q.push(neighbour);
      }
    }
  }

  if(!is_enclosed) {
    for (const auto & ele : visited) {
      if (auto it = surrounding_points.find(ele); it != surrounding_points.end()) {
        surrounding_points.erase(it);
      }
    }
  }
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_18_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::string line;
  std::fstream file(input);

  std::unordered_map<Point, int, PointHash> points;
  std::unordered_map<Point, int, PointHash> surrounding_points;
  std::unordered_map<std::tuple<int, int, Missing>, std::array<int, 2>,TupleHash> limits;

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
    if(surrounding_points.find(p) != surrounding_points.end()) {
      points[p] = surrounding_points[p];
      surrounding_points.erase(p);
    } else {
      points[p] = 0;
    }

    // Find max and min for every 2d point along 3rd dimension
    for (int d1 = 0; d1 < p.size(); d1++) {
      for (int d2 = d1+1; d2 < p.size(); d2++) {
        const auto p_2d = std::tuple<int, int, Missing>{p[d1], p[d2], static_cast<Missing>(3-d1-d2)};
        if (limits.find(p_2d) == limits.end()) {
          limits[p_2d] = std::array<int, 2>{{p[3-d1-d2], p[3-d1-d2]}};
        } else {
          limits[p_2d][0] = std::min(p[3-d1-d2], limits[p_2d][0]);
          limits[p_2d][1] = std::max(p[3-d1-d2], limits[p_2d][1]);
        }
      }
    }
    for (const auto& neighbour: get_neighbours(p)) {
      points[p]++;
      if (points.find(neighbour) != points.end()) {
        points[neighbour]++;
      } else if (surrounding_points.find(neighbour) != surrounding_points.end()) {
        surrounding_points[neighbour]++;
      } else {
        surrounding_points[neighbour] = 1;
      }
    }
  }

  // Use the max and min to fill in any points not seen yett as surrounding points
  for (const auto [point_2d, limit] : limits) {
    const auto dim = std::get<2>(point_2d);
    const auto min_d = limit[0];
    const auto max_d = limit[1];
    if(dim == Missing::Z) {
      for (int val = min_d-1; val <= max_d+1; val++) {
        const auto p = Point{{std::get<0>(point_2d), std::get<1>(point_2d), val}};
        if (points.find(p) == points.end() && surrounding_points.find(p) == surrounding_points.end()) {
          surrounding_points[p] = 0;
          for (const auto& neighbour: get_neighbours(p)) {
            surrounding_points[p]++;
            if (points.find(neighbour) != points.end()) {
              points[neighbour]++; // shouldnt happen
            } else if (surrounding_points.find(neighbour) != surrounding_points.end()) {
              surrounding_points[neighbour]++;
            } else {
              surrounding_points[neighbour] = 1;
            }
          }
        }
      }
    }
  }

  // Iterate over every surrounding point and check whether the space is enclosed using check_if_enclosed
  // check_if_enclosed function removes points from surrounding points if the they are not enclosed
  // which is found by checking whether anypoint seen in the search does not belong to both
  // poiints and surrounding_points. As the space has been filled in using max and min above,
  // this can only occur when the points trail off into open space
  auto temp = surrounding_points;
  for (const auto& [p, n] : temp) {
    check_if_enclosed(points, surrounding_points, p);
  }

  int total_sa = 0;
  for (const auto& [point, n_neighbours] : points) {
    for (const auto& n : get_neighbours(point)) {
      if (points.find(n) != points.end()) continue;
      if (surrounding_points.find(n) != surrounding_points.end()) continue;
      total_sa++;
    }
  }

  std::cout << total_sa << '\n';
  return 0;
}
