#include <numeric>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <queue>

#include <chrono>
#include <thread>

using namespace std::literals::chrono_literals;

enum UnitType {
  ELF,
  GOBLIN
};

struct Point {
  int row, col;
  Point(const int row = 0, const int col = 0) : row(row), col(col){}

  bool operator < (const Point& p) const {
    return row < p.row || (row == p.row && col < p.col);
  }

  bool operator > (const Point& p) const {
    return row > p.row || (row == p.row && col > p.col);
  }

  bool operator == (const Point& p) const {
    return row == p.row && col == p.col;
  }

  bool operator != (const Point& p) const {
    return row != p.row || col != p.col;
  }

  friend std::ostream& operator << (std::ostream& os, const Point& p);
};

std::ostream& operator << (std::ostream& os, const Point& p) {
  os << "("<< p.row << ", " << p.col << ")";
  return os;
}

struct point_hash {
  std::size_t operator () (const Point& p) const {
    return p.row ^ p.col;
  }
};

struct Unit {
  UnitType type;
  int hp;
  int ap;

  Unit (const UnitType type, const int hp = 200, const int ap = 3) :
    type(type), hp(hp), ap(ap){}
};

inline void add_keys_to_vec(std::vector<Point> & v, const std::unordered_map<Point, std::unique_ptr<Unit>, point_hash>& m) {
  for (const auto& i : m) {
    v.push_back(i.first);
  }
};

std::vector<Point> get_attack_positions(const Point& p) {
  const std::vector<Point> delta = {
    // In reading order
    Point(-1,0),
    Point(0,-1),
    Point(0,1),
    Point(1,0)
  };

  std::vector<Point> att_pos;
  for (const auto& d : delta) {
    att_pos.emplace_back(p.row + d.row, p.col + d.col);
  }
  return att_pos;
}

std::tuple<bool, int, std::vector<Point>> dfs (const Point& p, const Point& ap, int max_cost, const std::vector<std::vector<char>>& map) {
  std::queue<std::pair<Point, int>> q;
  std::unordered_map<Point, Point, point_hash> parent;
  std::vector<Point> path;
  q.push({p, 0});
  while(!q.empty()) {
    Point cp = q.front().first;
    int cc = q.front().second;
    q.pop();
    if (cp == ap) {
      while(cp != p) {
        path.push_back(cp);
        cp = parent[cp];
      }
      path.push_back(cp);
      return {true, cc, path};
    }
    if (cc + 1 <= max_cost) {
      for (const auto& n : get_attack_positions(cp)) {
        if (parent.find(n) == parent.end() && map[n.row][n.col] == '.') {
          q.push(std::make_pair(n, cc+1));
          parent[n] = cp;
        }
      }
    }
  }
  return {false, -1, path};
}

bool compare_paths(const std::vector<Point>& path1, const std::vector<Point>& path2) {
  int index = 0;
  // This should never happen as the cost is the same when this is called
  if (path1.size() != path2.size()) {
    return path1.size() < path2.size();
  }
  while(index < path1.size()) {
    if (path1[index] < path2[index]) return true;
    else if (path1[index] > path2[index]) return false;
    index++;
  }
  return true;
}

std::tuple<bool, Point> get_next_move (const Point& p, const std::vector<std::vector<char>>& map, const std::vector<Point>& att_pos) {
  int max_cost = std::numeric_limits<int>::max();
  std::vector<std::pair<int, std::vector<Point>>> results;
  for (const auto& ap : att_pos) {
    const auto [found, cost, path] = dfs(p, ap, max_cost, map);
    if (found) {
      results.emplace_back(cost, path);
      if (cost < max_cost) {
        max_cost = cost;
      }
    }
  }
  if (results.empty()) return {false, Point(-1, -1)};
  const auto it = std::min_element(std::begin(results), std::end(results),
    [](const auto& p1, const auto& p2) {return compare_paths(p1.second, p2.second); });
  return {true,it->second[it->second.size() - 2]};
}

