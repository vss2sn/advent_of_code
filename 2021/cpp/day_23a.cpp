#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using Map = std::vector<std::string>;

struct Point {
  int row, col;
  Point(const int row, const int col) : col(col), row(row) {}
  bool operator == (const Point& p) const {
    return row == p.row && col == p.col;
  }
  bool operator < (const Point& p) const {
    return row < p.row || (row == p.row  && col < p.col);
  }
  friend std::ostream& operator << (std::ostream& os, const Point& p);
};

std::ostream& operator << (std::ostream& os, const Point& p) {
  os << "(" << p.row << ", " << p.col << ")";
  return os;
}

struct hash_point {
  std::size_t operator () (const Point& p) const {
    return p.row * 10 + p.col;
  }
};

template <typename T>
void print(const std::vector<T>& v) {
  for (const auto& ele : v) {
    std::cout << ele << '\n';
  }
  std::cout << '\n';
}

std::vector<Point> move_out_of_room(const Map& map, const std::vector<int>& rooms, const Point& p) {
  std::vector<Point> pts;
  std::unordered_set s(std::begin(rooms),std::end(rooms));
  for (int i = 1; i < map[0].size()-1; i++) {
    if (s.find(i) == s.end()) {
      bool obstructed = false;
      for (int j = std::min(i, p.col) + 1; j < std::max(i, p.col); j++) {
        if (map[1][j] != '.') {
          obstructed = true;
          break;
        }
      }
      if (!obstructed) {
        pts.push_back(Point(1, i));
      }
    }
  }
  return pts;
}

std::vector<Point> move_into_room(const Map& map, const std::vector<int>& rooms, const Point& p) {
  std::vector<Point> pts;
  const auto dest_col = rooms[map[p.row][p.col] - 'A'];
  for (int j = std::min(dest_col, p.col) + 1; j < std::max(dest_col, p.col); j++) {
    if (map[1][j] != '.') {
      return {};
    }
  }
  for (int row = 2; row < map.size() - 1; row++) {
    if (!(map[row][dest_col] == '.' || map[row][dest_col] == map[p.row][p.col])) {
      return {};
    }
  }
  int dest_row = 2;
  while (map[dest_row + 1][dest_col] == '.') {
    dest_row++;
  }
  pts.push_back(Point(dest_row, dest_col));
  return pts;
}

std::vector<int> get_rooms(const Map& map) {
  std::vector<int> rooms;
  for (int col = 0; col < map[0].size(); col ++) {
    if (map[2][col] >= 65 && map[2][col] <= 90) {
      rooms.push_back(col);
    }
  }
  return rooms;
}

Map create_final_state(const Map& map, const std::vector<int>& rooms) {
  auto final_map = map;
  for (int index = 0; index < rooms.size(); index++) {
    for (int row = 2; row < map.size()-1; row++) {
      final_map[row][rooms[index]] = 'A' + index;
    }
  }
  return final_map;
}

std::vector<Point> get_movable_points(const Map& map, const std::vector<int>& rooms) {
  std::vector<Point> pts;
  for (int col = 1; col < map[0].size() - 1; col++) {
    int row = 1;
    while (map[row][col] != '#') {
      if (map[row][col] != '.') {
        pts.push_back(Point(row, col));
        break;
      }
      row++;
    }
  }
  return pts;
}

struct compare {
  bool operator () (const std::tuple<Map, int, int>& p1, const std::tuple<Map, int, int>& p2) const {
    return std::get<2>(p1) > std::get<2>(p2);
  }
};

struct hash_map {
  std::size_t operator () (const Map& map) const {
    size_t hash = 0;
    for (const auto& row : map) {
      hash += std::hash<std::string>()(row);
    }
    return hash;
  }
};

const std::unordered_map<char, int> costs {
  {'A', 1},
  {'B', 10},
  {'C', 100},
  {'D', 1000}
};

// Never over-estimate
int heuristic (const Map& map, const std::vector<int>& rooms) {
  int cost = 0;
  for (int row = 1; row < map.size()-1; row++) {
    for (int col = 1; col < map[0].size()-1; col++) {
      if (map[row][col] >= 65 && map[row][col] <= 90) {
        cost += std::abs((col - rooms[map[row][col] - 'A']) * costs.find(map[row][col])->second);
      }
    }
  }
  return cost;
}

std::string convert_map_to_string (const Map& m) {
  std::string str = "";
  for (const auto& row : m) {
    str += row;
  }
  return str;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_23_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  Map map;
  while(std::getline(file, line)) {
    map.push_back(line);
  }

  const auto rooms = get_rooms(map);
  const auto final_map = create_final_state(map, rooms);

  std::unordered_set<Map, hash_map> seen;
  std::priority_queue<std::tuple<Map, int, int>, std::vector<std::tuple<Map, int, int>>, compare> pq;
  pq.push(std::make_tuple(map, 0, heuristic(map, rooms)));

  while(!pq.empty()) {
    auto [c_map, c_cost, cost_with_heuristic] = pq.top();
    pq.pop();
    if (c_map == final_map) {
      std::cout << c_cost << '\n';
      return 0;
    }
    const auto movable_points = get_movable_points(c_map, rooms);
    for (const auto& pt : movable_points) {
      auto reachable = std::vector<Point>();
      if (pt.row == 1) {
        reachable = move_into_room(c_map, rooms, pt);
      } else {
        reachable = move_out_of_room(c_map, rooms, pt);
      }
      for (const auto& move : reachable) {
        if (c_map[move.row][move.col] == '.') {
          std::swap(c_map[move.row][move.col], c_map[pt.row][pt.col]);
          if (seen.find(c_map)!=seen.end()) {
            std::swap(c_map[move.row][move.col], c_map[pt.row][pt.col]);
            continue;
          }
          seen.insert(c_map);
          const auto new_cost = c_cost + costs.find(c_map[move.row][move.col])->second * (std::abs(move.row - pt.row) + std::abs(move.col - pt.col));
          if (c_map == final_map) {
            std::cout << new_cost << '\n';
            return 0;
          }
          pq.push({c_map, new_cost, new_cost + heuristic(c_map, rooms)});
          std::swap(c_map[move.row][move.col], c_map[pt.row][pt.col]);
        }
      }
    }
  }
  return 0;
}