void printGrid(const std::vector<std::vector<char>>& map,
  const std::unordered_map<Point, std::unique_ptr<Unit>, point_hash>& units) {
  for (int row = 0; row < map.size(); row++ ) {
    for (int col = 0; col < map[row].size(); col++) {
      std::cout << map[row][col];
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_15_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::fstream file(input);
  std::string line;
  std::vector<std::vector<char>> map;
  std::unordered_map<Point, std::unique_ptr<Unit>, point_hash> units;

  while(std::getline(file, line)) {
    map.emplace_back();
    for (const char c : line) {
      map.back().emplace_back(c);
      if (c == 'E') {
        const auto coord = Point(map.size() - 1, map.back().size() - 1);
        units.insert({coord, std::make_unique<Unit>(UnitType::ELF)});
      }
      else if (c == 'G') {
        const auto coord = Point(map.size() - 1, map.back().size() - 1);
        units.insert({coord, std::make_unique<Unit>(UnitType::GOBLIN)});
      }
    }
  }

  std::vector<Point> occupied;
  // printGrid(map, units);

  bool moved_or_attacked = true;
  int step = 0;
  bool round_ends_midway = false;

  while(units.size() > 1 && moved_or_attacked && !round_ends_midway) {
    round_ends_midway = false;
    // std::cout << "Step: " << step << '\n';
    moved_or_attacked = false;
    std::unordered_set<UnitType> n_unit_types;
    for (const auto& unit : units) {
      n_unit_types.insert(unit.second->type);
    }
    if (n_unit_types.size() == 1) {
      break;
    }
    occupied.clear();
    add_keys_to_vec(occupied, units);
    std:sort(std::begin(occupied), std::end(occupied));
    step++;

    for (int pos_i = 0; pos_i < occupied.size(); pos_i++) {

      n_unit_types.clear();
      for (const auto& unit : units) {
        n_unit_types.insert(unit.second->type);
      }
      if (n_unit_types.size() == 1) {
        round_ends_midway = true;
        break;
      }

      if (units.find(occupied[pos_i]) == units.end()) continue; //unit has been killed
      Point position = occupied[pos_i];
      std::vector<Point> att_pos;

      // Move
      bool need_to_move = true;
      for(const auto& n : get_attack_positions(position)) {
        if (units.find(n) != units.end() && units[n]->type != units[occupied[pos_i]]->type) {
          need_to_move = false;
          break;
        }
      }

      if (need_to_move) {
        auto unit = std::move(units[occupied[pos_i]]);
        units.erase(occupied[pos_i]);
        for (const auto& p : units) {
          if (p.second->type != unit->type) {
            for (const auto ap : get_attack_positions(p.first)) {
              att_pos.push_back(ap);
            }
          }
        }
        const auto [found, new_position] = get_next_move(occupied[pos_i], map, att_pos);
        if (found) {
          map[position.row][position.col] = '.';
          position = new_position;
          moved_or_attacked = true;
          if (unit->type == UnitType::ELF) {
            map[position.row][position.col] = 'E';
          } else if (unit->type == UnitType::GOBLIN) {
            map[position.row][position.col] = 'G';
          }
        }
        units[position] = std::move(unit);
      }

      // Attack
      std::vector<Point> opponent_positions;
      for(const auto& n : get_attack_positions(position)) {
        if (auto it = units.find(n); it != units.end() && it->second->type != units[position]->type) {
          opponent_positions.push_back(n);
        }
      }
      if (opponent_positions.size() > 0) {
        auto p = *std::min_element(std::begin(opponent_positions), std::end(opponent_positions),
          [&units](const Point& p1, const Point& p2) { return units[p1]->hp < units[p2]->hp; } );
        units[p]->hp -= units[position]->ap;
        moved_or_attacked = true;

        if (units[p]->hp <= 0) {
          units.erase(p);
          map[p.row][p.col] = '.';
        }
      }
    }
    // printGrid(map, units);
    // for (const auto& unit : units) {
    //   std::cout << "Round end health: " << unit.second->hp << '\n';
    // }
  }
  if (round_ends_midway) step--;
  auto sum_hp = std::accumulate(std::begin(units), std::end(units), 0,
    [](int total_hp, const auto& p) { return total_hp + p.second->hp; });
  std::cout << sum_hp << ' ' << step << ' ' << sum_hp * step << '\n';
  return 0;
}
